// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "AsyncTaskGameplayEffectChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnGameplayEffectChange, FGameplayTag, EffectGameplayTag, FActiveGameplayEffectHandle, Handle, int32, NewStackCount, int32, OldStackCount);
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAsyncTaskGameplayEffectChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
		static UAsyncTaskGameplayEffectChange* ListenForGameplayEffectStackChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTag EffectGameplayTag);

	// You must call this function manually when you want the AsyncTask to end.
	// For UMG Widgets, you would call it in the Widget's Destruct event.
	UFUNCTION(BlueprintCallable)
		void EndTask();

	UPROPERTY(BlueprintAssignable)
		FOnGameplayEffectChange OnGameplayEffectStackChange;

protected:
	UPROPERTY()
		UAbilitySystemComponent* ASC;

	FGameplayTag EffectGameplayTag;

	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
	virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);

	virtual void GameplayEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount);
};
