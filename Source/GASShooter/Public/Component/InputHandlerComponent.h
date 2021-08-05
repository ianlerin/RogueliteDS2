// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GASShooter/GASShooter.h"
#include "Components/ActorComponent.h"
#include "InputHandlerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionRegister, EGSAbilityInputID, AbilityID, bool, bPressed);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSHOOTER_API UInputHandlerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	FOnActionRegister OnActionPressedDelegate;
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<EAdditionalHelperInput> AdditionalInput;
public:	
	// Sets default values for this component's properties
	UInputHandlerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Registerkey(EAdditionalHelperInput InputID, bool bPressed);
	void RegisterAction(EGSAbilityInputID AdditionalInputKey, bool bPressed);
	const TArray<EAdditionalHelperInput>&GetPressedInput()
	{
		return AdditionalInput;
	}

};
