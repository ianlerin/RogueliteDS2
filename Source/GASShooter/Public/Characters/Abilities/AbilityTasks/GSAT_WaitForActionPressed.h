// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GASShooter/GASShooter.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GSAT_WaitForActionPressed.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionPressed, EGSAbilityInputID, IDName);
/**
 *
 */
UCLASS()
class GASSHOOTER_API UGSAT_WaitForActionPressed : public UAbilityTask
{
	GENERATED_BODY()
protected:
	UFUNCTION()
		void OnActionPressed(EGSAbilityInputID AbilityID, bool bPressed);
public:

	UPROPERTY(BlueprintAssignable)
		FOnActionPressed OnActionPressedDelegate;
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UGSAT_WaitForActionPressed* WaitForActionPressed(UGameplayAbility* OwningAbility,FName TaskInstance);
	virtual void Activate() override;
};
