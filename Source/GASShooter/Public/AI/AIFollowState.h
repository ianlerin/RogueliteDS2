// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "AIFollowState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIFollowState : public UAIBaseState
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinAttackDistance = 200;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinYawDifferenceToAttack = 30;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EAIState FollowupState = EAIState::EAS_Attack;
	FTimerHandle FollowCheckHandle;
	UFUNCTION()
	void CheckFollowState();
	void StartFollowCheckTimer();
	virtual void OnExitState()override;
	virtual void OnEnterState()override;

	bool CheckValidAttackCondition();
};
