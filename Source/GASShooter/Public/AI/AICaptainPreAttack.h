// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIPreAttackState.h"
#include "AICaptainPreAttack.generated.h"

class UGSGameplayAbility;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAICaptainPreAttack : public UAIPreAttackState
{
	GENERATED_BODY()
protected:
	virtual void OnTransition() override;
	UFUNCTION()
	void OnLeapEnd();
	virtual void CheckDistance() override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGSGameplayAbility> LeapAbility;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGSGameplayAbility> SlamDownAbility;

	UFUNCTION()
	virtual void OnExitState() override;
};
