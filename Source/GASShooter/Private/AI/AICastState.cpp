// Copyright 2020 Dan Kestranek.


#include "AI/AICastState.h"
#include "GSBlueprintFunctionLibrary.h"
#include "AI/GSHeroAIController.h"
#include "Player/GSPlayerController.h"
#include "Characters/GSCharacterBase.h"

void UAIPreCastState::OnEnterState()
{
	Super::OnEnterState();
	OnStartCastCountdown();
}

void UAIPreCastState::OnStartCastCountdown()
{
	UE_LOG(LogTemp, Warning, TEXT(" UAIPreCastState::OnStartCastCountdown"));
	if (MyGSController)
	{
		FTimerDelegate CastSpellDelayDelegate;
		CastSpellDelayDelegate.BindUFunction(this, "CastSpell");
		MyGSController->GetWorldTimerManager().SetTimer(CastSpellDelayHandle, CastSpellDelayDelegate, CastDelay, false);
	}
}

void UAIPreCastState::OnExitState()
{
	Super::OnExitState();
	MyGSController->GetWorldTimerManager().ClearTimer(CastSpellDelayHandle);
}

void UAIPreCastState::CastSpell()
{
	UE_LOG(LogTemp, Warning, TEXT(" UAIPreCastState::CastSpell"));
	if (MyGSController)
	{
		MyGSController->SetFocus(nullptr);
	}

	if (CharBase)
	{
		float Dist = (CharBase->GetActorLocation() - MyGSController->EnemyDetected->GetActorLocation()).Size();
		float CurrentMinDist = 10000;
		TSubclassOf<UGameplayAbility>Ability;
		UE_LOG(LogTemp, Warning, TEXT(" UAIPreCastState::CastSpell:Dist:%f"), Dist);
		for (auto DistanceData : AbilityDistanceMap)
		{
			if (Dist < DistanceData.Key&&Dist<CurrentMinDist)
			{
				CurrentMinDist = Dist;
				Ability = DistanceData.Value;
			
			}

		}
		bool bActivate = GSAbilityComp->TryActivateAbilityByClass(Ability);

		//UE_LOG(LogTemp, Warning, TEXT(" UAIPreCastState::CastSpell success:%i, Ability:%s"), bActivate,*Ability->GetName());
		if (!bActivate)
		{
			OnStartCastCountdown();
		}
	}
}

