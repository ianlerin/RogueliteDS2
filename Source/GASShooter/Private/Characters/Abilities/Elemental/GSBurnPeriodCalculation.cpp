// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/Elemental/GSBurnPeriodCalculation.h"
#include "GSBlueprintFunctionLibrary.h"
#include "GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Characters/Abilities/Elemental/LongBurnAbility.h"

float UGSBurnPeriodCalculation::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec & Spec) const
{
	if (Spec.GetEffectContext().GetSourceObject())
	{
		UE_LOG(LogTemp, Warning, TEXT("UGSBurnPeriodCalculation::CalculateBaseMagnitude:%s"), *Spec.GetEffectContext().GetSourceObject()->GetName());
	}
	ACharacter* Char = Cast<ACharacter>(Spec.GetEffectContext().GetSourceObject());
	UAbilitySystemComponent* AbilitySystemComp = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Char);
	float Increment = 0;
	if (AbilitySystemComp)
	{
		auto Ability=UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(AbilitySystemComp, LongBurnAbility);
		ULongBurnAbility* LongBurn = Cast<ULongBurnAbility>(Ability);
	
		if (LongBurn)
		{
			Increment = LongBurn->GetBurnExtendDuration();
			UE_LOG(LogTemp, Warning, TEXT("UGSBurnPeriodCalculation::LongBurn:%f"), Increment);
			
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("UGSBurnPeriodCalculation::No LongBurn"));
		}
	}
	return Increment+ BaseBurnDuration;
}