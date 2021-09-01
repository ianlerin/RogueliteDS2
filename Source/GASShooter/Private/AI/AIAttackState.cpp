// Copyright 2020 Dan Kestranek.


#include "AI/AIAttackState.h"
#include "GSBlueprintFunctionLibrary.h"
#include "GASShooter/GASShooter.h"
#include "AI/GSHeroAIController.h"
#include "Characters/Abilities/GSGameplayAbility.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"

void UAIAttackState::OnEnterState()
{
	Super::OnEnterState();
	if (GSAbilityComp)
	{
		bool bActivateAttack=GSAbilityComp->TryActivateAbilityByClass(AttackAbility);
		if (!bActivateAttack)
		{
			TransitionState(EAIState::EAS_PreAttack);
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("UAIAttackState::OnEnterState,bActivateAttack:%i"), bActivateAttack);
		UGSGameplayAbility* AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
		if (AttackAbilityInstance)
		{
			AttackAbilityInstance->OnNextComboDelegate.AddDynamic(this, &UAIAttackState::OnNextCombo);
			AttackAbilityInstance->OnAbilityEndedDelegate.AddDynamic(this, &UAIAttackState::OnAttackEnd);
		}
	}
	MyGSController->SetFocus(nullptr);
	//MyGSController->ClearFocus(EAIFocusPriority::Default);
}

void UAIAttackState::OnExitState()
{
	Super::OnExitState();
	UE_LOG(LogTemp, Warning, TEXT("UAIAttackState::OnExitState"));
	UGSGameplayAbility* AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
	if (AttackAbilityInstance)
	{
		AttackAbilityInstance->OnNextComboDelegate.RemoveAll(this);
		AttackAbilityInstance->OnAbilityEndedDelegate.RemoveAll(this);
	}

	//MyGSController->SetFocus(MyGSController->EnemyDetected);
}

void UAIAttackState::OnNextCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIAttackState::OnNextCombo"));
	UGSGameplayAbility* AttackAbilityInstance = UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(GSAbilityComp, AttackAbility);
	if (AttackAbilityInstance)
	{
		AttackAbilityInstance->ExecuteNextCombo(EGSAbilityInputID::PrimaryFire);
	}
}

void UAIAttackState::OnAttackEnd(bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("UAIAttackState::OnAttackEnd"));
	TransitionState(EAIState::EAS_PreAttack);
}