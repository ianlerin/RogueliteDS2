// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "AIDeathState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIDeathState : public UAIBaseState
{
	GENERATED_BODY()
	void OnEnterState();
};
