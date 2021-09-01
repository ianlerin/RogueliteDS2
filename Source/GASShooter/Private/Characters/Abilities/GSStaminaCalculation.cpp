// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/GSStaminaCalculation.h"
#include "Characters/Heroes/GSHeroCharacter.h"
#include "GSBlueprintFunctionLibrary.h"
#include "Characters/Abilities/AttributeSets/GSAttributeSetBase.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"

struct GSStaminaStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Stamina);
	GSStaminaStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGSAttributeSetBase, Stamina, Source, true);
	}
};

static const GSStaminaStatics& StaminaStatics()
{
	static GSStaminaStatics DStatics;
	return DStatics;
}

UGSStaminaCalculation::UGSStaminaCalculation()
{
	RelevantAttributesToCapture.Add(GSStaminaStatics().StaminaDef);
}

void UGSStaminaCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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
	

	float ToAdd = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GSStaminaStatics().StaminaDef, EvaluationParameters, ToAdd);

	UE_LOG(LogTemp, Warning, TEXT("UGSStaminaCalculation::Execute, final 1:%f"), ToAdd);
	AGSHeroCharacter* MyHero = Cast< AGSHeroCharacter>(SourceAbilitySystemComponent->GetAvatarActor());
	if (MyHero)
	{
		if (MyHero->IsStaminaRegenOnCD())
		{
			ToAdd=0.0f;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("UGSStaminaCalculation::Execute, final 2:%f"), ToAdd);
	// Set the Target's damage meta attribute
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GSStaminaStatics().StaminaProperty, EGameplayModOp::Additive, ToAdd));
}
