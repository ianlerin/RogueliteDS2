// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GSStaminaCalculation.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSStaminaCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UGSStaminaCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
