// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Engine/PostProcessVolume.h"
#include "GameplayAbilities/Public/GameplayEffectTypes.h"
#include "Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "GlobalPostProcessVolume.generated.h"

class UAbilitySystemComponent;
class UPostProcessComponent;
/**
 * 
 */
UCLASS(Blueprintable)
class GASSHOOTER_API AGlobalPostProcessVolume : public AActor
{
	GENERATED_BODY()
public:
	AGlobalPostProcessVolume();
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		UPostProcessComponent* MyPostProcessComponent;

	UPROPERTY()
	UAbilitySystemComponent* AbilityComp;
	
	virtual void BeginPlay() override;
	FTimerHandle DilationTimeLoopHandler;
	FTimerHandle DilationTimeGeneralHandler;
	float DilationTotalDuration = 0;
	float StartDilationSaturation = 0;
	UPROPERTY()
	class UAsyncTaskGameplayTagAddedRemoved*  GameplayTagListenTask;
	UPROPERTY()
		class UAsyncTaskGameplayEffectChange* GameplayEffectListenTask;
	
	UFUNCTION()
	void OnDilationEffectAddedCallback(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount);
	
	UFUNCTION()
	void OnTimeDilateState(bool bTimeDilated);
	UFUNCTION()
	void OnDilationAdded(FGameplayTag MyTag);
	UFUNCTION()
	void OnDilationRemoved();
	UFUNCTION()
	void EndUpdateDilation();
	UFUNCTION()
	void UpdateDilation();

	
	bool bDilationActive = false;
	
	
};
