// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ElementStackListener.generated.h"

class UAsyncTaskGameplayEffectChange;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSHOOTER_API UElementStackListener : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UAsyncTaskGameplayEffectChange* ChillHandler;
	UPROPERTY(EditDefaultsOnly,category="chill")
	float ChillMinFresnel = 10.0;
	UPROPERTY(EditDefaultsOnly, category = "chill")
	float ChillMaxFresnel = 3.0;
	UPROPERTY(EditDefaultsOnly, category = "chill")
	int32 ChillMaxStack = 20;

	UPROPERTY(EditDefaultsOnly, category = "chill")
	TSubclassOf<UGameplayEffect>ChillEffect;

	UPROPERTY(EditDefaultsOnly, category = "Burn")
	TSubclassOf<UGameplayAbility>DeepBurnAbility;

	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> CharacterBodyMatArray;
public:	

	// Sets default values for this component's properties
	UElementStackListener();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnChillStack(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount);
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	const 	TSubclassOf<UGameplayEffect> GetChillEffect()
	{
		return ChillEffect;
	}
	const TSubclassOf<UGameplayAbility> GetDeepBurnAbility()
	{
		return DeepBurnAbility;
	}

};
