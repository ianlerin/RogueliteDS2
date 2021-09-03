// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/GSBurnCalculation.h"
#include "Characters/Heroes/GSHeroCharacter.h"
#include "Characters/Abilities/Elemental/DeepBurnAbility.h"
#include "GSBlueprintFunctionLibrary.h"
#include "Characters/Component/ElementStackListener.h"
#include "Characters/Abilities/AttributeSets/GSAttributeSetBase.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"

struct GSBurnDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	GSBurnDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSAttributeSetBase, Damage, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSAttributeSetBase, Health, Target, true);
	}
};

static const GSBurnDamageStatics& BurnDamageStatics()
{
	static GSBurnDamageStatics DStatics;
	return DStatics;
}

UGSBurnCalculation::UGSBurnCalculation()
{
	RelevantAttributesToCapture.Add(GSBurnDamageStatics().DamageDef);

	RelevantAttributesToCapture.Add(GSBurnDamageStatics().HealthDef);

}

void UGSBurnCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	UGSAbilitySystemComponent* GSSourceAbilityComp = Cast<UGSAbilitySystemComponent>(SourceAbilitySystemComponent);
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
	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GSBurnDamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here

	UE_LOG(LogTemp, Warning, TEXT("UGSBurnCalculation::Execute,Damage:%f"), Damage);
	AGSHeroCharacter* MyHero = Cast< AGSHeroCharacter>(SourceAbilitySystemComponent->GetAvatarActor());
	auto DeepBurnAbilityClass= MyHero->GetElementStackListener()->GetDeepBurnAbility();
	UGSGameplayAbility* GSGameplayAbility=UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(SourceAbilitySystemComponent, DeepBurnAbilityClass);
	UDeepBurnAbility* DeepBurn = Cast<UDeepBurnAbility>(GSGameplayAbility);
	if (DeepBurn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGSBurnCalculation::Execute,has deepburn"));
		UnmitigatedDamage= UnmitigatedDamage+DeepBurn->GetBurnAdditionalDamage();
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("UGSBurnCalculation::Execute,no deepburn"));
	}
	float MitigatedDamage = (UnmitigatedDamage);

	UE_LOG(LogTemp, Warning, TEXT("UGSBurnCalculation::Execute,MitigatedDamage:%f"), MitigatedDamage);
	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GSBurnDamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}
}
