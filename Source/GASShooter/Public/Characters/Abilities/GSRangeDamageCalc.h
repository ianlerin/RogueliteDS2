// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GSRangeDamageCalc.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSRangeDamageCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
protected:

	float HeadShotMultiplier = 1.5;
	UGSRangeDamageCalc();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
