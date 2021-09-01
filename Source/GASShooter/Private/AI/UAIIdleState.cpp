// Copyright 2020 Dan Kestranek.


#include "AI/UAIIdleState.h"
#include "Characters/Heroes/GSHeroCharacter.h"
#include "TimerManager.h"
#include "AI/GSHeroAIController.h"

void UUAIIdleState::OnEnterState()
{
	Super::OnEnterState();
	UE_LOG(LogTemp, Warning, TEXT("UUAIIdleState::OnEnterState"));
	//StartIdleCheckTimer();
	if (MyGSController)
	{
		MyGSController->OnEnemySetDelegate.RemoveAll(this);
		MyGSController->OnEnemySetDelegate.AddUniqueDynamic(this, &UUAIIdleState::OnEnemyUpdate);
	}
	OnEnemyUpdate(MyGSController->EnemyDetected);
}

void UUAIIdleState::OnExitState()
{
	Super::OnExitState();

	UE_LOG(LogTemp, Warning, TEXT("UUAIIdleState::OnExitState"));
	MyGSController->GetWorldTimerManager().ClearTimer(IdleCheckHandler);
	if (MyGSController)
	{
		MyGSController->OnEnemySetDelegate.RemoveAll(this);
	}
}

void UUAIIdleState::OnEnemyUpdate(APawn* EnemyPawn)
{
	AGSHeroCharacter* HeroEnemyPawn = Cast<AGSHeroCharacter>(EnemyPawn);
	UE_LOG(LogTemp, Warning, TEXT("UUAIIdleState::OnEnemyUpdate"));
	if (!HeroEnemyPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("UUAIIdleState::OnEnemyUpdate, no enemy"));
		return;
	}
	else if (HeroEnemyPawn->IsAlive())
	{
		UE_LOG(LogTemp, Warning, TEXT("UUAIIdleState::OnEnemyUpdate, pre attack"));
		TransitionState(EAIState::EAS_PreAttack);
	}
	else
	{
		return;
	}
}

void UUAIIdleState::StartIdleCheckTimer()
{
	FTimerDelegate IdleCheckDelegate;
	IdleCheckDelegate.BindUFunction(this, "CheckIdleState");
	MyGSController->GetWorldTimerManager().SetTimer(IdleCheckHandler, IdleCheckDelegate, 0.25, true);

}

void UUAIIdleState::CheckIdleState()
{
	if (MyGSController->EnemyDetected)
	{
		TransitionState(EAIState::EAS_PreAttack);
	}
}