// Copyright 2020 Dan Kestranek.


#include "AI/AIFollowState.h"
#include "Characters/GSCharacterBase.h"
#include "GSBlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"
#include "AI/GSHeroAIController.h"

void UAIFollowState::OnExitState()
{
	Super::OnExitState();
	if (MyGSController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAIFollowState::OnExitState"));
		MyGSController->GetWorldTimerManager().ClearTimer(FollowCheckHandle);
		MyGSController->StopMovement();
		CharBase->GetMovementComponent()->StopMovementImmediately();
	}
}

void UAIFollowState::OnEnterState()
{
	Super::OnEnterState();
	//MyGSController->SetFocus(MyGSController->EnemyDetected);
	UE_LOG(LogTemp, Warning, TEXT("UAIFollowState::OnEnterState"));
	if (MyGSController)
	{
		MyGSController->MoveToActor(MyGSController->EnemyDetected, 100);
	}
	StartFollowCheckTimer();
}

void UAIFollowState::StartFollowCheckTimer()
{
	FTimerDelegate FollowCheckDelegate;
	FollowCheckDelegate.BindUFunction(this, "CheckFollowState");
	MyGSController->GetWorldTimerManager().SetTimer(FollowCheckHandle, FollowCheckDelegate, 0.25, true);

}

bool UAIFollowState::CheckValidAttackCondition()
{
	if (!MyGSController->EnemyDetected || !MyGSController->GetPawn()) { return false; }
	//rotation and movement check
	float Yaw = UGSBlueprintFunctionLibrary::AdditionalYawToRotateToFaceLocation(MyGSController->GetPawn(), MyGSController->EnemyDetected->GetActorLocation(), false);
	if (FMath::Abs(Yaw) > MinYawDifferenceToAttack) { return false; }
	float Distance = (MyGSController->EnemyDetected->GetActorLocation() - MyGSController->GetPawn()->GetActorLocation()).Size();
	if (Distance > MinAttackDistance) { return false; }
	if (!MyGSController->CheckIfEnemyInSight()) { return false; }
	return true;
}

void UAIFollowState::CheckFollowState()
{
	if (!bActive) { return; }
	if (!MyGSController->GetWorldTimerManager().IsTimerActive(FollowCheckHandle)) { return; }
	UE_LOG(LogTemp, Warning, TEXT("UAIFollowState::CheckFollowState, bactive:%i"),bActive);
	if (CheckValidAttackCondition())
	{
		TransitionState(FollowupState);
	}
	if (MyGSController->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		if (!bActive) { return; }
		UE_LOG(LogTemp, Warning, TEXT("UAIFollowState::CheckFollowState, move to "));
		MyGSController->MoveToActor(MyGSController->EnemyDetected, 100);
	}
}
