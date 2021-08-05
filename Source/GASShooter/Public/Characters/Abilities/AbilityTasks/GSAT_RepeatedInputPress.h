// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilities/Public/Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GSAT_RepeatedInputPress.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSAT_RepeatedInputPress : public UAbilityTask
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FInputPressDelegate		OnPress;

	UFUNCTION()
		void OnPressCallback();

	virtual void Activate() override;

	/** Wait until the user presses the input button for this ability's activation. Returns time this node spent waiting for the press. Will return 0 if input was already down. */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UGSAT_RepeatedInputPress* RepeatedInputPress(UGameplayAbility* OwningAbility, bool bTestAlreadyPressed = false);

protected:

	float StartTime;
	bool bTestInitialState;
	FDelegateHandle DelegateHandle;
};
