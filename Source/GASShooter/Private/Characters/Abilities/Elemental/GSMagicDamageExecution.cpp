// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/Elemental/GSMagicDamageExecution.h"
#include "Characters/Heroes/GSHeroCharacter.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"

struct GSMagicDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	GSMagicDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSAttributeSetBase, Damage, Source, true);

		// Capture the Target's Armor. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSAttributeSetBase, Armor, Target, false);
	}
};

static const GSMagicDamageStatics& MagicDamageStatics()
{
	static GSMagicDamageStatics DStatics;
	return DStatics;
}

UGSMagicDamageExecution::UGSMagicDamageExecution()
{
	RelevantAttributesToCapture.Add(GSMagicDamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(GSMagicDamageStatics().ArmorDef);
}

void UGSMagicDamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UE_LOG(LogTemp, Warning, TEXT("UGSMagicDamageCalc::Execute"));
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
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagicDamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MagicDamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here
	AGSHeroCharacter* MyHero = Cast< AGSHeroCharacter>(SourceAbilitySystemComponent->GetAvatarActor());
	float MitigatedDamage = (UnmitigatedDamage);
	//float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + Armor));

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(MagicDamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}
