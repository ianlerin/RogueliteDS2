// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystemComponent.h"
#include "GSAT_ListenForGEChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGameplayEffectStackChanged, FGameplayTag, EffectGameplayTag, FActiveGameplayEffectHandle, Handle, int32, NewStackCount, int32, OldStackCount);
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSAT_ListenForGEChange : public UAbilityTask
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UGSAT_ListenForGEChange* ListenForGameplayEffectStackChange(UAbilitySystemComponent* Comp, FGameplayTag EffectGameplayTag);

	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
		void EndTask();

	UPROPERTY(BlueprintAssignable)
	FOnGameplayEffectStackChanged OnGameplayEffectStackChange;

protected:
	UPROPERTY()
		UAbilitySystemComponent* ASC;

	FGameplayTag EffectGameplayTag;

	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);

	virtual void GameplayEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount);
};
