// Copyright 2020 Dan Kestranek.


#include "Characters/GSCharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "AI/AIStateHandlerComponent.h"
#include "Characters/Abilities/AsyncTaskGameplayTagAddedRemoved.h"
#include "Characters/Abilities/GSAbilitySystemGlobals.h"
#include "Characters/GSCharacterBase.h"
#include "GameplayTagContainer.h"

UGSCharacterMovementComponent::UGSCharacterMovementComponent()
{
	SprintSpeedMultiplier = 1.4f;
	ADSSpeedMultiplier = 0.8f;
	KnockedDownSpeedMultiplier = 0.4f;

	KnockedDownTag = FGameplayTag::RequestGameplayTag("State.KnockedDown");
	InteractingTag = FGameplayTag::RequestGameplayTag("State.Interacting");
	InteractingRemovalTag = FGameplayTag::RequestGameplayTag("State.InteractingRemoval");

}

void UGSCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("UGSCharacterMovementComponent::BeginPlay"));
	AGSCharacterBase* Owner = Cast<AGSCharacterBase>(GetOwner());
	if (Owner)
	{
		if (Owner->GetController())
		{
			UAIStateHandlerComponent* StateHandlerComp=Owner->GetController()->FindComponentByClass<UAIStateHandlerComponent>();
			if (StateHandlerComp)
			{
				UE_LOG(LogTemp, Warning, TEXT("UGSCharacterMovementComponent::BeginPlay,Setupsuccess"));
				StateHandlerComp->ChangeStateDelegate.AddDynamic(this, &UGSCharacterMovementComponent::OnAIChangeState);
		    }
		}
	}
}

void UGSCharacterMovementComponent::OnAIChangeState(EAIState State)
{
	if (State == EAIState::EAS_Stun || State == EAIState::EAS_Death|| State==EAIState::EAS_SlamDown)
	{
		bMovementDisabled = true;

		MaxWalkSpeed = 0;
	}
	else
	{
		bMovementDisabled = false;
		float Speed = GetMaxSpeed();
		MaxWalkSpeed = Speed;
	}
}

float UGSCharacterMovementComponent::GetMaxSpeed() const
{
	AGSCharacterBase* Owner = Cast<AGSCharacterBase>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (!Owner->IsAlive()|| bMovementDisabled)
	{
		return 0.0f;
	}
	
	// Don't move while interacting or being interacted on (revived)
	if (Owner->GetAbilitySystemComponent() && Owner->GetAbilitySystemComponent()->GetTagCount(InteractingTag)
		> Owner->GetAbilitySystemComponent()->GetTagCount(InteractingRemovalTag))
	{
		return 0.0f;
	}

	int ChillStack = Owner->GetAbilitySystemComponent()->GetGameplayEffectCount(ChillEffectClass, Owner->GetAbilitySystemComponent());

	float BaseMS = Owner->GetMoveSpeed();

	if (RequestToStartSprinting)
	{
		BaseMS= BaseMS * SprintSpeedMultiplier;
	}

	BaseMS = BaseMS * (1-ChillStack*0.025);
	/*
	if (RequestToStartADS)
	{
		return Owner->GetMoveSpeed() * ADSSpeedMultiplier;
	}
	*/

	return BaseMS;
}

void UGSCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;

	RequestToStartADS = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

FNetworkPredictionData_Client* UGSCharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UGSCharacterMovementComponent* MutableThis = const_cast<UGSCharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FGSNetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UGSCharacterMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void UGSCharacterMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UGSCharacterMovementComponent::StartAimDownSights()
{
	RequestToStartADS = true;
}

void UGSCharacterMovementComponent::StopAimDownSights()
{
	RequestToStartADS = false;
}

void UGSCharacterMovementComponent::FGSSavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartADS = false;
}

uint8 UGSCharacterMovementComponent::FGSSavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartADS)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UGSCharacterMovementComponent::FGSSavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FGSSavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}

	if (SavedRequestToStartADS != ((FGSSavedMove*)&NewMove)->SavedRequestToStartADS)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UGSCharacterMovementComponent::FGSSavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UGSCharacterMovementComponent* CharacterMovement = Cast<UGSCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartADS = CharacterMovement->RequestToStartADS;
	}
}

void UGSCharacterMovementComponent::FGSSavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UGSCharacterMovementComponent* CharacterMovement = Cast<UGSCharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}

UGSCharacterMovementComponent::FGSNetworkPredictionData_Client::FGSNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement)
	: Super(ClientMovement)
{
}

FSavedMovePtr UGSCharacterMovementComponent::FGSNetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FGSSavedMove());
}

