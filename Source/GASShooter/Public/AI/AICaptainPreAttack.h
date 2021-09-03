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

	bool CheckLeapCondition();
	virtual void OnTransition() override;
	UFUNCTION()
	void OnLeapEnd();
	virtual void CheckDistance() override;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGSGameplayAbility> LeapAbility;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGSGameplayAbility> SlamDownAbility;
	UPROPERTY(EditDefaultsOnly)
	float MinDistanceToLeap	 = 500;
	UFUNCTION()
	virtual void OnExitState() override;
};
