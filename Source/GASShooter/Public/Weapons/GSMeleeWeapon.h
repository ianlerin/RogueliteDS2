// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/GSWeapon.h"
#include "GSMeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSMeleeWeapon : public AGSWeapon
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float MeleeWeaponDamage = 10;
public:
	float GetMeleeWeaponDamage()
	{
		return MeleeWeaponDamage;
	}
};
