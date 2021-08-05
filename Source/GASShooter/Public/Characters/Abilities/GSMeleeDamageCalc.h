// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GSMeleeDamageCalc.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSMeleeDamageCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGSMeleeDamageCalc();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
