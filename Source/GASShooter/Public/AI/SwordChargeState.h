// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GASShooter/GASShooter.h"
#include "AI/AIBaseState.h"
#include "SwordChargeState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChargeStateTransition, ESwordChargeState, ChargeState);
/**
 * 
 */
UCLASS()
class GASSHOOTER_API USwordChargeState : public UAIBaseState
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnChargeStateTransition OnChargeStateTransitionDelegate;
	UFUNCTION(BlueprintCallable)
	void SetStage(ESwordChargeState StateToSet);

protected:
	UPROPERTY(BlueprintReadOnly)
	ESwordChargeState ChargeState;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility>AttackAbility;

	virtual void OnEnterState() override;

	virtual void OnExitState() override;
	UFUNCTION()
	void OnStateEnd(bool bWasCancelled);
public:
	ESwordChargeState GetChargeState()
	{
		return ChargeState;
	}

};
