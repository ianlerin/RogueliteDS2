// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "GASShooter/GASShooter.h"
#include "Runtime/PhysicsCore/Public/PhysicalMaterials/PhysicalMaterial.h"
#include "UObject/NoExportTypes.h"
#include "Commons.generated.h"

class UAIBaseState;
class UGameplayEffect;
class UAnimMontage;
class UGameplayAbility;
class AGSWeapon;
class USoundCue;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FRollTable :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGSAbilityInputID PrimaryKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EAdditionalHelperInput AdditionalInput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* MontageToPlay = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EDirection RollDirection;
};

USTRUCT(BlueprintType)
struct FApplyEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bBlockable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Magnitude = 1;
};

USTRUCT(BlueprintType)
struct FComboChainData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName ChainName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<EGSAbilityInputID> InputCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray< EAdditionalHelperInput>AdditionalKeyRequired;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* MontageToPlay = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<EMovementMode> RequiredMovement = EMovementMode::MOVE_Walking;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float DefaultDamage = 10.0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FApplyEffectData> AbilityToApplyToOthers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bBlockable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bParriable = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bBreakBlock = true;
};

USTRUCT(BlueprintType)
struct FHitData :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ActorType;
	//UPROPERTY(EditDefaultsOnly)

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EDirection, UAnimMontage*> DirectionMontageMap;

	//TMap<FComboChainData, UAnimMontage*>MyComboMap;
};

USTRUCT(BlueprintType)
struct FMeleeWeaponData:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	EWeaponType Type;
	//UPROPERTY(EditDefaultsOnly)

	UPROPERTY(EditDefaultsOnly)
	TArray< FComboChainData>MyComboCollection;
	//TMap<FComboChainData, UAnimMontage*>MyComboMap;
};

USTRUCT(BlueprintType)
struct FPlayerStat :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AttackRate = 1.0;
};

USTRUCT(BlueprintType)
struct FPlayerTableStat :public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ActorType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FPlayerStat PlayerStat;
};

USTRUCT(BlueprintType)
struct FAIStateTable :public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> ActorType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap< EAIState, TSubclassOf<UAIBaseState>> StateMap;
};

USTRUCT(BlueprintType)
struct FHitSoundData
{

	GENERATED_BODY()

    public:

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
			USoundCue* NormalSound = nullptr;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
			USoundCue* CriticalSound = nullptr;
};

USTRUCT(BlueprintType)
struct FWeaponHitData :public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> WeaponType;
	UPROPERTY(EditDefaultsOnly)
	TMap< TEnumAsByte<EPhysicalSurface>, FHitSoundData> SurfaceSoundMap;
	//
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	//TArray<FMaterialSoundData>SoundDataCollection;

};

USTRUCT(BlueprintType)
struct FResourceRestriction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap< EResourceType, float> ResourceRestriction;
};

USTRUCT(BlueprintType)
struct FCastExtraData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString SpellSpawnSocket;

};

USTRUCT(BlueprintType)
struct FCastAbilityAnimData :public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		TSoftClassPtr<UGameplayAbility>  CastAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TMap<EWeaponType, FCastExtraData>DataCollection;
};



UCLASS()
class GASSHOOTER_API UCommons : public UObject
{
	GENERATED_BODY()
	
};
