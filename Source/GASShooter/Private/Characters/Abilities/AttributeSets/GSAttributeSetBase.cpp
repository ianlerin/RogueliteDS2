// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AttributeSets/GSAttributeSetBase.h"
#include "Characters/GSCharacterBase.h"
#include "GameplayEffect.h"
#include "Characters/Component/ElementStackListener.h"
#include "GameplayEffectExtension.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"
#include "Characters/Abilities/GSGameplayAbility.h"
#include "Player/GSPlayerController.h"

UGSAttributeSetBase::UGSAttributeSetBase()
{
	// Cache tags
	HeadShotTag = FGameplayTag::RequestGameplayTag(FName("Effect.Damage.HeadShot"));
}

void UGSAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	// This is called whenever attributes change, so for max health/mana we want to scale the current totals to match
	Super::PreAttributeChange(Attribute, NewValue);

	// If a Max value changes, adjust current to keep Current % of Current to Max
	if (Attribute == GetBaseMaxHealthAttribute()) // GetMaxHealthAttribute comes from the Macros defined at the top of the header
	{
		RecalculateMaxHealthAfterRestriction();
		//AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	if (Attribute == GetBaseStaminaAttribute()) // GetMaxHealthAttribute comes from the Macros defined at the top of the header
	{
		RecalculateMaxStaminaAfterRestriction();
		//AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMoveSpeedAttribute())
	{
		// Cannot slow less than 150 units/s and cannot boost more than 1000 units/s
		NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
	}
	else if (Attribute == GetStrAttribute())
	{
		//AdjustAttributeForStrChanges(Str.GetCurrentValue(), NewValue);
	}
}

void UGSAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	FGameplayTagContainer SpecAssetTags;
	Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

	// Get the Target actor, which should be our owner
	AActor* TargetActor = nullptr;
	AController* TargetController = nullptr;
	AGSCharacterBase* TargetCharacter = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter = Cast<AGSCharacterBase>(TargetActor);
	}
	// Get the Source actor
	AActor* SourceActor = nullptr;
	AController* SourceController = nullptr;
	AGSCharacterBase* SourceCharacter = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		SourceController = Source->AbilityActorInfo->PlayerController.Get();
		if (SourceController == nullptr && SourceActor != nullptr)
		{
			if (APawn* Pawn = Cast<APawn>(SourceActor))
			{
				SourceController = Pawn->GetController();
			}
		}

		// Use the controller to find the source pawn
		if (SourceController)
		{
			SourceCharacter = Cast<AGSCharacterBase>(SourceController->GetPawn());
		}
		else
		{
			SourceCharacter = Cast<AGSCharacterBase>(SourceActor);
		}

		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			// If character was alive before damage is added, handle damage
			// This prevents damage being added to dead things and replaying death animations
			bool WasAlive = true;

			if (TargetCharacter)
			{
				WasAlive = TargetCharacter->IsAlive();

				if (!TargetCharacter->IsAlive())
				{
					//UE_LOG(LogTemp, Warning, TEXT("%s() %s is NOT alive when receiving damage"), *FString(__FUNCTION__), *TargetCharacter->GetName());
				}
			}

			

			// Apply the damage to shield first if it exists
			const float OldShield = GetShield();
			float DamageAfterShield = LocalDamageDone - OldShield;
			if (OldShield > 0)
			{
				float NewShield = OldShield - LocalDamageDone;
				SetShield(FMath::Clamp<float>(NewShield, 0.0f, GetMaxShield()));
			}

			if (DamageAfterShield > 0)
			{
				// Apply the health change and then clamp it
				const float NewHealth = GetHealth() - DamageAfterShield;

				UE_LOG(LogTemp, Warning, TEXT("UGSAttributeSetBase::NewHealth:%f"), NewHealth);
				SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
			}

			if (TargetCharacter)
			{
				if (WasAlive)
				{
					// This is the log statement for damage received. Turned off for live games.
					UE_LOG(LogTemp, Log, TEXT("%s() %s Damage Received: %f"), *FString(__FUNCTION__), *GetOwningActor()->GetName(), LocalDamageDone);

					// Show damage number for the Source player unless it was self damage
					if (SourceActor != TargetActor)
					{
						AGSPlayerController* PC = Cast<AGSPlayerController>(SourceController);
						if (PC)
						{
							FGameplayTagContainer DamageNumberTags;

							if (Data.EffectSpec.DynamicAssetTags.HasTag(HeadShotTag))
							{
								DamageNumberTags.AddTagFast(HeadShotTag);
							}

							PC->ShowDamageNumber(LocalDamageDone, TargetCharacter, DamageNumberTags);
						}
					}

					if (!TargetCharacter->IsAlive())
					{
						// TargetCharacter was alive before this damage and now is not alive, give XP and Gold bounties to Source.
						// Don't give bounty to self.
						if (SourceController != TargetController)
						{
							// Create a dynamic instant Gameplay Effect to give the bounties
							UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
							GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;

							int32 Idx = GEBounty->Modifiers.Num();
							GEBounty->Modifiers.SetNum(Idx + 2);

							FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
							InfoXP.ModifierMagnitude = FScalableFloat(GetXPBounty());
							InfoXP.ModifierOp = EGameplayModOp::Additive;
							InfoXP.Attribute = UGSAttributeSetBase::GetXPAttribute();

							FGameplayModifierInfo& InfoGold = GEBounty->Modifiers[Idx + 1];
							InfoGold.ModifierMagnitude = FScalableFloat(GetGoldBounty());
							InfoGold.ModifierOp = EGameplayModOp::Additive;
							InfoGold.Attribute = UGSAttributeSetBase::GetGoldAttribute();

							Source->ApplyGameplayEffectToSelf(GEBounty, 1.0f, Source->MakeEffectContext());
						}
					}
				}
			}
		
		}
	}// Damage
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Handle other health changes.
		// Health loss should go through Damage.
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealthAfterRestriction()));
	} // Health
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		// Handle mana changes.
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	} // Mana
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
	
		// Handle stamina changes.
	if (Data.EvaluatedData.Magnitude < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UGSAttributeSetBase::PostGameplayEffectExecute, stamina change:%f"), Data.EvaluatedData.Magnitude);
		GetWorld()->GetTimerManager().SetTimer(StaminaCooldownTimer, StaminaRegenCooldown, false);

	}
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStaminaAfterRestriction()));
		
	}
	else if (Data.EvaluatedData.Attribute == GetShieldAttribute())
	{
		// Handle shield changes.
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield()));
	}
	else if (Data.EvaluatedData.Attribute == GetMagicAttackRateAttribute())
	{
	SetMagicAttackRate(FMath::Clamp(GetMagicAttackRate(), 0.0f, GetMaxMagicAttackRate()));
	}
	else if (Data.EvaluatedData.Attribute == GetBowDrawRateAttribute())
	{
	UE_LOG(LogTemp, Warning, TEXT(" UGSAttributeSetBase::PostGameplayEffectExecute, bow:%f"), GetBowDrawRate());
	}

}

