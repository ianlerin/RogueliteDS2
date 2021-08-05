// Copyright 2020 Dan Kestranek.


#include "AI/AIPreAttack_BlockState.h"
#include "AI/GSHeroAIController.h"	
#include "Characters/Abilities/GSGameplayAbility.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"

void UAIPreAttack_BlockState::OnEnterState()
{
	Super::OnEnterState();
    bool bSuccess=GSAbilityComp->TryActivateAbilityByClass(BlockAbility);
	UE_LOG(LogTemp, Warning, TEXT("UAIPreAttack_BlockState::OnEnterState:%i"), bSuccess);
	FTimerDelegate RepeatBlockDelegate;
	RepeatBlockDelegate.BindUFunction(this, "OnRepeatBlocking");
//	MyGSController->GetWorldTimerManager().SetTimer(RepeatBlockHandle, RepeatBlockDelegate, 0.5, true);
}

void UAIPreAttack_BlockState::OnExitState()
{
	Super::OnExitState();
}


void UAIPreAttack_BlockState::OnRepeatBlocking()
{
	bool bSuccess = GSAbilityComp->TryActivateAbilityByClass(BlockAbility);

	UE_LOG(LogTemp, Warning, TEXT("UAIPreAttack_BlockState::OnRepeatBlocking:%i"), bSuccess);
	if (bSuccess)
	{
		//MyGSController->GetWorldTimerManager().ClearTimer(RepeatBlockHandle);
	}
}