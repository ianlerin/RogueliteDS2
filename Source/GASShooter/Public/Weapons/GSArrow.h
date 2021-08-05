// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/GSProjectile.h"
#include "GSArrow.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSArrow : public AGSProjectile
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FTransform EquipTransform;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ArrowSpeed = 200;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ArrowDamage = 50;
};
