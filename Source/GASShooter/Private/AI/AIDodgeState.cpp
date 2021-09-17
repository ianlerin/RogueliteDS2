// Copyright 2020 Dan Kestranek.


#include "AI/AIDodgeState.h"
#include "GSBlueprintFunctionLibrary.h"
#include "Characters/Abilities/GSGameplayAbility.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"


void UAIDodgeState::OnEnterState()
{
	Super::OnEnterState();
	if (GSAbilityComp)
	{
		GSAbilityComp->TryActivateAbilityByClass(DodgeAbility);
		UGSGameplayAbility* DodgeAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp,DodgeAbility);
		if (DodgeAbilityInstance)
		{
			DodgeAbilityInstance->OnAbilityEndedDelegate.AddDynamic(this, &UAIDodgeState::OnDodgeEnd);
		}
	}
}

void UAIDodgeState::OnExitState()
{
	Super::OnExitState();
	UGSGameplayAbility* DodgeAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, DodgeAbility);
	if (DodgeAbilityInstance)
	{
		DodgeAbilityInstance->OnAbilityEndedDelegate.RemoveAll(this);
	}
}

void UAIDodgeState::OnDodgeEnd(bool bWasCancelled)
{
	TransitionState(EAIState::EAS_Idle);
}