// Copyright 2020 Dan Kestranek.


#include "AI/AIDeathState.h"

void UAIDeathState::OnEnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIDeathState::OnEnterState"));
	MyGSController->SetFocus(nullptr);
	//MyGSController->ClearFocus(EAIFocusPriority::Default);
}