// Copyright 2020 Dan Kestranek.


#include "Weapons/GSShield.h"
#include "Characters/Heroes/GSHeroCharacter.h"
#include "Characters/Abilities/AttributeSets/GSAttributeSetBase.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"



AGSShield::AGSShield()
{
	AttributeSetBase = CreateDefaultSubobject<UGSAttributeSetBase>(TEXT("AttributeSetBase"));
}


void AGSShield::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributes();
	if (IsValid(AttributeSetBase))
	{
		UE_LOG(LogTemp, Warning, TEXT("AGSShield::SetHealth:%f, max:%f"), Health, AttributeSetBase->GetMaxHealth());
		AttributeSetBase->SetHealth(Health);
		AttributeSetBase->SetShield(0);
		UE_LOG(LogTemp, Warning, TEXT("AGSShield::GetHealth:%f"), AttributeSetBase->GetHealth());
		AttributeSetBase->GetHealth();
	}
	HealthChangedDelegateHandle = WeaponAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGSShield::HealthChanged);
}

void AGSShield::InitializeAttributes()
{
	if (!IsValid(WeaponAbilitySystemComponent))
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = WeaponAbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = WeaponAbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 1, EffectContext);
	if (NewHandle.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AGSShield::InitializeAttributes, success"));
		FActiveGameplayEffectHandle ActiveGEHandle = WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
	}
}

void AGSShield::HealthChanged(const FOnAttributeChangeData& Data)
{
	FGameplayCueParameters GCParameters;
	UE_LOG(LogTemp, Warning, TEXT("AGSShield::HealthChanged:%f"), AttributeSetBase->GetHealth());
	if (AttributeSetBase->GetHealth() <= 0)
	{
		AbilitySystemComponent->ExecuteGameplayCueLocal(FGameplayTag::RequestGameplayTag("GameplayCue.Shield.Break"), GCParameters);
		for (auto Effect : OwnerShieldBreakEffectClass)
		{
			FGameplayEffectContextHandle EffectContext = WeaponAbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);
			FGameplayEffectSpecHandle NewHandle = WeaponAbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
			WeaponAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}

		if (OwningCharacter)
		{
			OwningCharacter->UnEquipWeapon(this);
			OwningCharacter->RemoveWeaponFromInventory(this);
		}
		
		Destroy();
	}
}

float AGSShield::GetHealth()
{
	return AttributeSetBase->GetMaxHealth();
}

UAbilitySystemComponent* AGSShield::GetAbilitySystemComponent() const
{
	return WeaponAbilitySystemComponent;
}