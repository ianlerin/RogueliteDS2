// Copyright 2020 Dan Kestranek.


#include "AI/AIRollState.h"


void UAIRollState::OnEnterState()
{
	Super::OnEnterState();
}


void UAIRollState::OnExitState()
{
	Super::OnExitState();
}

void UAIRollState::OnRollEnded(FGameplayTag MyTag)
{
	TransitionState(EAIState::EAS_Idle);
}