// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AbilityTasks/GSAT_WaitForActionPressed.h"
#include "Characters/Heroes/GSHeroCharacter.h"
#include "Component/InputHandlerComponent.h"

UGSAT_WaitForActionPressed * UGSAT_WaitForActionPressed::WaitForActionPressed(UGameplayAbility * OwningAbility, FName TaskInstance)
{
	UGSAT_WaitForActionPressed* MyObj = NewAbilityTask<UGSAT_WaitForActionPressed>(OwningAbility);
	return MyObj;
}

void UGSAT_WaitForActionPressed::Activate()
{
	AGSHeroCharacter* HeroChar = Cast<AGSHeroCharacter>(GetAvatarActor());
	if (HeroChar)
	{
		HeroChar->GetInputHandlerComp()->OnActionPressedDelegate.AddDynamic(this, &UGSAT_WaitForActionPressed::OnActionPressed);
	}
}

void UGSAT_WaitForActionPressed::OnActionPressed(EGSAbilityInputID AbilityID, bool bPressed)
{
	if (bPressed)
	{
		OnActionPressedDelegate.Broadcast(AbilityID);
	}
}