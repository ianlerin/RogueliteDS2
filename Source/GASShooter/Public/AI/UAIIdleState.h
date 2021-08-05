// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "UAIIdleState.generated.h"

/**
 * 
 */

UCLASS(Blueprintable)
class GASSHOOTER_API UUAIIdleState : public UAIBaseState
{
	GENERATED_BODY()
protected:
	FTimerHandle IdleCheckHandler;
	virtual void OnEnterState() override;
	virtual void OnExitState()override;
	UFUNCTION()
	void OnEnemyUpdate(APawn* EnemyPawn);
	void StartIdleCheckTimer();
	void CheckIdleState();
};