bool UGSAttributeSetBase::IsStaminaCDOnRegen()
{
	return GetWorld()->GetTimerManager().IsTimerActive(StaminaCooldownTimer);
}

float UGSAttributeSetBase::RecalculateMaxManaAfterRestriction()
{
	float TempMax = GetMaxMana();
	for (auto ManaRestriction : ManaRestrictionArray)
	{
		TempMax = (100 - ManaRestriction.Value)/100*TempMax;
	}
	SetMaxManaAfterRestriction(TempMax);
	return GetMaxManaAfterRestriction();
}

float UGSAttributeSetBase::RecalculateMaxHealthAfterRestriction()
{
	float TempMax = GetMaxHealth();
	for (auto HealthRestriction : HealthRestrictionArray)
	{
		TempMax = (100 - HealthRestriction.Value)/100*TempMax;
	}

	SetMaxHealthAfterRestriction(TempMax);
	return GetMaxHealthAfterRestriction();
}

float UGSAttributeSetBase::RecalculateMaxStaminaAfterRestriction()
{
	float TempMax = GetMaxStamina();
	for (auto StaminaRestriction : StaminaRestrictionArray)
	{
		TempMax = (100 - StaminaRestriction.Value)/100* TempMax;
	}
	SetMaxStaminaAfterRestriction(TempMax);
	return GetMaxStaminaAfterRestriction();
}

float UGSAttributeSetBase::GetMaxHealth()
{
	float Max=GetStr() * 10 + GetBaseMaxHealth();
	//UE_LOG(LogTemp, Warning, TEXT("UGSAttributeSetBase::GetMaxHealth, Base:%f, Str:%f, Max:%f"), GetBaseMaxHealth(), GetStr(), Max);
	return Max;
}

float UGSAttributeSetBase::GetMaxMana()
{
	return GetInt() * 10 + GetBaseMaxMana();
}

float UGSAttributeSetBase::GetMaxStamina()
{
	return GetDex() * 10 + GetBaseStamina();
}


void UGSAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, ManaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, StaminaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, MaxShield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, ShieldRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, CharacterLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, XP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, XPBounty, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, Gold, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGSAttributeSetBase, GoldBounty, COND_None, REPNOTIFY_Always);
}

