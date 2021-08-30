// Copyright 2020 Dan Kestranek.


#include "AI/AICaptainPreAttack.h"
#include "Characters/GSCharacterBase.h"
#include "GSBlueprintFunctionLibrary.h"
#include "AI/GSHeroAIController.h"
#include "AI/AIStateHandlerComponent.h"

void UAICaptainPreAttack::OnTransition()
{
	UE_LOG(LogTemp, Warning, TEXT(" UAICaptainPreAttack::OnTransition"));
	if (!MyHandler) { return; }

	
	if (CheckLeapCondition())
	{
		bool bLeapSuccess=GSAbilityComp->TryActivateAbilityByClass(LeapAbility);

		UE_LOG(LogTemp, Warning, TEXT(" UAICaptainPreAttack::OnTransition, Activate leap:%i, LeapAbility:%s"), bLeapSuccess,*LeapAbility.Get()->GetName());
		UGSGameplayAbility* LeapInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, LeapAbility);
		if (LeapInstance)
		{
			//LeapInstance->OnAbilityEndedDelegate.AddDynamic(this, &UAICaptainPreAttack::OnLeapEnd);
		}
	}
	else
	{
		TransitionState(EAIState::EAS_Follow);
	}
}

bool UAICaptainPreAttack::CheckLeapCondition()
{
	AActor* FocusedActor = MyGSController->GetFocusActor();
	// at least a certain range from character
	float Distance = FVector::Dist(FocusedActor->GetActorLocation(), CharBase->GetActorLocation());
	bool bLargerThanMinDistance = Distance < MinDistanceToLeap;
	// at least higher than character
	bool bIsHigherThanFocused = CharBase->GetActorLocation().Z > FocusedActor->GetActorLocation().Z;
	// if not on cd
	bool bIsOnCD = MyHandler->CheckStateCD(EAIState::EAS_SlamDown);
	return bLargerThanMinDistance && bIsHigherThanFocused && !bIsOnCD;
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