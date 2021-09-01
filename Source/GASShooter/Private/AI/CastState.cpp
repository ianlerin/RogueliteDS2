// Copyright 2020 Dan Kestranek.


#include "AI/CastState.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"
#include "Characters/Abilities/AsyncTaskGameplayTagAddedRemoved.h"
void UCastState::OnEnterState()
{
	Super::OnEnterState();
	FGameplayTagContainer CastContainer;
	UE_LOG(LogTemp, Warning, TEXT(" UCastState::OnEnterState"));
	CastContainer.AddTag(FGameplayTag::RequestGameplayTag("Status.Chanelling"));
	CastListenEndTask = UAsyncTaskGameplayTagAddedRemoved::ListenForGameplayTagAddedOrRemoved(GSAbilityComp, CastContainer);
	CastListenEndTask->OnTagRemoved.AddDynamic(this, &UCastState::OnCastEnd);
}

void UCastState::OnCastEnd(FGameplayTag MyTag)
{

	TransitionState(EAIState::EAS_Idle);
}

void UCastState::OnExitState()
{

	Super::OnExitState();

	UE_LOG(LogTemp, Warning, TEXT(" UCastState::OnExitState"));
	CastListenEndTask->EndTask();
}