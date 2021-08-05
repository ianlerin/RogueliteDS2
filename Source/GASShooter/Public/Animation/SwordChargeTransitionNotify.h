// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GASShooter/GASShooter.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SwordChargeTransitionNotify.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API USwordChargeTransitionNotify : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ESwordChargeState SwordChargeState;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
