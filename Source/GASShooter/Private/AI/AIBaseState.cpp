// Copyright 2020 Dan Kestranek.

#include "AI/AIBaseState.h"
#include "AI/GSHeroAIController.h"
#include "Characters/Abilities/AttributeSets/GSAttributeSetBase.h"
#include "Characters/Abilities/AsyncTaskGameplayEffectChange.h"
#include "GSBlueprintFunctionLibrary.h"
#include "Player/GSPlayerController.h"
#include "Characters/GSCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Characters/Abilities/AsyncTaskAttributeChanged.h"
#include "Characters/Abilities/AsyncTaskGameplayTagAddedRemoved.h"
#include "Characters/Abilities/GSAbilitySystemComponent.h"
#include "GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "AI/AIStateHandlerComponent.h"
void UAIBaseState::OnEnterState()
{
	if (MyGSController)
	{
		CharBase = Cast<AGSCharacterBase>(MyGSController->GetPawn());
		if (CharBase)
		{
			CharBase->OnCharacterPreDied.RemoveAll(this);
			CharBase->OnCharacterPreDied.AddUniqueDynamic(this, &UAIBaseState::OnDeath);
			CharBase->OnDestroyed.AddUniqueDynamic(this, &UAIBaseState::OnDestroyed);
		}
	}
	else
	{
		CharBase = Cast<AGSCharacterBase>(MyPlayerController->GetPawn());
	}
	FGameplayTagContainer MyContainer;
	DisableMovementTag.Empty();
	DisableMovementTag.Add(FGameplayTag::RequestGameplayTag("Status.Getup"));
	DisableMovementTag.Add(FGameplayTag::RequestGameplayTag("Status.Liftup"));
	DisableMovementTag.Add(FGameplayTag::RequestGameplayTag("Status.Parried"));
	DisableMovementTag.Add(FGameplayTag::RequestGameplayTag("Status.Stagger"));
	DisableMovementTag.Add(FGameplayTag::RequestGameplayTag("Status.Knockback"));
	for (auto Tag : DisableMovementTag)
	{
		MyContainer.AddTag(Tag);
	}

	GameplayTagListenTask = UAsyncTaskGameplayTagAddedRemoved::ListenForGameplayTagAddedOrRemoved(GSAbilityComp, MyContainer);
	if (!GameplayTagListenTask)
	{
	}
	if (CharBase)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAIBaseState::Rebound add:%s, state:%s"), *CharBase->GetName(), *GetName());
	}

	GameplayTagListenTask->OnTagAdded.RemoveAll(this);

	GameplayTagListenTask->OnTagAdded.AddUniqueDynamic(this, &UAIBaseState::OnTagAdded);
	GameplayTagListenTask->OnTagRemoved.RemoveAll(this);
	GameplayTagListenTask->OnTagRemoved.AddUniqueDynamic(this, &UAIBaseState::OnTagRemoved);
	FGameplayTagContainer RollContainer;
	RollContainer.AddTag(FGameplayTag::RequestGameplayTag("Status.Roll"));
	RollListenTask = UAsyncTaskGameplayTagAddedRemoved::ListenForGameplayTagAddedOrRemoved(GSAbilityComp, RollContainer);
	RollListenTask->OnTagAdded.AddDynamic(this, &UAIBaseState::OnRollAdded);
	RollListenTask->OnTagRemoved.AddDynamic(this, &UAIBaseState::OnRollEnded);
	FGameplayTagContainer CastContainer;
	CastContainer.AddTag(FGameplayTag::RequestGameplayTag("Status.Chanelling"));
	CastListenTask = UAsyncTaskGameplayTagAddedRemoved::ListenForGameplayTagAddedOrRemoved(GSAbilityComp, CastContainer);
	CastListenTask->OnTagAdded.AddDynamic(this, &UAIBaseState::OnCastStart);
	bActive = true;
	if (StateCDDuration > 0)
	{
		CharBase->GetWorldTimerManager().SetTimer(StateCDHandle, StateCDDuration, false);
	}
	auto AttributeToListen =CharBase->AttributeSetBase->GetStaminaAttribute();
	AttributeChangeListener = UAsyncTaskAttributeChanged::ListenForAttributeChange(GSAbilityComp, AttributeToListen);
	if (AttributeChangeListener)
	{
		AttributeChangeListener->OnAttributeChanged.AddDynamic(this, &UAIBaseState::OnStaminaChange);
	}
}

