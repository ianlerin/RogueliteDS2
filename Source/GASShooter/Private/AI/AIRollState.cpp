// Copyright 2020 Dan Kestranek.


#include "AI/AIRollState.h"
#include "Characters/GSCharacterBase.h"
#include "GSBlueprintFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AI/GSHeroAIController.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"
#include "AI/AIStateHandlerComponent.h"

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

void UAIRollState::OnStartListen()
{
	Super::OnStartListen();
	UE_LOG(LogTemp, Warning, TEXT(" UAIRollState::OnStartListen"));
	AActor* FocusedActor = MyGSController->GetFocusActor();
	if (FocusedActor)
	{
		auto FocusedASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(FocusedActor);
		UGSAbilitySystemComponent* GSASC = Cast<UGSAbilitySystemComponent>(FocusedASC);
		if (GSASC)
		{
			GSASC->AbilityActivatedDelegate.AddDynamic(this, &UAIRollState::FocusedActorCast);
		}
	}
}

void UAIRollState::OnStopListen()
{
	AActor* FocusedActor = MyGSController->GetFocusActor();
	if (FocusedActor)
	{
		auto FocusedASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(FocusedActor);
		UGSAbilitySystemComponent* GSASC = Cast<UGSAbilitySystemComponent>(FocusedASC);
		if (GSASC)
		{
			GSASC->AbilityActivatedDelegate.RemoveAll(this);
		}
	}
}

void UAIRollState::FocusedActorCast(FGameplayAbilitySpecHandle Handle)
{
	UE_LOG(LogTemp, Warning, TEXT(" UAIRollState::FocusedActorCast"));
	AActor* FocusedActor = MyGSController->GetFocusActor();
	if (!FocusedActor) { return; }
	if (!CharBase) { return; }
	//ensure enough stamina
	//float Stamina = CharBase->GetStamina();
	float Stamina = 100;
	if (Stamina < 60)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAIRollState::Not enough stamina"));
		return;
	}
	//ensure within hit range
	float Distance = FVector::Dist(FocusedActor->GetActorLocation(), CharBase->GetActorLocation());
	if (Distance > 250)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAIRollState::No distance"));
		return;
	}
	//ensure the focused is facing me
	float Yaw = UGSBlueprintFunctionLibrary::AdditionalYawToRotateToFaceLocation(FocusedActor, CharBase->GetActorLocation(),false);
	if (Yaw > -90 && Yaw < 90)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAIRollState::FocusedActorCast, PerformDodge"));
		bool bRoll = GSAbilityComp->TryActivateAbilityByClass(RollAbility);
		if (!bRoll)
		{
			UE_LOG(LogTemp, Warning, TEXT(" UAICaptainPreAttack::Roll fail"));
			TransitionState(EAIState::EAS_PreAttack);
			return;
		}
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT(" UAICaptainPreAttack::Face incorrect:%f"),Yaw);
	}
}
