// Copyright 2020 Dan Kestranek.


#include "AI/AICaptainPreAttack.h"
#include "GSBlueprintFunctionLibrary.h"
#include "AI/AIStateHandlerComponent.h"

void UAICaptainPreAttack::OnTransition()
{
	UE_LOG(LogTemp, Warning, TEXT(" UAICaptainPreAttack::OnTransition"));
	if (!MyHandler) { return; }
	if (!MyHandler->CheckStateCD(EAIState::EAS_SlamDown))
	{
		bool bLeapSuccess=GSAbilityComp->TryActivateAbilityByClass(LeapAbility);

		UE_LOG(LogTemp, Warning, TEXT(" UAICaptainPreAttack::OnTransition, Activate leap:%i, LeapAbility:%s"), bLeapSuccess,*LeapAbility.Get()->GetName());
		UGSGameplayAbility* LeapInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, LeapAbility);
		if (LeapInstance)
		{
			LeapInstance->OnAbilityEndedDelegate.AddDynamic(this, &UAICaptainPreAttack::OnLeapEnd);
		}
	}
	else
	{
		TransitionState(EAIState::EAS_Follow);
	}
}

void UAICaptainPreAttack::OnLeapEnd()
{
	UGSGameplayAbility* LeapInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, LeapAbility);
	if (LeapInstance)
	{
		LeapInstance->OnAbilityEndedDelegate.RemoveAll(this);
	}
	bool bActivate=GSAbilityComp->TryActivateAbilityByClass(SlamDownAbility);
	UE_LOG(LogTemp, Warning, TEXT(" UAICaptainPreAttack::OnLeapEnd, activate slam:%i"), bActivate);
	if (!bActivate)
	{
		TransitionState(EAIState::EAS_Idle);
	}
}

void UAICaptainPreAttack::CheckDistance()
{

}

void UAICaptainPreAttack::OnExitState()
{
	UE_LOG(LogTemp, Warning, TEXT(" UAICaptainPreAttack::OnExitState"));
	UGSGameplayAbility* LeapInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, LeapAbility);
	if (LeapInstance)
	{
		LeapInstance->OnAbilityEndedDelegate.RemoveAll(this);
	}
}