void UGSAttributeSetBase::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UGSAttributeSetBase::AdjustAttributeForStrChanges(float OldStr, float NewStr)
{
	
}

void UGSAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, Health, OldHealth);
}

void UGSAttributeSetBase::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, HealthRegenRate, OldHealthRegenRate);
}

void UGSAttributeSetBase::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, Mana, OldMana);
}

void UGSAttributeSetBase::OnRep_ManaRegenRate(const FGameplayAttributeData& OldManaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, ManaRegenRate, OldManaRegenRate);
}

void UGSAttributeSetBase::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, Stamina, OldStamina);
}

void UGSAttributeSetBase::OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, StaminaRegenRate, OldStaminaRegenRate);
}

void UGSAttributeSetBase::OnRep_Shield(const FGameplayAttributeData& OldShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, Shield, OldShield);
}

void UGSAttributeSetBase::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, MaxShield, OldMaxShield);
}

void UGSAttributeSetBase::OnRep_ShieldRegenRate(const FGameplayAttributeData& OldShieldRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, ShieldRegenRate, OldShieldRegenRate);
}

void UGSAttributeSetBase::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, Armor, OldArmor);
}

void UGSAttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, MoveSpeed, OldMoveSpeed);
}

void UGSAttributeSetBase::OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, CharacterLevel, OldCharacterLevel);
}

void UGSAttributeSetBase::OnRep_XP(const FGameplayAttributeData& OldXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, XP, OldXP);
}

void UGSAttributeSetBase::OnRep_XPBounty(const FGameplayAttributeData& OldXPBounty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, XPBounty, OldXPBounty);
}

void UGSAttributeSetBase::OnRep_Gold(const FGameplayAttributeData& OldGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, Gold, OldGold);
}

void UGSAttributeSetBase::OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGSAttributeSetBase, GoldBounty, OldGoldBounty);
}

void UGSAttributeSetBase::AddResourceRestriction(const UGSGameplayAbility* Ability, EResourceType Type, float Amount)
{
	switch (Type)
	{
	case(EResourceType::ERT_Health):
		HealthRestrictionArray.Add(Ability->GetClass(), Amount);
		RecalculateMaxHealthAfterRestriction();
		break;
	case(EResourceType::ERT_Mana):

		ManaRestrictionArray.Add(Ability->GetClass(), Amount);
		break;
	case(EResourceType::ERT_Stamina):

		StaminaRestrictionArray.Add(Ability->GetClass(), Amount);
		RecalculateMaxStaminaAfterRestriction();
		break;
	}
}

void UGSAttributeSetBase::AddBaseDamageModifier(const UGSGameplayAbility* Ability, float Amount, bool bAdd)
{
	if (bAdd)
	{
		BaseDamageModifierArray.Add(Ability->GetClass(), Amount);
	}
	else
	{
		BaseDamageModifierArray.Remove(Ability->GetClass());
	}
	float TempMax = GetBaseDamage();
	for (auto Modifier: BaseDamageModifierArray)
	{
		TempMax=(100 + Modifier.Value)/100*TempMax;
	}
	SetBaseDamage(TempMax);
}

void UGSAttributeSetBase::RecalculateSlowRatio()
{
	UAbilitySystemComponent* Source = GetOwningAbilitySystemComponent();
	if (Source)
	{
		AActor* SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
		AGSCharacterBase* SourceCharacter = Cast<AGSCharacterBase>(SourceActor);
		int32 ChillStack=Source->GetGameplayEffectCount(SourceCharacter->ElementStackListener->GetChillEffect(), nullptr,true);
		float ToSet = GetBaseMeleeAttackRate()*(1 - 0.04*ChillStack);

		ToSet = FMath::Clamp(ToSet, 0.4f, GetMaxMeleeAttackRate());

		UE_LOG(LogTemp, Warning, TEXT(" UGSAttributeSetBase::RecalculateSlowRatio, %s, ChillStack:%i"), *SourceActor->GetName(), ChillStack);
		SetMeleeAttackRate(ToSet);
		//bow
		int32 Level = Source->GetGameplayEffectCount(SourceCharacter->DrawBowIncreaseEffect, nullptr);
		float BowDrawRateToSet = GetBaseBowDrawRate() + Level * 0.1;
		BowDrawRateToSet = BowDrawRateToSet *(1 - 0.04*ChillStack);
		float ToReturn = FMath::Clamp(BowDrawRateToSet, 0.4f, GetMaxBowDrawRate());
		UE_LOG(LogTemp, Warning, TEXT(" UGSAttributeSetBase::RecalculateSlowRatio, bow:%f"), ToReturn);
		SetBowDrawRate(ToReturn);
	}
	//bow
}