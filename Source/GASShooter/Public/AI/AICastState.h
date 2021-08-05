// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIBaseState.h"
#include "AICastState.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UAIPreCastState : public UAIBaseState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<float, TSubclassOf<UGameplayAbility>>AbilityDistanceMap;
	UPROPERTY(EditDefaultsOnly)
	float CastDelay = 1.5;
	virtual void OnEnterState() override;
	UFUNCTION()
	void CastSpell();
	FTimerHandle CastSpellDelayHandle;
	UFUNCTION()
		void OnStartCastCountdown();

	virtual void OnExitState() override;
};
