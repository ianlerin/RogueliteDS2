// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponStackListener.generated.h"

class UAsyncTaskGameplayEffectChange;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASSHOOTER_API UWeaponStackListener : public UActorComponent
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UAsyncTaskGameplayEffectChange* RisingWindHandler;

public:	
	// Sets default values for this component's properties
	UWeaponStackListener();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnRisingWindStack(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
