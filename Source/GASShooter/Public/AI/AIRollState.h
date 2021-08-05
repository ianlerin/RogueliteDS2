// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "AIRollState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIRollState : public UAIBaseState
{
	GENERATED_BODY()
	virtual void OnRollEnded(FGameplayTag MyTag) override;

	virtual void OnEnterState() override;
	virtual void OnExitState() override;
};
