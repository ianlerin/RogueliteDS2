// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GSProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileExplodeHit, FHitResult, Hit);

class USphereComponent;
UCLASS()
class GASSHOOTER_API AGSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGSProjectile();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "PBProjectile")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "PBProjectile")
	class USphereComponent* SphereComp;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UPROPERTY(EditDefaultsOnly)
	EProjectileImpact ProjectileImpactType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bMultipleHit = false;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float ExplodeRadius = 50;
public:
	UPROPERTY(BlueprintAssignable)
	FOnProjectileExplodeHit OnProjectileExplodeHitDelegate;

};
