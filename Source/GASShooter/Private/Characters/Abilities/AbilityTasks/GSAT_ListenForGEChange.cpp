// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AbilityTasks/GSAT_ListenForGEChange.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
UGSAT_ListenForGEChange * UGSAT_ListenForGEChange::ListenForGameplayEffectStackChange(UAbilitySystemComponent * AbilitySystemComponent, FGameplayTag InEffectGameplayTag)
{
	UGSAT_ListenForGEChange* ListenForGameplayEffectStackChange = NewObject<UGSAT_ListenForGEChange>();
	ListenForGameplayEffectStackChange->ASC = AbilitySystemComponent;
	ListenForGameplayEffectStackChange->EffectGameplayTag = InEffectGameplayTag;

	if (!IsValid(AbilitySystemComponent) || !InEffectGameplayTag.IsValid())
	{
		ListenForGameplayEffectStackChange->EndTask();
		return nullptr;
	}

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(ListenForGameplayEffectStackChange, &UGSAT_ListenForGEChange::OnActiveGameplayEffectAddedCallback);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(ListenForGameplayEffectStackChange, &UGSAT_ListenForGEChange::OnRemoveGameplayEffectCallback);

	return ListenForGameplayEffectStackChange;
}

void UGSAT_ListenForGEChange::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->OnActiveGameplayEffectAddedDelegateToSelf.RemoveAll(this);
		ASC->OnAnyGameplayEffectRemovedDelegate().RemoveAll(this);
	}

	//SetReadyToDestroy();
	MarkPendingKill();
}

void UGSAT_ListenForGEChange::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent * Target, const FGameplayEffectSpec & SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(EffectGameplayTag) || GrantedTags.HasTagExact(EffectGameplayTag))
	{
		ASC->OnGameplayEffectStackChangeDelegate(ActiveHandle)->AddUObject(this, &UGSAT_ListenForGEChange::GameplayEffectStackChanged);
		OnGameplayEffectStackChange.Broadcast(EffectGameplayTag, ActiveHandle, 1, 0);
	}
}

void UGSAT_ListenForGEChange::OnRemoveGameplayEffectCallback(const FActiveGameplayEffect & EffectRemoved)
{
	FGameplayTagContainer AssetTags;
	EffectRemoved.Spec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectRemoved.Spec.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(EffectGameplayTag) || GrantedTags.HasTagExact(EffectGameplayTag))
	{
		OnGameplayEffectStackChange.Broadcast(EffectGameplayTag, EffectRemoved.Handle, 0, 1);
	}
}

void UGSAT_ListenForGEChange::GameplayEffectStackChanged(FActiveGameplayEffectHandle EffectHandle, int32 NewStackCount, int32 PreviousStackCount)
{
	OnGameplayEffectStackChange.Broadcast(EffectGameplayTag, EffectHandle, NewStackCount, PreviousStackCount);
}
