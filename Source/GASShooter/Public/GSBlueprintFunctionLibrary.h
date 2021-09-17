// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericTeamAgentInterface.h"	
#include "Characters/Abilities/GSAbilitySystemComponent.h"
#include "Characters/Abilities/GSAbilityTypes.h"
#include "Characters/Abilities/GSGameplayAbility.h"
#include "GASShooter/Public/Commons.h"
#include "Characters/Abilities/GSGameplayEffectTypes.h"
#include "GSBlueprintFunctionLibrary.generated.h"


class USoundCue;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UGSBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	// Returns the player's editor window role - Server (listen host) or Client #
	UFUNCTION(BlueprintCallable)
	static FString GetPlayerEditorWindowRole(UWorld* World);


	/**
	* GameplayAbility
	*/
	UFUNCTION(BlueprintCallable, Category = "Ability")
	static UGSGameplayAbility* GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static UGSGameplayAbility* GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass);

	UFUNCTION(BlueprintCallable, Category = "Ability")
	static bool IsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle);


	/**
	* FGameplayAbilitySpecHandle
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	static bool IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle);


	/**
	* FGSGameplayEffectContainerSpec
	*/

	// USTRUCTs cannot contain UFUNCTIONS so we make static functions here
	// Checks if spec has any effects
	UFUNCTION(BlueprintPure, Category = "Ability|Container")
	static bool DoesEffectContainerSpecHaveEffects(const FGSGameplayEffectContainerSpec& ContainerSpec);

	// Checks if spec has any targets
	UFUNCTION(BlueprintPure, Category = "Ability|Container")
	static bool DoesEffectContainerSpecHaveTargets(const FGSGameplayEffectContainerSpec& ContainerSpec);

	// Clears spec's targets
	UFUNCTION(BlueprintCallable, Category = "Ability|Container")
	static void ClearEffectContainerSpecTargets(UPARAM(ref) FGSGameplayEffectContainerSpec& ContainerSpec);

	// Adds targets to a copy of the passed in effect container spec and returns it
	UFUNCTION(BlueprintCallable, Category = "Ability|Container", Meta = (AutoCreateRefTerm = "TargetData, HitResults, TargetActors"))
	static void AddTargetsToEffectContainerSpec(UPARAM(ref) FGSGameplayEffectContainerSpec& ContainerSpec, const TArray<FGameplayAbilityTargetDataHandle>& TargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);

	// Applies container spec that was made from an ability
	UFUNCTION(BlueprintCallable, Category = "Ability|Container")
	static TArray<FActiveGameplayEffectHandle> ApplyExternalEffectContainerSpec(const FGSGameplayEffectContainerSpec& ContainerSpec);


	/**
	* FGSGameplayEffectContext
	*/

	// Returns TargetData
	UFUNCTION(BlueprintCallable, Category = "Ability|EffectContext", Meta = (DisplayName = "GetTargetData"))
	static FGameplayAbilityTargetDataHandle EffectContextGetTargetData(FGameplayEffectContextHandle EffectContext);

	// Adds TargetData
	UFUNCTION(BlueprintCallable, Category = "Ability|EffectContext", Meta = (DisplayName = "AddTargetData"))
	static void EffectContextAddTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetData);

	UFUNCTION(BlueprintCallable)
	static FComboChainData InitialFindNextAnimToPlay(UDataTable* Table, EWeaponType WeaponType, TArray< EGSAbilityInputID>InputCollection, AActor* AvatarActor, UPARAM(Ref)TArray <FComboChainData>&FutureChainData);

	UFUNCTION(BlueprintCallable)
	static FComboChainData FindNextAnimToPlay(TArray< FComboChainData>PossibleChainCombo, EWeaponType WeaponType, TArray< EGSAbilityInputID>InputCollection, AActor* AvatarActor, UPARAM(Ref)TArray <FComboChainData>&FutureChainData);
	static void CompareComboChain(TArray< FComboChainData> DTComboChain, TArray< EGSAbilityInputID>InputCollection, TArray<FComboChainData>&MatchingComboChain, TArray < FComboChainData>&FutureChainData);

	UFUNCTION(BlueprintCallable)
	static FGameplayTag GetRollDirection(UDataTable* RollTable,EGSAbilityInputID PrimaryRollKey, AActor* AvatarActor);

	UFUNCTION(BlueprintCallable)
	static EDirection ConvertRotationToDirection(FRotator Rot);

	UFUNCTION(BlueprintCallable)
	static float AdditionalYawToRotateToFaceLocation(AActor* ActorToRotate, FVector LocToFace, bool bAllPositive);
	/**
	* FGameplayAbilityTargetDataHandle
	*/
	UFUNCTION(BlueprintCallable, Category = "Ability|TargetData")
	static void ClearTargetData(UPARAM(ref) FGameplayAbilityTargetDataHandle& TargetData);

	UFUNCTION(BlueprintCallable, Category = "Ability|TargetData")
	static FGameplayAbilityTargetDataHandle MakeGameplayAbilityTargetDataHandleFromHitResults(const TArray<FHitResult> HitResults);

	UFUNCTION(BlueprintCallable, Category = "Team")
	static ETeamAttitude::Type GetAttitudeBetweenActors(AActor* A, AActor* B);

	UFUNCTION(BlueprintCallable, Category = "Trace")
		static TArray<FHitResult> ArcLineTargetForObjects_Front(AActor* MyActor, FVector StartLoc, float ForwardRange, int SmashArc, int IncrementArc, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTraceFor, TArray<AActor*>ActorsToIgnore);

	UFUNCTION(BlueprintCallable, Category = "Sound")
		static USoundCue* GetHitSoundCue(UDataTable* WeaponHitTable, AActor* SourceActor, UPhysicalMaterial* HitMat, bool bIsCritical);

	UFUNCTION(BlueprintCallable, Category = "UI")
		static FText GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero);

	UFUNCTION(BlueprintCallable, Category = "UI")
	static EDirection ConvertDirectionTagToDirection(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category = "Calc")
	static FVector GetVelocityToReachTarget(FVector Start, FVector End, FVector Acceleration, float T);

	UFUNCTION(BlueprintCallable, Category = "UI")
	static FCastExtraData GetCasterMontage(TSubclassOf<UGameplayAbility> Class, UDataTable* Table, AGSWeapon* CurrentWeapon);
};