void UAIBaseState::OnExitState()
{
	UE_LOG(LogTemp, Warning, TEXT(" UAIBaseState::OnExitState2"));
	if (MyGSController)
	{
	}

	if (CharBase)
	{
		CharBase->OnCharacterPreDied.RemoveAll(this);
		if (CharBase->GetController())
		{
			GameplayTagListenTask->EndTask();
			GameplayTagListenTask->OnTagAdded.RemoveAll(this);

			GameplayTagListenTask->OnTagRemoved.RemoveAll(this);
			AttributeChangeListener->EndTask();
			RollListenTask->EndTask();
			CastListenTask->EndTask();
			UE_LOG(LogTemp, Warning, TEXT(" UAIBaseState::OnExitState, end task"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" UAIBaseState::OnExitState2, no controller"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAIBaseState::OnExitState2, no char base"));
	}
	bActive = false;
}

void UAIBaseState::OnRollAdded(FGameplayTag MyTag)
{
}

void UAIBaseState::OnRollEnded(FGameplayTag MyTag)
{
}

void UAIBaseState::OnCastStart(FGameplayTag MyTag)
{
	UE_LOG(LogTemp, Warning, TEXT(" UAIBaseState::OnCastStart:%s"), *MyTag.GetTagName().ToString());
	TransitionState(EAIState::EAS_Cast);
}

void UAIBaseState::OnTagAdded(FGameplayTag MyTag)
{
	UE_LOG(LogTemp, Warning, TEXT(" UAIBaseState::OnTagAdded:%s"), *MyTag.GetTagName().ToString());
	TransitionState(EAIState::EAS_Stun);
}

void UAIBaseState::OnTagRemoved(FGameplayTag MyTag)
{
}

void UAIBaseState::Setup(UAIStateHandlerComponent* MyHandlerToSet)
{
	if (!MyHandlerToSet) { return; }
	MyHandler = MyHandlerToSet;
	MyGSController = Cast<AGSHeroAIController>(MyHandlerToSet->GetOwner());
	if (MyGSController)
	{
		auto AbilitySysComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MyGSController->GetPawn());
		GSAbilityComp = Cast<UGSAbilitySystemComponent>(AbilitySysComp);
		MyPawn = MyGSController->GetPawn();
	}

	MyPlayerController = Cast<AGSPlayerController>(MyHandlerToSet->GetOwner());
	if (MyPlayerController)
	{
		auto AbilitySysComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MyPlayerController->GetPawn());
		GSAbilityComp = Cast<UGSAbilitySystemComponent>(AbilitySysComp);
		MyPawn = MyPlayerController->GetPawn();
	}
	DisableMovementTag.Add(FGameplayTag::RequestGameplayTag("Status.Getup"));
	DisableMovementTag.Add(FGameplayTag::RequestGameplayTag("Status.Liftup"));
	DisableMovementTag.Add(FGameplayTag::RequestGameplayTag("Status.Parried"));
	DisableMovementTag.Add(FGameplayTag::RequestGameplayTag("Status.Stagger"));
}

void UAIBaseState::TransitionState(EAIState NewState)
{
	if (!MyHandler) { return; }
	if (bPendingKill) { return; }
	MyHandler->TransitionState(NewState);
}

void UAIBaseState::OnDamageStackChange(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount)
{
	/*
	if (MyState == EAIState::EAS_Attack) { return; }
	if (NewStackCount>=DodgeDamageCount )
	{
		TransitionState(EAIState::EAS_Dodge);
	}
	*/
}

bool UAIBaseState::bCheckValidAttackCondition()
{
	if (!MyGSController) { return false; }
	return MyGSController->CheckValidAttackCondition(MyGSController->EnemyDetected);
}

void UAIBaseState::OnDeath(AGSCharacterBase* Died)
{
	UE_LOG(LogTemp, Warning, TEXT("UAIBaseState::OnDeath"));
	if (!MyHandler) { return; }
	MyHandler->TransitionState(EAIState::EAS_Death);
}

void UAIBaseState::OnDestroyed(AActor* Destroyed)
{
	UE_LOG(LogTemp, Warning, TEXT("UAIBaseState::OnDeath"));
	if (!MyHandler) { return; }
	MyHandler->TransitionState(EAIState::EAS_Death);
}

UAIBaseState* UAIBaseState::AttemptToTransition(EAIState NewState)
{
	if (NewState == EAIState::EAS_BowAim)
	{
		TransitionState(EAIState::EAS_BowAim);
		return MyHandler->GetCurrentBaseState();
	}
	if (NewState == EAIState::EAS_SwordCharge)
	{
		TransitionState(EAIState::EAS_SwordCharge);
		return MyHandler->GetCurrentBaseState();
	}
	if (NewState == EAIState::EAS_SlamDown)
	{
		TransitionState(EAIState::EAS_SlamDown);
		return MyHandler->GetCurrentBaseState();
	}
	return nullptr;
}

bool UAIBaseState::GetIsOnTimer()
{
	if (CharBase)
	{
		return CharBase->GetWorldTimerManager().IsTimerActive(StateCDHandle);
	}
	return false;
}

void UAIBaseState::OnStaminaChange(FGameplayAttribute Attribute, float NewValue, float OldValue)
{

}

void UAIBaseState::OnStartListen()
{
	if (MyGSController)
	{
		CharBase = Cast<AGSCharacterBase>(MyGSController->GetPawn());
	}
}

void UAIBaseState::OnStopListen()
{

}