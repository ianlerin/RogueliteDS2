// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/Public/GameplayAbilitySpec.h"
#include "AI/AIBaseState.h"
#include "AIAttackState.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIAttackState : public UAIBaseState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayAbility>AttackAbility;
	UPROPERTY()
	FGameplayAbilitySpecHandle AttackHandle;
protected:
	virtual void OnExitState()override;
	virtual void OnEnterState()override;
	UFUNCTION()
	void OnNextCombo();
	UFUNCTION()
	void OnAttackEnd();
};
