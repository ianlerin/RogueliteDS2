// Copyright 2020 Dan Kestranek.

#include "AI/AIPreAttackState.h"
#include "AI/GSHeroAIController.h"	
#include "Characters/Abilities/AsyncTaskGameplayEffectChange.h"
#include "Kismet/KismetMathLibrary.h"

void UAIPreAttackState::OnExitState()
{
	Super::OnExitState();
	MyGSController->GetWorldTimerManager().ClearTimer(LocRestartHandler);

	MyGSController->GetWorldTimerManager().ClearTimer(DistanceCheckHandler);
	MyGSController->StopMovement();
	if (SpawnedDamageEffectListener)
	{
		SpawnedDamageEffectListener->EndTask();
	}

}

void UAIPreAttackState::OnEnterState()
{
	Super::OnEnterState();
	if (!MyGSController->EnemyDetected)
	{
		//TransitionState(EAIState::EAS_Idle);
		//return;
	}
	//CurrentRandomMoveRequired=MaxRandomMoveRequired
	CurrentRandomMoveRequired = UKismetMathLibrary::RandomIntegerInRange(MinRandomMoveRequired, MaxRandomMoveRequired);
	UE_LOG(LogTemp, Warning, TEXT("UAIPreAttackState::OnEnterState, Enemy:%s"),*MyGSController->EnemyDetected->GetName());
	MyGSController->SetFocus(MyGSController->EnemyDetected);
	//StartSmoothRotation();

	MoveToRandLocationAroundRange();
	
	FTimerDelegate LocRestartDelegate;
	LocRestartDelegate.BindUFunction(this, "MoveToRandLocationAroundRange");
	FTimerDelegate DistanceCheckDelegate;

	DistanceCheckDelegate.BindUFunction(this, "CheckDistance");
	MyGSController->GetWorldTimerManager().SetTimer(DistanceCheckHandler, DistanceCheckDelegate, 0.25, true);
	MyGSController->GetWorldTimerManager().SetTimer(LocRestartHandler, LocRestartDelegate, RestartRandomMovementTimer, true);

	SpawnedDamageEffectListener = UAsyncTaskGameplayEffectChange::ListenForGameplayEffectStackChange(GSAbilityComp, FGameplayTag::RequestGameplayTag("Event.DamageCount"));
	if (SpawnedDamageEffectListener)
	{
		SpawnedDamageEffectListener->OnGameplayEffectStackChange.AddDynamic(this, &UAIBaseState::OnDamageStackChange);
	}
}

void UAIPreAttackState::OnDamageStackChange(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount)
{
	if (OldStackCount > NewStackCount) { return; }
	RotCount++;
	if (RotCount > CurrentRandomMoveRequired)
	{
		RotCount = 0;
		OnTransition();
	}
	UE_LOG(LogTemp, Warning, TEXT("UAIPreAttackState::OnDamageStackChange,RotCount:%i"), RotCount);
	if (RotCount > CurrentRandomMoveRequired)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAIPreAttackState::MoveToRandLocationAroundRange,transition to follow"));
		OnTransition();
	}
}

void UAIPreAttackState::OnTransition()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIPreAttackState::OnTransition"));
	TransitionState(EAIState::EAS_Follow);
}

void UAIPreAttackState::MoveToRandLocationAroundRange()
{
	if (!MyGSController->GetPawn() || !MyGSController->EnemyDetected) { return; }
	if (RotCount > CurrentRandomMoveRequired)
	{
		RotCount = 0;
		UE_LOG(LogTemp, Warning, TEXT("UAIPreAttackState::MoveToRandLocationAroundRange,transition to follow"));
		OnTransition();
	}
	else
	{
		if (!MyGSController->GetPawn() || !MyGSController->EnemyDetected) { return; }
		int32 bIsLeft = UKismetMathLibrary::RandomIntegerInRange(0, 1);

		float RandRot = UKismetMathLibrary::RandomFloatInRange(45, 90);
		if (bIsLeft)
		{
			RandRot = UKismetMathLibrary::RandomFloatInRange(-90, -45);
		}
		float RandLength = UKismetMathLibrary::RandomFloatInRange(300, 400);
		FVector DiffVec = MyGSController->GetPawn()->GetActorLocation() - MyGSController->EnemyDetected->GetActorLocation();
		FVector ForwardVec = DiffVec.GetSafeNormal();
		FVector Vec = UKismetMathLibrary::GreaterGreater_VectorRotator(ForwardVec, FRotator(0, RandRot, 0));
		TargetLoc = MyGSController->EnemyDetected->GetActorLocation() + Vec * RandLength;
		auto Request = MyGSController->MoveToLocation(TargetLoc, 100);
		
		Async(EAsyncExecution::TaskGraph, [this]()
		{
			
		});
	}
	RotCount++;

}

void UAIPreAttackState::CheckDistance()
{
	FVector DiffVec = MyGSController->GetPawn()->GetActorLocation() - MyGSController->EnemyDetected->GetActorLocation();
	if (DiffVec.Size() >= MinDistance)
	{
		MoveToRandLocationAroundRange();
	}
}