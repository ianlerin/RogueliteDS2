// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "PlayerIdleState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UPlayerIdleState : public UAIBaseState
{
	GENERATED_BODY()

	virtual void OnEnterState() override;
	virtual void OnRollAdded(FGameplayTag MyTag) override;
};
