// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "GameplayAbilities/Public/GameplayAbilitySpec.h"
#include "AIDodgeState.generated.h"
class UGameplayAbility;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIDodgeState : public UAIBaseState
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility>DodgeAbility;
	UPROPERTY()
	FGameplayAbilitySpecHandle DodgeHandle;
protected:
	UFUNCTION()
	void OnDodgeEnd();

	virtual void OnExitState()override;
	virtual void OnEnterState()override;
};
