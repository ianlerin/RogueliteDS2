// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/BowState.h"
#include "AIBowState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIBowState : public UBowState
{
	GENERATED_BODY()
protected:
	virtual void OnStageChange() override;
	UFUNCTION()
	void ShootArrow();
	FTimerHandle AimHandler;
	void OnUpdateFocus();
	void StartFocusEnemy();
	UPROPERTY(EditDefaultsOnly)
	float MinAimTime = 2;
	UPROPERTY(EditDefaultsOnly)
	float MaxAimTime = 5;
	UPROPERTY(EditDefaultsOnly)
	float AdditionalYaw = 20;
	UPROPERTY(EditDefaultsOnly)
	float MaxArrowDistance = 1500;
	void OnRestartArrow();
	virtual void OnExitState() override;
	UPROPERTY(EditDefaultsOnly)
	float MinStaminaToAttack = 60;
	virtual void OnBowAttackEnd(bool bWasCancelled) override;
	virtual void OnEnterState() override;
	UFUNCTION()
	void AdjustRotation();
	FTimerHandle AdjustRotationHandler;
	UPROPERTY()
	class UGSGameplayAbility* AttackAbilityInstance = nullptr;
};
