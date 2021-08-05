// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/GSGameplayAbility.h"
#include "DeepBurnAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UDeepBurnAbility : public UGSGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, float> BurnMap;
public:
	float GetBurnAdditionalDamage();
};
