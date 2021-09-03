// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Commons.h"
#include "AIStateHandlerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeState, EAIState, State);
class UAIBaseState;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSHOOTER_API UAIStateHandlerComponent : public UActorComponent
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly)
	UAIBaseState* CurrentBaseState;
	UPROPERTY(EditDefaultsOnly)
	UDataTable* AIStateTable;
	UPROPERTY()
	TMap< EAIState, UAIBaseState*> StateMapCollection;
public:	
	// Sets default values for this component's properties

	UAIStateHandlerComponent();
	FChangeState ChangeStateDelegate;
	UAIBaseState* GetCurrentBaseState() const
	{
		return CurrentBaseState;
	}
	UFUNCTION(BlueprintCallable)
	ESwordChargeState GetSwordChargeState();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void ConstructStates();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void TransitionState(EAIState NewState);
	UFUNCTION(BlueprintCallable)
	UAIBaseState* AttemptToTransition(EAIState NewState);
	UFUNCTION(BlueprintCallable)
	ESlamDownState GetSlamDownState();
	//should be mainly for AI
	bool CheckStateCD(EAIState NewState);
	//call during end play
	void DestroyAllStates();
};
