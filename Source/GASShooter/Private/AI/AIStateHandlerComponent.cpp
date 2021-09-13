// Copyright 2020 Dan Kestranek.


#include "AI/AIStateHandlerComponent.h"
#include "AI/UAIIdleState.h"
#include "AI/SlamDownState.h"
#include "AI/SwordChargeState.h"
#include "AI/AIBaseState.h"

// Sets default values for this component's properties
UAIStateHandlerComponent::UAIStateHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAIStateHandlerComponent::BeginPlay()
{
	Super::BeginPlay();
	ConstructStates();
	TransitionState(EAIState::EAS_Idle);
	// ...
	
}

void UAIStateHandlerComponent::ConstructStates()
{
	AController* MyController = Cast<AController>(GetOwner());
	if (MyController&&AIStateTable)
	{
		APawn* MyPawn = Cast<APawn>(MyController->GetPawn());
	
		AIStateTable->ForeachRow< FAIStateTable>("State", [=](FName Key, FAIStateTable StateTableInfo) {
		if (StateTableInfo.ActorType==MyPawn->GetClass())
		{
			for (auto StateInfo : StateTableInfo.StateMap)
			{
				if (StateInfo.Value.Get())
				{
					auto CreatedObj = NewObject<UAIBaseState>(GetOwner(), StateInfo.Value);
					if (CreatedObj)
					{
						UE_LOG(LogTemp, Warning, TEXT("UAIStateHandlerComponent::ConstructStates:%s"), *CreatedObj->GetName());
						UUAIIdleState* IdleState = Cast< UUAIIdleState>(CreatedObj);
						CreatedObj->Setup(this);
						StateMapCollection.Add(StateInfo.Key, CreatedObj);
						//CreatedObj->Setup(this);

					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("UAIStateHandlerComponent::ConstructStates fail!"));
				}
			}
		}
	});
	}
}

void UAIStateHandlerComponent::DestroyAllStates()
{
	UE_LOG(LogTemp, Warning, TEXT("UAIStateHandlerComponent::DestroyAllStates"));
	for (auto StateInfo : StateMapCollection)
	{
		StateInfo.Value->SetbPendingKill(true);
		StateInfo.Value->ConditionalBeginDestroy();

		
	}
}

// Called every frame
void UAIStateHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAIStateHandlerComponent::TransitionState(EAIState NewState)
{
	if (NewState == EAIState::EAS_Idle)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::TransitionState, idle"));
	}
	if (NewState == EAIState::EAS_PreAttack)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::TransitionState, pre attack"));
	}
	if (NewState == EAIState::EAS_Attack)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::TransitionState, attack"));
	}
	if (NewState == EAIState::EAS_Follow)
	{
		UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::TransitionState, Follow"));
	}
	
	if (!StateMapCollection.Contains(NewState)) {
		UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::TransitionState no state:%i"), NewState);
		return; 
	}

	if (CurrentBaseState)
	{
		CurrentBaseState->OnExitState();
	}
	CurrentBaseState = StateMapCollection[NewState];
	if (CurrentBaseState)
	{
		CurrentBaseState->OnEnterState();
		OnEnableListener(CurrentBaseState->ListenerState);
		ChangeStateDelegate.Broadcast(NewState);
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::TransitionState no base state"));
	}
}


void UAIStateHandlerComponent::OnEnableListener(TArray<EAIState>ToEnable)
{
	UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::OnEnableListener:%i"),ToEnable.Num());
	for (auto State : ToEnable)
	{
		if (StateMapCollection.Contains(State))
		{
			UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::OnEnableListener:%s"), *StateMapCollection[State]->GetName());
			StateMapCollection[State]->OnStartListen();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::OnEnableListener, no state"));
		}
	}
}

UAIBaseState* UAIStateHandlerComponent::AttemptToTransition(EAIState NewState)
{
	if (CurrentBaseState)
	{
		return CurrentBaseState->AttemptToTransition(NewState);
	}
	return nullptr;
}

ESwordChargeState UAIStateHandlerComponent::GetSwordChargeState()
{
	if (CurrentBaseState)
	{
		USwordChargeState* SwordChargeState = Cast<USwordChargeState>(CurrentBaseState);
		if (SwordChargeState)
		{
			return SwordChargeState->GetChargeState();
		}
	}
	return ESwordChargeState::ECS_None;
}

ESlamDownState UAIStateHandlerComponent::GetSlamDownState()
{
	if (CurrentBaseState)
	{
		USlamDownState* SlamDownState = Cast<USlamDownState>(CurrentBaseState);
		if (SlamDownState)
		{
			return SlamDownState->GetSlamDownState();
		}
	}
	return ESlamDownState::ESDS_None;
}

bool UAIStateHandlerComponent::CheckStateCD(EAIState NewState)
{
	if (StateMapCollection.Contains(NewState)) {
		UE_LOG(LogTemp, Warning, TEXT(" UAIStateHandlerComponent::TransitionState no state:%i"), NewState);
		return StateMapCollection[NewState]->GetIsOnTimer();
	}
	return false;
}