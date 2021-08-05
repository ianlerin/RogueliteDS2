// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GSMagicDamageExecution.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSMagicDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGSMagicDamageExecution();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
