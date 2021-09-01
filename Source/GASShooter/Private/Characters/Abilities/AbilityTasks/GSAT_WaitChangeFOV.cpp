// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AbilityTasks/GSAT_WaitChangeFOV.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveFloat.h"
#include "GASShooter/GASShooter.h"

UGSAT_WaitChangeFOV::UGSAT_WaitChangeFOV(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bTickingTask = true;
	bIsFinished = false;
}

UGSAT_WaitChangeFOV* UGSAT_WaitChangeFOV::WaitChangeFOV(UGameplayAbility* OwningAbility, FName TaskInstanceName, class UCameraComponent* CameraComponent, float TargetFOV, float Duration, UCurveFloat* OptionalInterpolationCurve)
{
	UE_LOG(LogTemp, Warning, TEXT("UGSAT_WaitChangeFOV::WaitChangeFOV,TargetFOV:%f"), TargetFOV);
	UGSAT_WaitChangeFOV* MyObj = NewAbilityTask<UGSAT_WaitChangeFOV>(OwningAbility, TaskInstanceName);

	MyObj->CameraComponent = CameraComponent;
	if (CameraComponent != nullptr)
	{
		MyObj->StartFOV = MyObj->CameraComponent->FieldOfView;
	}

	MyObj->TargetFOV = TargetFOV;
	MyObj->Duration = FMath::Max(Duration, 0.001f);		// Avoid negative or divide-by-zero cases
	MyObj->TimeChangeStarted = MyObj->GetWorld()->GetTimeSeconds();
	MyObj->TimeChangeWillEnd = MyObj->TimeChangeStarted + MyObj->Duration;
	MyObj->LerpCurve = OptionalInterpolationCurve;

	return MyObj;
}

void UGSAT_WaitChangeFOV::Activate()
{
}

void UGSAT_WaitChangeFOV::TickTask(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("UGSAT_WaitChangeFOV::TickTask"));
	if (bIsFinished)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UGSAT_WaitChangeFOV::TickTask,bIsFinished"));
		return;
	}

	Super::TickTask(DeltaTime);

	if (CameraComponent)
	{
		float CurrentTime = GetWorld()->GetTimeSeconds();

		if (CurrentTime >= TimeChangeWillEnd)
		{
			bIsFinished = true;

			//UE_LOG(LogTemp, Warning, TEXT("UGSAT_WaitChangeFOV::TargetFOV:%f"), TargetFOV);
			CameraComponent->SetFieldOfView(TargetFOV);
			
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				//UE_LOG(LogTemp, Warning, TEXT("UGSAT_WaitChangeFOV::OnTargetFOVReached"));
				OnTargetFOVReached.Broadcast();
			}
			EndTask();
		}
		else
		{
			float NewFOV;

			float MoveFraction = (CurrentTime - TimeChangeStarted) / Duration;
			if (LerpCurve)
			{
				MoveFraction = LerpCurve->GetFloatValue(MoveFraction);
			}

			NewFOV = FMath::Lerp<float, float>(StartFOV, TargetFOV, MoveFraction);

			//UE_LOG(LogTemp, Warning, TEXT("UGSAT_WaitChangeFOV::NewFOV:%f"), NewFOV);
			CameraComponent->SetFieldOfView(NewFOV);
		}
	}
	else
	{
		bIsFinished = true;
		EndTask();
	}
}

void UGSAT_WaitChangeFOV::OnDestroy(bool AbilityIsEnding)
{
	Super::OnDestroy(AbilityIsEnding);
}
