// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/Public/GameplayEffectTypes.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "GASShooter/GASShooter.h"
#include "UObject/NoExportTypes.h"
#include "AIBaseState.generated.h"
class UAIStateHandlerComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class GASSHOOTER_API UAIBaseState : public UObject
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EAIState MyState;
	//how many hits to take in a row before dodging
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 DodgeDamageCount = 3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float StateCDDuration = 0;
	UPROPERTY()
	UAIStateHandlerComponent* MyHandler;
	UPROPERTY()
	class AGSCharacterBase* CharBase = nullptr;
	UPROPERTY()
	class AGSHeroAIController* MyGSController = nullptr;
	UPROPERTY()
	class AGSPlayerController* MyPlayerController = nullptr;
	UPROPERTY()
	class APawn* MyPawn = nullptr;
	UPROPERTY()
	class UGSAbilitySystemComponent* GSAbilityComp = nullptr;
	UPROPERTY()
	TArray<FGameplayTag>DisableMovementTag;
	UPROPERTY()
	class UAsyncTaskGameplayTagAddedRemoved* GameplayTagListenTask = nullptr;
	UPROPERTY()
	class UAsyncTaskGameplayTagAddedRemoved* RollListenTask = nullptr;
	UPROPERTY()
	class UAsyncTaskGameplayTagAddedRemoved* CastListenTask = nullptr;
	//sometimes state need a cooldown for AI to check
	FTimerHandle StateCDHandle;
	bool bActive = false;
public:

	virtual UAIBaseState* AttemptToTransition(EAIState NewState);
	void Setup(UAIStateHandlerComponent* MyHandlerToSet);
	virtual void OnEnterState();
	virtual void OnExitState();
	UFUNCTION()
	void OnCastStart(FGameplayTag MyTag);
	UFUNCTION()
	virtual void OnRollAdded(FGameplayTag MyTag);
	UFUNCTION()
	virtual void OnRollEnded(FGameplayTag MyTag);
	UFUNCTION()
	virtual void OnTagAdded(FGameplayTag MyTag);
	UFUNCTION()
	virtual void OnTagRemoved(FGameplayTag MyTag);
	UFUNCTION()
	virtual void OnDamageStackChange(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount);
	bool GetIsOnTimer();
protected:
	void TransitionState(EAIState NewState);
	bool bCheckValidAttackCondition();
	UFUNCTION()
	void OnDeath(AGSCharacterBase* Died);
};
