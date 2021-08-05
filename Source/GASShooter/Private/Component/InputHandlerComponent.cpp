// Copyright 2020 Dan Kestranek.


#include "Component/InputHandlerComponent.h"

// Sets default values for this component's properties
UInputHandlerComponent::UInputHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInputHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInputHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UInputHandlerComponent::Registerkey(EAdditionalHelperInput AdditionalInputKey, bool bPressed)
{
	if (AdditionalInputKey == EAdditionalHelperInput::DownKey)
	{

		UE_LOG(LogTemp, Warning, TEXT("UInputHandlerComponent::Registerkey,Down"));
	}
	UE_LOG(LogTemp, Warning, TEXT("UInputHandlerComponent::Registerkey"));
	if (bPressed)
	{
		AdditionalInput.Add(AdditionalInputKey);
	}
	else
	{

		AdditionalInput.Remove(AdditionalInputKey);
	}
}

void UInputHandlerComponent::RegisterAction(EGSAbilityInputID AdditionalInputKey, bool bPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("UInputHandlerComponent::RegisterAction"));
	OnActionPressedDelegate.Broadcast(AdditionalInputKey, bPressed);
}