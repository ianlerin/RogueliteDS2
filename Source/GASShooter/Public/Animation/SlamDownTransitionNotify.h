// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GASShooter/GASShooter.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SlamDownTransitionNotify.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API USlamDownTransitionNotify : public UAnimNotify
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere)
		ESlamDownState SlamDownState;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
