// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "AIArcherPreArtackState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIArcherPreArtackState : public UAIBaseState
{
	GENERATED_BODY()

	virtual void OnEnterState() override;
	virtual void OnExitState() override;
	UPROPERTY(EditDefaultsOnly)
	float Radius = 1500;
};
