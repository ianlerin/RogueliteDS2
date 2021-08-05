// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "CastState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UCastState : public UAIBaseState
{
	GENERATED_BODY()
protected:

	virtual void OnEnterState() override;
	virtual void OnExitState() override;
	UPROPERTY()
	class UAsyncTaskGameplayTagAddedRemoved* CastListenEndTask;

	UFUNCTION()
	void OnCastEnd(FGameplayTag MyTag);
};
