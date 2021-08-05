// Copyright 2020 Dan Kestranek.


#include "GlobalPostProcessVolume.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Characters/Abilities/AsyncTaskGameplayEffectChange.h"
#include "Runtime/Engine/Classes/Components/PostProcessComponent.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Abilities/AsyncTaskGameplayTagAddedRemoved.h"

AGlobalPostProcessVolume::AGlobalPostProcessVolume()
{
	MyPostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(FName("MyPostProcessComponent"));
}


void AGlobalPostProcessVolume::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("AGlobalPostProcessVolume::BeginPlay"));
	Super::BeginPlay();
	FGameplayTagContainer MyContainer;
	MyContainer.AddTag(FGameplayTag::RequestGameplayTag("Weapon.HeadShotSlow"));
	APawn* MyPawn=UGameplayStatics::GetPlayerPawn(this, 0);
	AbilityComp=UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MyPawn);
	GameplayTagListenTask = UAsyncTaskGameplayTagAddedRemoved::ListenForGameplayTagAddedOrRemoved(AbilityComp, MyContainer);
	//GameplayTagListenTask->OnTagAdded.AddDynamic(this, &AGlobalPostProcessVolume::OnDilationAdded);
	//GameplayTagListenTask->OnTagRemoved.AddDynamic(this, &AGlobalPostProcessVolume::OnDilationRemoved);
	GameplayEffectListenTask = UAsyncTaskGameplayEffectChange::ListenForGameplayEffectStackChange(AbilityComp, FGameplayTag::RequestGameplayTag("Weapon.HeadShotSlow"));
	//GameplayEffectListenTask->OnGameplayEffectStackChange.AddDynamic(this, &AGlobalPostProcessVolume::OnDilationEffectAddedCallback);
}

void AGlobalPostProcessVolume::OnDilationEffectAddedCallback(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount)
{
	if (NewStackCount >0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGlobalPostProcessVolume::OnDilationEffectAddedCallback:%i"), NewStackCount);
		MyPostProcessComponent->Settings.ColorSaturation = FVector4(0.3, 0.3, 0.3, 0.3);
		DilationTotalDuration = UAbilitySystemBlueprintLibrary::GetActiveGameplayEffectTotalDuration(Handle);
		StartDilationSaturation = MyPostProcessComponent->Settings.ColorSaturation.X;
		FTimerDelegate DilationLoopDelegate;
		FTimerDelegate DilationGeneralDelegate;
		DilationLoopDelegate.BindUFunction(this, "UpdateDilation");
		DilationGeneralDelegate.BindUFunction(this, "EndUpdateDilation");
		GetWorldTimerManager().SetTimer(DilationTimeGeneralHandler, DilationGeneralDelegate, DilationTotalDuration, false);

		GetWorldTimerManager().SetTimer(DilationTimeLoopHandler, DilationLoopDelegate, 0.1, true);
	}

}

void AGlobalPostProcessVolume::OnTimeDilateState(bool bTimeDilated)
{
	if (bTimeDilated)
	{
		MyPostProcessComponent->Settings.ColorSaturation = FVector4(0.3, 0.3, 0.3, 0.3);
	}
	else
	{
		MyPostProcessComponent->Settings.ColorSaturation = FVector4(1, 1, 1, 1);
	}
}

void AGlobalPostProcessVolume::OnDilationRemoved()
{
	UE_LOG(LogTemp, Warning, TEXT("AGlobalPostProcessVolume::OnDilationRemoved"));
	OnTimeDilateState(false);
}

void AGlobalPostProcessVolume::OnDilationAdded(FGameplayTag MyTag)
{
	UE_LOG(LogTemp, Warning, TEXT("AGlobalPostProcessVolume::OnDilationAdded"));
	OnTimeDilateState(true);
}



void AGlobalPostProcessVolume::UpdateDilation()
{
	float TimeRemain = GetWorldTimerManager().GetTimerElapsed(DilationTimeGeneralHandler);
	float Saturation=UKismetMathLibrary::Lerp(0.3, 1, TimeRemain / DilationTotalDuration);

	UE_LOG(LogTemp, Warning, TEXT("AGlobalPostProcessVolume::UpdateDilation:%f"), Saturation);
	MyPostProcessComponent->Settings.ColorSaturation = FVector4(Saturation, Saturation, Saturation, Saturation);
}

void AGlobalPostProcessVolume::EndUpdateDilation()
{
	UE_LOG(LogTemp, Warning, TEXT("AGlobalPostProcessVolume::EndUpdateDilation"));
	GetWorldTimerManager().ClearTimer(DilationTimeLoopHandler);
}
