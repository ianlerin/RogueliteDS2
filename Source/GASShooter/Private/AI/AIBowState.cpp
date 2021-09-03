// Copyright 2020 Dan Kestranek.


#include "AI/AIBowState.h"
#include "Characters/GSCharacterBase.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Characters/Abilities/GSGameplayAbility.h"
#include "Weapons/GSArrow.h"
#include "GSBlueprintFunctionLibrary.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"
#include "AI/GSHeroAIController.h"


void UAIBowState::OnEnterState()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnEnterState"));
	Super::OnEnterState();
	if (MyGSController)
	{
		bool bActivate = GSAbilityComp->TryActivateAbilityByClass(AttackAbility);
		if (!AttackAbility.Get())
		{
			UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnEnterState no attack ability"));
			return;
		}
		if (!MyGSController->GetPawn())
		{
			UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnEnterState no pawn"));
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnEnterState:%s trying to use %s"), *MyGSController->GetPawn()->GetName(), *AttackAbility->GetName());
	}
	//MyGSController->SetFocus(nullptr);
}

void UAIBowState::OnStageChange()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnStageChange"));
	Super::OnStageChange();
	switch (BowDrawState)
	{

	case(EBowDrawState::EBDS_Draw):

		UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnStageChange,draw"));
		break;
	case(EBowDrawState::EBDS_Aim):

		UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnStageChange,draw aim"));
		StartFocusEnemy();
		break;
	case(EBowDrawState::EBDS_Start):

		UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnStageChange,draw start"));
		//OnRestartArrow();
		break;
	}
}

void UAIBowState::OnExitState()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnExitState"));
	Super::OnExitState();
	if (MyGSController)
	{
		MyGSController->GetWorldTimerManager().ClearTimer(AimHandler);
	}
	MyGSController->GetWorldTimerManager().ClearTimer(AdjustRotationHandler);
}

void UAIBowState::StartFocusEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIBowState::StartFocusEnemy"));
	float RandAimTime=UKismetMathLibrary::RandomFloatInRange(MinAimTime, MaxAimTime);
	FTimerDelegate AimDelegate;
	AimDelegate.BindUFunction(this, "ShootArrow");
	MyGSController->GetWorldTimerManager().SetTimer(AimHandler, AimDelegate, RandAimTime, false);
	FTimerDelegate  AdjustRotationDelegate;
	AdjustRotationDelegate.BindUFunction(this, "AdjustRotation");
	MyGSController->GetWorldTimerManager().SetTimer(AdjustRotationHandler, AdjustRotationDelegate, 0.1, true);
	AdjustRotation();
}


void UAIBowState::AdjustRotation()
{
	
	
	//FRotator RotationToSet=UKismetMathLibrary::FindLookAtRotation(GSArrow->GetActorLocation(), MyGSController->EnemyDetected->GetActorLocation());
	FRotator RotationToSet = UKismetMathLibrary::FindLookAtRotation(CharBase->GetActorLocation(), MyGSController->EnemyDetected->GetActorLocation());
	MyGSController->SetControlRotation(RotationToSet);
	RotationToSet.Yaw = RotationToSet.Yaw + AdditionalYaw;
	CharBase->SetActorRotation(RotationToSet);
	UE_LOG(LogTemp, Warning, TEXT("UAIBowState::MyGSController, aimrot:%s, control rot:%s"), *RotationToSet.ToString(), *MyGSController->GetControlRotation().ToString());
	if (GSArrow)
	{
		
	}

}

void UAIBowState::OnUpdateFocus()
{

}

void UAIBowState::OnRestartArrow()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnRestartArrow"));
	if (!MyGSController->EnemyDetected)
	{
		TransitionState(EAIState::EAS_PreAttack);
		return;
	}
	FVector DiffVec = MyGSController->GetPawn()->GetActorLocation() - MyGSController->EnemyDetected->GetActorLocation();
	if (DiffVec.Size() > MaxArrowDistance)
	{
		TransitionState(EAIState::EAS_PreAttack);
	}
	else
	{
		GSAbilityComp->TryActivateAbilityByClass(AttackAbility);
	}
}


void UAIBowState::ShootArrow()
{
	if (!AttackAbilityInstance)
	{
		AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
	}
	if (AttackAbilityInstance)
	{
		SetStage(EBowDrawState::EBDS_Release);
		//AttackAbilityInstance->ExecuteNextCombo(EGSAbilityInputID::PrimaryFire);
	}
}

void UAIBowState::OnBowAttackEnd(bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("UAIBowState::OnBowAttackEnd"));
	float Stamina = CharBase->GetStamina();
	if (bWasCancelled&&Stamina< MinStaminaToAttack)
	{
		TransitionState(EAIState::EAS_PreAttack);
	}
	else
	{
		bool bActivate = GSAbilityComp->TryActivateAbilityByClass(AttackAbility);
	}
}

