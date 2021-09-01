// Copyright 2020 Dan Kestranek.


#include "AI/BowState.h"
#include "GSBlueprintFunctionLibrary.h"
#include "GASShooter/GASShooter.h"
#include "AI/GSHeroAIController.h"
#include "Camera/PlayerCameraManager.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Abilities/GSGameplayAbility.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"

void UBowState::OnEnterState()
{
	Super::OnEnterState();
	SetStage(EBowDrawState::EBDS_Start);
	UE_LOG(LogTemp, Warning, TEXT(" UBowState::OnEnterState"));
	UGSGameplayAbility* AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
	if (AttackAbilityInstance)
	{
		AttackAbilityInstance->OnAbilityEndedDelegate.AddDynamic(this, &UBowState::OnBowAttackEnd);
	}
	APlayerCameraManager* Manager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (Manager&&MyPlayerController)
	{
		Manager->ViewPitchMax = MaxBowPitch;

		Manager->ViewPitchMin = MinBowPitch;
	}
}


void UBowState::OnExitState()
{
	Super::OnExitState();
	UE_LOG(LogTemp, Warning, TEXT(" UBowState::OnExitState"));
	UGSGameplayAbility* AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
	if (AttackAbilityInstance)
	{
		AttackAbilityInstance->OnAbilityEndedDelegate.RemoveAll(this);
	}
	APlayerCameraManager* Manager = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (Manager&&MyPlayerController)
	{
		Manager->ViewPitchMax = 90;

		Manager->ViewPitchMin = -90;
	}
}

void UBowState::OnBowAttackEnd(bool bWasCancelled)
{
	TransitionState(EAIState::EAS_Idle);
}


void UBowState::OnStageChange()
{

}