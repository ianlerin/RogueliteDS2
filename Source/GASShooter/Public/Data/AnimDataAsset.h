// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GASShooter/GASShooter.h"
#include "Engine/DataAsset.h"
#include "AnimDataAsset.generated.h"

class UBlendSpace;
class UAimOffsetBlendSpace1D;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeaponAnimData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlendSpace* BSIdleWalkRun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAimOffsetBlendSpace1D* AimOffSet;
};

UCLASS(Blueprintable)
class GASSHOOTER_API UAnimDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TMap<EWeaponType, FWeaponAnimData> MyIdleWalkRunAnimMap;
};
