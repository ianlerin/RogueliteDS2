// Copyright 2020 Dan Kestranek.


#include "Characters/Component/WeaponStackListener.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "AbilitySystemGlobals.h"
#include "Runtime/Engine/Classes/Kismet/KismetMaterialLibrary.h"
#include "Characters/Abilities/AsyncTaskGameplayEffectChange.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
UWeaponStackListener::UWeaponStackListener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponStackListener::BeginPlay()
{
	Super::BeginPlay();

	UAbilitySystemComponent* AbilityComp = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
	if (AbilityComp)
	{
		RisingWindHandler = UAsyncTaskGameplayEffectChange::ListenForGameplayEffectStackChange(AbilityComp, FGameplayTag::RequestGameplayTag("Weapon.Bow.IncreaseSpeed"));
		if (RisingWindHandler)
		{
			UE_LOG(LogTemp, Warning, TEXT("UElementStackListener::BeginPlay, setup listener"));
			RisingWindHandler->OnGameplayEffectStackChange.AddDynamic(this, &UWeaponStackListener::OnRisingWindStack);
		}
	}
	// ...
	
}


// Called every frame
void UWeaponStackListener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponStackListener::OnRisingWindStack(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount)
{
	AGSCharacterBase* CharBase = Cast<AGSCharacterBase>(GetOwner());
	if (CharBase)
	{
		CharBase->RecalculateSlowRatio();
	}
}

