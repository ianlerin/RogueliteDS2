// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GASShooter/GASShooter.h"
#include "BowStateTransitionNotify.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API UBowStateTransitionNotify : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EBowDrawState BowAimState;
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
