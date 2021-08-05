// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "AIPreAttackState.generated.h"

class UAsyncTaskGameplayEffectChange;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIPreAttackState : public UAIBaseState
{
	GENERATED_BODY()
protected:
	FRotator CurrentRotToSet;
	FRotator OriginalRotation;
	int32 CurrentRandomMoveRequired = 0;
	FVector TargetLoc;
	float RotTimeRequired;
	UPROPERTY(EditDefaultsOnly)
	float MinDistance = 600;
	int32 RotCount = 0;
	virtual void OnExitState()override;
	virtual void OnEnterState()override;
	FTimerHandle DistanceCheckHandler;
	void OnDamageStackChange(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount);
	UFUNCTION()
	void MoveToRandLocationAroundRange();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RotRatePerSecond = 30;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RestartRandomMovementTimer = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MinRandomMoveRequired = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxRandomMoveRequired = 3;
	UPROPERTY()
	class UAsyncTaskGameplayEffectChange* SpawnedDamageEffectListener;
	FTimerHandle LocRestartHandler;

	virtual void OnTransition();
	UFUNCTION()
	virtual void CheckDistance();


};
