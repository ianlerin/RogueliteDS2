// Copyright 2020 Dan Kestranek.


#include "AI/PlayerIdleState.h"
#include "AI/AIStateHandlerComponent.h"
void UPlayerIdleState::OnEnterState()
{
	Super::OnEnterState();
}

void UPlayerIdleState::OnRollAdded(FGameplayTag MyTag)
{
	TransitionState(EAIState::EAS_Roll);
}

