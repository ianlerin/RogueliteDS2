// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/GSMeleeDamageCalc.h"
#include "Characters/Heroes/GSHeroCharacter.h"
#include "Weapons/GSMeleeWeapon.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"

struct GSMeleeDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	GSMeleeDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSAttributeSetBase, Damage, Source, true);

		// Capture the Target's Armor. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSAttributeSetBase, Armor, Target, false);
	}
};

static const GSMeleeDamageStatics& MeleeDamageStatics()
{
	static GSMeleeDamageStatics DStatics;
	return DStatics;
}

UGSMeleeDamageCalc::UGSMeleeDamageCalc()
{
	RelevantAttributesToCapture.Add(MeleeDamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(MeleeDamageStatics().ArmorDef);
}

void UGSMeleeDamageCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UE_LOG(LogTemp, Warning, TEXT("UGSMeleeDamageCalc::Execute"));
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MeleeDamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MeleeDamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here
	AGSHeroCharacter* MyHero = Cast< AGSHeroCharacter>(SourceAbilitySystemComponent->GetAvatarActor());
	if (MyHero)
	{
		AGSMeleeWeapon* MeleeWeapon = Cast<AGSMeleeWeapon>(MyHero->GetCurrentWeapon());
		if (MeleeWeapon)
		{
			UnmitigatedDamage+=MeleeWeapon->GetMeleeWeaponDamage();
		}
		UnmitigatedDamage=MyHero->GetBaseDamage() + MyHero->GetBaseDamageSpecfic(EDamageType::EDT_Melee)+ UnmitigatedDamage;
	}
	float MitigatedDamage = (UnmitigatedDamage);
	//float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + Armor));

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(MeleeDamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}
