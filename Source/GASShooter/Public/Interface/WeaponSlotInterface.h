// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponSlotInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponSlotInterface : public UInterface
{
	GENERATED_BODY()
};

class UTextureImage2D;
/**
 * 
 */
class GASSHOOTER_API IWeaponSlotInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UTextureImage2D* Get2DImage() = 0;
};
