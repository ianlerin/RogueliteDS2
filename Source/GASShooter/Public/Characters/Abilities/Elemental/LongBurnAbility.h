// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/GSGameplayAbility.h"
#include "LongBurnAbility.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API ULongBurnAbility : public UGSGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<int32, float> BurnExtendMap;
public:

	float GetBurnExtendDuration();

};
