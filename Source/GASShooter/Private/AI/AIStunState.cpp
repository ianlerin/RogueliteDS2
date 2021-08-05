// Copyright 2020 Dan Kestranek.


#include "AI/AIStunState.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"

void UAIStunState::OnEnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIStunState::OnEnterState"));
	Super::OnEnterState();
	if(MyGSController)
	MyGSController->SetFocus(nullptr);
}

void UAIStunState::OnTagRemoved(FGameplayTag MyTag)
{
	for (auto Tag : DisableMovementTag)
	{
		if (GSAbilityComp->HasMatchingGameplayTag(Tag))
		{
			UE_LOG(LogTemp, Warning, TEXT("UAIStunState::OnTagRemoved, %s not removed"),*Tag.GetTagName().ToString());
			return;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UAIStunState::OnTagRemoved, transition out"));
	if (MyGSController)
	{
		if (MyGSController->EnemyDetected)
		{
			TransitionState(StateAfterStun);
		}
		else
		{
			TransitionState(EAIState::EAS_Idle);
		}
	}
	else
	{
		TransitionState(EAIState::EAS_Idle);
	}
}

void UAIStunState::OnExitState()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIStunState::OnExitState"));
	Super::OnExitState();
}
