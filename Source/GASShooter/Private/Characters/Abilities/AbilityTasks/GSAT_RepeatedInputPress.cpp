// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AbilityTasks/GSAT_RepeatedInputPress.h"
#include "AbilitySystemComponent.h"

UGSAT_RepeatedInputPress::UGSAT_RepeatedInputPress(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	StartTime = 0.f;
	bTestInitialState = false;
}

void UGSAT_RepeatedInputPress::OnPressCallback()
{
	float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;

	if (!Ability || !AbilitySystemComponent)
	{
		return;
	}

	//AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey()).Remove(DelegateHandle);

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent, IsPredictingClient());

	if (IsPredictingClient())
	{
		// Tell the server about this
		AbilitySystemComponent->ServerSetReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey(), AbilitySystemComponent->ScopedPredictionKey);
	}
	else
	{
		AbilitySystemComponent->ConsumeGenericReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey());
	}

	// We are done. Kill us so we don't keep getting broadcast messages
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnPress.Broadcast(ElapsedTime);
	}
	//EndTask();
}

UGSAT_RepeatedInputPress* UGSAT_RepeatedInputPress::RepeatedInputPress(class UGameplayAbility* OwningAbility, bool bTestAlreadyPressed)
{
	UGSAT_RepeatedInputPress* Task = NewAbilityTask<UGSAT_RepeatedInputPress>(OwningAbility);
	Task->bTestInitialState = bTestAlreadyPressed;
	return Task;
}

void UGSAT_RepeatedInputPress::Activate()
{
	StartTime = GetWorld()->GetTimeSeconds();
	if (Ability)
	{
		if (bTestInitialState && IsLocallyControlled())
		{
			FGameplayAbilitySpec *Spec = Ability->GetCurrentAbilitySpec();
			if (Spec && Spec->InputPressed)
			{
				OnPressCallback();
				return;
			}
		}

		DelegateHandle = AbilitySystemComponent->AbilityReplicatedEventDelegate(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(this, &UGSAT_RepeatedInputPress::OnPressCallback);
		if (IsForRemoteClient())
		{
			if (!AbilitySystemComponent->CallReplicatedEventDelegateIfSet(EAbilityGenericReplicatedEvent::InputPressed, GetAbilitySpecHandle(), GetActivationPredictionKey()))
			{
				SetWaitingOnRemotePlayerData();
			}
		}
	}
}