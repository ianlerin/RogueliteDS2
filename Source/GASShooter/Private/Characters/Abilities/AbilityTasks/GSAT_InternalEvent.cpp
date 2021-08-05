// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AbilityTasks/GSAT_InternalEvent.h"


UGSAT_InternalEvent* UGSAT_InternalEvent::StartIntervalClockEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, float TotalDuration, float Interval)
{
	UGSAT_InternalEvent* MyObj = NewAbilityTask<UGSAT_InternalEvent>(OwningAbility, TaskInstanceName);
	if (MyObj)
	{
		MyObj->TotalDuration = TotalDuration;
		MyObj->Interval = Interval;
	}
	return MyObj;
}


void UGSAT_InternalEvent::Activate()
{
	UE_LOG(LogTemp, Warning, TEXT("UGSAT_InternalEvent::Activate"));
	FTimerDelegate GeneralDelegate;
	GeneralDelegate.BindUFunction(this, "OnGeneralTimerEnd");
	FTimerDelegate LoopDelegate;
	LoopDelegate.BindUFunction(this, "OnLoopTimerUpdate");
	if (TotalDuration > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(GeneralHandler, GeneralDelegate, TotalDuration, false);
	}
	if (Interval > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(LoopHandler, LoopDelegate, Interval, true);
	}
}

void UGSAT_InternalEvent::OnGeneralTimerEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("UGSAT_InternalEvent::OnGeneralTimerEnd"));
	GetWorld()->GetTimerManager().ClearTimer(LoopHandler);
	GeneralUpdateDelegate.Broadcast();
}

void UGSAT_InternalEvent::OnLoopTimerUpdate()
{
	UE_LOG(LogTemp, Warning, TEXT("UGSAT_InternalEvent::OnLoopTimerUpdate"));
	IntervalUpdateDelegate.Broadcast();
}