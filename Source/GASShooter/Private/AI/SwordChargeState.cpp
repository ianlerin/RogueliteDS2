// Copyright 2020 Dan Kestranek.


#include "AI/SwordChargeState.h"

void USwordChargeState::OnEnterState()
{
	Super::OnEnterState();

	UE_LOG(LogTemp, Warning, TEXT(" USwordChargeState::OnEnterState"));
	SetStage(ESwordChargeState::ECS_Start);
	UGSGameplayAbility* AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
	if (AttackAbilityInstance)
	{
		AttackAbilityInstance->OnAbilityEndedDelegate.AddDynamic(this, &USwordChargeState::OnStateEnd);
	}
}

void USwordChargeState::SetStage(ESwordChargeState StateToSet)
{
	ChargeState = StateToSet;
	OnChargeStateTransitionDelegate.Broadcast(StateToSet);
}

void USwordChargeState::OnStateEnd()
{
	UE_LOG(LogTemp, Warning, TEXT(" USwordChargeState::OnStateEnd"));
	TransitionState(EAIState::EAS_Idle);
}


void USwordChargeState::OnExitState()
{
	UE_LOG(LogTemp, Warning, TEXT(" USwordChargeState::OnExitState"));
	Super::OnExitState();
	UGSGameplayAbility* AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
	if (AttackAbilityInstance)
	{
		AttackAbilityInstance->OnAbilityEndedDelegate.RemoveAll(this);
	}
}