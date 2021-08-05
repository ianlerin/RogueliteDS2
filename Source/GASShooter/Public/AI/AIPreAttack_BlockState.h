// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIPreAttackState.h"
#include "AIPreAttack_BlockState.generated.h"

class UGSGameplayAbility;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIPreAttack_BlockState : public UAIPreAttackState
{
	GENERATED_BODY()
protected:
	virtual void OnEnterState() override;
	virtual void OnExitState() override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGSGameplayAbility> BlockAbility;

	FTimerHandle RepeatBlockHandle;

	UFUNCTION()
	void OnRepeatBlocking();
};
