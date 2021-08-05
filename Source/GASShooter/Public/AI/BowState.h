// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GASShooter/GASShooter.h"
#include "AI/AIBaseState.h"
#include "BowState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBowAimTransition, EBowDrawState, PrevDrawState);
class UGameplayAbility;
class AGSArrow;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UBowState : public UAIBaseState
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility>AttackAbility;
	UPROPERTY()
	FGameplayAbilitySpecHandle AttackHandle;
	virtual void OnEnterState() override;
	virtual void OnExitState() override;
	UFUNCTION()
	virtual void OnStateEnd();
	UPROPERTY(BlueprintAssignable)
	FOnBowAimTransition OnBowAimTransitionDelegate;
	UPROPERTY(BlueprintReadWrite)
	AGSArrow* GSArrow = nullptr;
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EBowDrawState BowDrawState = EBowDrawState::EBDS_None;
	
	float MaxBowPitch = 40;
	float MinBowPitch = -40;

	virtual void OnStageChange();
	UFUNCTION(BlueprintCallable)
	void SetStage(EBowDrawState DrawStateToSet)
	{
		BowDrawState = DrawStateToSet;
		OnBowAimTransitionDelegate.Broadcast(DrawStateToSet);
		OnStageChange();
	}
};
