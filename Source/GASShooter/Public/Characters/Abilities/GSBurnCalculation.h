// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GSBurnCalculation.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSBurnCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
		UGSBurnCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
