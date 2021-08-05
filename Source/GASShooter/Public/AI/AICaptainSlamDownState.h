// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/SlamDownState.h"
#include "AICaptainSlamDownState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAICaptainSlamDownState : public USlamDownState
{
	GENERATED_BODY()

protected:


	virtual void OnEnterState() override;
};
