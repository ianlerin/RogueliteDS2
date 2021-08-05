// Copyright 2020 Dan Kestranek.


#include "AI/GSHeroAIController.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "Characters/Abilities/AsyncTaskGameplayEffectChange.h"
#include "AI/AIStateHandlerComponent.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "GameplayAbilities/Public/AbilitySystemGlobals.h"
#include "GASShooter/Public/GSBlueprintFunctionLibrary.h"

AGSHeroAIController::AGSHeroAIController()
{
	bWantsPlayerState = true;
	AIStateHandler = CreateDefaultSubobject<UAIStateHandlerComponent>("AIStateHandler");
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComp");
}

void AGSHeroAIController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AGSHeroAIController::BeginPlay"));
	FGenericTeamId::SetAttitudeSolver([](FGenericTeamId ID1, FGenericTeamId ID2) {
		UE_LOG(LogTemp, Warning, TEXT("AGSHeroAIController, solve"));
		if (ID1.GetId() == ID2.GetId())
		{
			UE_LOG(LogTemp, Warning, TEXT("AGSHeroAIController, solve friendly"));
			return ETeamAttitude::Friendly;
		}
		UE_LOG(LogTemp, Warning, TEXT("AGSHeroAIController, solve hostile"));
		return ETeamAttitude::Hostile; 
	}
	);
	SetGenericTeamId(FGenericTeamId(2));
	/*
	auto GSAbilityComp = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetPawn());
	SpawnedDamageEffectListener = UAsyncTaskGameplayEffectChange::ListenForGameplayEffectStackChange(GSAbilityComp, FGameplayTag::RequestGameplayTag("Event.DamageCount"));
	if (SpawnedDamageEffectListener)
	{
		SpawnedDamageEffectListener->OnGameplayEffectStackChange.AddDynamic(this, &AGSHeroAIController::OnDamageStackChange);
	}
	*/
}

void AGSHeroAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TArray<AActor*>PerceivedActors;
	PerceptionComp->GetPerceivedActors(SenseClass, PerceivedActors);
}


void AGSHeroAIController::OnDamageStackChange(FGameplayTag EffectGameplayTag, FActiveGameplayEffectHandle Handle, int32 NewStackCount, int32 OldStackCount)
{
	APlayerState* OppState = Cast<APlayerState>(Handle.GetOwningAbilitySystemComponent()->GetOwner());
	if (OppState)
	{
		SetEnemyDetected(OppState->GetPawn());
		UE_LOG(LogTemp, Warning, TEXT("AGSHeroAIController::OnDamageStackChange,source:%s"), *OppState->GetPawn()->GetName());
	}
}

bool AGSHeroAIController::CheckValidAttackCondition(APawn* MyTarget)
{
	if (!MyTarget||!GetPawn()) { return false; }
	//rotation and movement check
	float Yaw=UGSBlueprintFunctionLibrary::AdditionalYawToRotateToFaceLocation(GetPawn(), MyTarget->GetActorLocation(),false);
	if (FMath::Abs(Yaw) > MinYawDifferenceToAttack) { return false; }
	float Distance = (MyTarget->GetActorLocation() - GetPawn()->GetActorLocation()).Size();
	if (Distance > MinAttackDistance) { return false; }
	return true;
}

void AGSHeroAIController::SetEnemyDetected(APawn* Enemy)
{
	EnemyDetected = Enemy;
	OnEnemySetDelegate.Broadcast(Enemy);
}

bool AGSHeroAIController::CheckIfEnemyInSight()
{
	TArray<AActor*>PerceivedActors;
	PerceptionComp->GetPerceivedActors(SenseClass, PerceivedActors);
	return PerceivedActors.Find(EnemyDetected) >= 0;
}

/*
ETeamAttitude::Type AGSHeroAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	auto TeamInterface = Cast<IGenericTeamAgentInterface>(&Other);
	if (!TeamInterface) { return ETeamAttitude::Neutral; }
	if (TeamInterface->GetGenericTeamId().GetId() == GetGenericTeamId().GetId())
	{
		return ETeamAttitude::Friendly;
	}
	return ETeamAttitude::Hostile;
}
*/