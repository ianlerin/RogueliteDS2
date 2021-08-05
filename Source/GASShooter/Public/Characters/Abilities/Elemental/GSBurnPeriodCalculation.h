// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GSBurnPeriodCalculation.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSBurnPeriodCalculation : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec & Spec) const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility>LongBurnAbility;

	UPROPERTY(EditDefaultsOnly)
	float BaseBurnDuration = 1.0f;
};
