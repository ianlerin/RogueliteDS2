// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GASShooter/GASShooter.h"
#include "AI/AIBaseState.h"
#include "SlamDownState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlamDownTransition, ESlamDownState, SlamState);
/**
 * 
 */
UCLASS()
class GASSHOOTER_API USlamDownState : public UAIBaseState
{
	GENERATED_BODY()
	
protected:

	
	UPROPERTY(BlueprintAssignable)
	FOnSlamDownTransition OnSlamDownTransitionDelegate;
	FTimerHandle GroundCheckHandler;

protected:

	UPROPERTY(BlueprintReadOnly)
	ESlamDownState SlamDownState = ESlamDownState::ESDS_None;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility>AttackAbility;

	virtual void OnEnterState() override;

	virtual void OnExitState() override;

	UFUNCTION()
	void OnStateEnd(bool bWasCancelled);
	UFUNCTION()
	void CheckGround();
public:
	ESlamDownState GetSlamDownState()
	{
		return SlamDownState;
	}
	UFUNCTION(BlueprintCallable)
	void SetStage(ESlamDownState StateToSet);
};
