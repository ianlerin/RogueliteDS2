// Copyright 2020 Dan Kestranek.


#include "Weapons/GSProjectile.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGSProjectile::AGSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->InitialSpeed = 7000.0f;

	bReplicates = true;

	//TODO change this to a better value
	NetUpdateFrequency = 100.0f;

	SphereComp = CreateDefaultSubobject<USphereComponent>(FName("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AGSProjectile::OnSphereOverlap);
}

void AGSProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AGSProjectile::OnSphereOverlap:%s"),*OtherActor->GetName());
	TArray<AActor*>ActorsToIgnore;
	if (GetOwner())
	{

		UE_LOG(LogTemp, Warning, TEXT("AGSProjectile::Owner:%s"), *GetOwner()->GetName());
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("AGSProjectile::No Owner"));
	}
	if (OtherActor == GetOwner()) { return; }
	ActorsToIgnore.Add(GetOwner());
	TArray<FHitResult> OutHits;
	bool bHit=UKismetSystemLibrary::SphereTraceMulti(this, GetActorLocation(), GetActorLocation(), ExplodeRadius, ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, OutHits,true);
	for (auto Hit : OutHits)
	{
		OnProjectileExplodeHitDelegate.Broadcast(Hit);
		if (!bMultipleHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("AGSProjectile::OnSphereOverlap: Destroy, Hit:%s"),*Hit.GetActor()->GetName());
			Destroy();
			return;
		}
		
	}
	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGSProjectile::OnSphereOverlap: Destroy"));
		Destroy();
	}
}