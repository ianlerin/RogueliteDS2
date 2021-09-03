// Copyright 2020 Dan Kestranek.


#include "AI/SlamDownState.h"
#include "Characters/Abilities/GSGameplayAbility.h"
#include "GSBlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "AI/GSHeroAIController.h"

void USlamDownState::OnEnterState()
{
	Super::OnEnterState();

	UE_LOG(LogTemp, Warning, TEXT(" USlamDownState::OnEnterState"));
	SetStage(ESlamDownState::ESDS_Start);
	UGSGameplayAbility* AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
	if (AttackAbilityInstance)
	{
		AttackAbilityInstance->OnAbilityEndedDelegate.AddDynamic(this, &USlamDownState::OnStateEnd);
	}
}

void USlamDownState::SetStage(ESlamDownState StateToSet)
{
	SlamDownState = StateToSet;
	OnSlamDownTransitionDelegate.Broadcast(SlamDownState);
	if (StateToSet == ESlamDownState::ESDS_Loop)
	{
		UE_LOG(LogTemp, Warning, TEXT(" USlamDownState::SetStage, loop"));
		FTimerDelegate FollowCheckDelegate;
		FollowCheckDelegate.BindUFunction(this, "CheckGround");
		MyPawn->GetWorldTimerManager().SetTimer(GroundCheckHandler, FollowCheckDelegate,0.03,true);
	}
	if (StateToSet == ESlamDownState::ESDS_End)
	{
		UE_LOG(LogTemp, Warning, TEXT(" USlamDownState::SetStage, end"));
		MyPawn->GetWorldTimerManager().ClearTimer(GroundCheckHandler);
	}
	if (StateToSet == ESlamDownState::ESDS_None)
	{
		
		UE_LOG(LogTemp, Warning, TEXT(" USlamDownState::SetStage, none"));
	}
}

void USlamDownState::OnStateEnd(bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT(" USlamDownState::OnStateEnd"));
	SetStage(ESlamDownState::ESDS_None);
	TransitionState(EAIState::EAS_Idle);
}


void USlamDownState::OnExitState()
{
	UE_LOG(LogTemp, Warning, TEXT(" USlamDownState::OnExitState"));
	Super::OnExitState();
	UGSGameplayAbility* AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
	if (AttackAbilityInstance)
	{
		AttackAbilityInstance->OnAbilityEndedDelegate.RemoveAll(this);
	}
	
	MyPawn->GetWorldTimerManager().ClearTimer(GroundCheckHandler);
}

void USlamDownState::CheckGround()
{
	UE_LOG(LogTemp, Warning, TEXT(" USlamDownState::CheckGround"));
	if (!MyPawn) { return; }
	TArray<AActor*>ActorsToIgnore;
	ActorsToIgnore.Add(MyPawn);
	FVector StartLoc = MyPawn->GetActorLocation();
	FVector EndLoc = StartLoc + FVector(0, 0, -100);
	FHitResult Res;
	bool bBlock=UKismetSystemLibrary::LineTraceSingle(this, StartLoc, EndLoc, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::Persistent, Res, false);
	if (bBlock)
	{
		if (SlamDownState == ESlamDownState::ESDS_Loop)
		{
			UE_LOG(LogTemp, Warning, TEXT(" USlamDownState::CheckGround loop"), *Res.Actor->GetName());
			SetStage(ESlamDownState::ESDS_End);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" USlamDownState::CheckGround not loop"), *Res.Actor->GetName());
			OnStateEnd(false);
		}
	
	}
}