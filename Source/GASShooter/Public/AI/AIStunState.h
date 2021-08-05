// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "AIStunState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIStunState : public UAIBaseState
{
	GENERATED_BODY()
protected:
	virtual void OnTagRemoved(FGameplayTag MyTag) override;
	UPROPERTY(EditDefaultsOnly)
	EAIState StateAfterStun = EAIState::EAS_PreAttack;
	
	virtual void OnEnterState() override;
	virtual void OnExitState() override;
};
