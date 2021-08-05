// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "GSAT_InternalEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FIntervalEventUpdate);
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSAT_InternalEvent : public UAbilityTask
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintAssignable)
		FIntervalEventUpdate IntervalUpdateDelegate;

	UPROPERTY(BlueprintAssignable)
		FIntervalEventUpdate GeneralUpdateDelegate;

	float TotalDuration = 0;
	float Interval = 0;
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static UGSAT_InternalEvent* StartIntervalClockEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, float TotalDuration, float Interval);
protected:

	UFUNCTION()
		void OnGeneralTimerEnd();

	UFUNCTION()
		void OnLoopTimerUpdate();

	virtual void Activate() override;

	UPROPERTY()
		FTimerHandle LoopHandler;

	UPROPERTY()
		FTimerHandle GeneralHandler;
};
