// Copyright 2020 Dan Kestranek.


#include "Characters/Component/ElementStackListener.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "AbilitySystemGlobals.h"
#include "Characters/GSCharacterBase.h"
#include "Runtime/Engine/Classes/Kismet/KismetMaterialLibrary.h"
#include "Characters/Abilities/AsyncTaskGameplayEffectChange.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "AbilitySystemComponent.h"

// Sets default values for this component's properties
UElementStackListener::UElementStackListener()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UElementStackListener::BeginPlay()
{
	Super::BeginPlay();
	ACharacter* CharOwner = Cast<ACharacter>(GetOwner());
	if (CharOwner)
	{
		int i = 0;
		for (auto Mat : CharOwner->GetMesh()->GetMaterials())
		{

			auto CreatedMat=UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, Mat);
			CharOwner->GetMesh()->SetMaterial(i, CreatedMat);
			CharacterBodyMatArray.Add(CreatedMat);
			i++;
		}
		UAbilitySystemComponent* AbilityComp = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner());
		if (AbilityComp)
		{
			ChillHandler = UAsyncTaskGameplayEffectChange::ListenForGameplayEffectStackChange(AbilityComp, FGameplayTag::RequestGameplayTag("Status.Element.Chill"));
			if (ChillHandler)
			{
				UE_LOG(LogTemp, Warning, TEXT("UElementStackListener::BeginPlay, setup listener"));
				//ChillHandler->OnGameplayEffectStackChange.AddDynamic(this, &UElementStackListener::OnChillStack);
			}
		}
	}


	// ...
	
}


// Called every frame
void UElementStackListener::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UElementStackListener::OnChillStack(FGameplayTag EffectGameplayTag ,FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount)
{
	float Stack = NewStackCount;
	float Max = ChillMaxStack;
	float Alpha = Stack / Max;

	float FrenelToApply=UKismetMathLibrary::Lerp(ChillMinFresnel, ChillMaxFresnel, Alpha);

	UE_LOG(LogTemp, Warning, TEXT("UElementStackListener::Stack:%i,OldStack:%i"), NewStackCount, OldStackCount);
	for (auto Mat : CharacterBodyMatArray)
	{
		Mat->SetScalarParameterValue("FresnelVal", FrenelToApply);
		if (NewStackCount > 0)
		{

			Mat->SetScalarParameterValue("Chill", 1);
		}
		else
		{

			Mat->SetScalarParameterValue("Chill", 0);
		}
	}

	AGSCharacterBase* CharBase = Cast< AGSCharacterBase>(GetOwner());
	if (CharBase)
	{
		CharBase->RecalculateSlowRatio();
	}
}
