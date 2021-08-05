// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "AIController.h"
#include "GSHeroAIController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemySet, APawn*, MyEnemy);

/**
 * 
 */
class UAISense;
class APawn;
class UAIPerceptionComponent;
class UAIStateHandlerComponent;
UCLASS()
class GASSHOOTER_API AGSHeroAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AGSHeroAIController();
	UFUNCTION(BlueprintCallable)
	bool CheckValidAttackCondition(APawn* MyTarget);
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	//set by perception in bp
	UPROPERTY(BlueprintReadWrite)
	APawn* EnemyDetected = nullptr;
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAIStateHandlerComponent* AIStateHandler;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* PerceptionComp;
	UFUNCTION(BlueprintCallable)
	void SetEnemyDetected(APawn* Enemy);
	FOnEnemySet OnEnemySetDelegate;

	bool CheckIfEnemyInSight();
protected:
	UFUNCTION()
		void OnDamageStackChange(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount);
	UPROPERTY()
	class UAsyncTaskGameplayEffectChange* SpawnedDamageEffectListener;
	UPROPERTY(EditDefaultsOnly)
	float MinYawDifferenceToAttack = 30;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAISense>SenseClass;
	UPROPERTY(EditDefaultsOnly)
	float MinAttackDistance = 200;
	//virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	//ETeamAttitude::Type SolveAttitude(FGenericTeamID ID1, FGenericTeamID ID2);
};
