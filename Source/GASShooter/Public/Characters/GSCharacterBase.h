// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Data/AnimDataAsset.h"
#include "GASShooter/GASShooter.h"
#include "TimerManager.h"
#include "GSCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, AGSCharacterBase*, Character);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceChange);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterPreDiedDelegate, AGSCharacterBase*, Character);
USTRUCT(BlueprintType)
struct GASSHOOTER_API FGSDamageNumber
{
	GENERATED_USTRUCT_BODY()

	float DamageAmount;

	FGameplayTagContainer Tags;

	FGSDamageNumber() {}

	FGSDamageNumber(float InDamageAmount, FGameplayTagContainer InTags) : DamageAmount(InDamageAmount)
	{
		// Copy tag container
		Tags.AppendTags(InTags);
	}
};
class UGameplayAbility;
class UGSCharacterMovementComponent;
/**
* The base Character class for the game. Everything with an AbilitySystemComponent in this game will inherit from this class.
* This class should not be instantiated and instead subclassed.
*/
UCLASS()
class GASSHOOTER_API AGSCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	
public:
	// Sets default values for this character's properties
	AGSCharacterBase(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable, Category = "GASShooter|GSCharacter")
	FCharacterDiedDelegate OnCharacterDied;

	FOnResourceChange OnStaminaChangeDelegate;

	FOnResourceChange OnHealthChangeDelegate;

	FOnResourceChange OnManaChangeDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GASShooter|GSCharacter")
	FCharacterPreDiedDelegate OnCharacterPreDied;
	// Implement IAbilitySystemInterface
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter")
	virtual bool IsAlive() const;

	// Switch on AbilityID to return individual ability levels.
	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter")
	virtual int32 GetAbilityLevel(EGSAbilityInputID AbilityID) const;

	// Removes all CharacterAbilities. Can only be called by the Server. Removing on the Server will remove from Client too.
	virtual void RemoveCharacterAbilities();

	UFUNCTION(BlueprintCallable)
		virtual void Die();

	void PreFinishDying();

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter")
	virtual void FinishDying();

	virtual void AddDamageNumber(float Damage, FGameplayTagContainer DamageNumberTags);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* MyStencilComponent;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UElementStackListener* ElementStackListener;
	/**
	* Getters for attributes from GSAttributeSetBase
	**/

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	int32 GetCharacterLevel() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetRestrictedHealth() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetMana() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetMaxMana() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetRestrictedMana() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetRestrictedStamina() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetShield() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetMaxShield() const;

	// Gets the Current value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetMoveSpeed() const;

	// Gets the Base value of MoveSpeed
	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetMoveSpeedBaseValue() const;

	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetBowDrawRate() const;
	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetMeleeAttackRate() const;

	float GetMaxMeleeAttackRate() const;
	UFUNCTION(BlueprintCallable)
	void SetBowDrawRate(float ToSet);
	void SetMeleeAttackRate(float ToSet);
	float GetMaxHealth();

	void SetMagicAttackRate(float MagicAttack);
	UFUNCTION(BlueprintCallable, Category = "GASShooter|GSCharacter|Attributes")
	float GetMagicAttackRate() const;
	float GetBaseMagicAttackRate()const;
	float GetBaseMeleeAttackRate() const;
	float GetBaseBowDrawRate() const;
	void RecalculateSlowRatio();
	void OnFocus(bool bFocus);
protected:
	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	TArray<FGSDamageNumber> DamageNumberQueue;
	FTimerHandle DamageNumberTimer;
	
	// Reference to the ASC. It will live on the PlayerState or here if the character doesn't have a PlayerState.
	UPROPERTY()
	class UGSAbilitySystemComponent* AbilitySystemComponent;

	// Reference to the AttributeSetBase. It will live on the PlayerState or here if the character doesn't have a PlayerState.
	UPROPERTY()
	class UGSAttributeSetBase* AttributeSetBase;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASShooter|GSCharacter")
	FText CharacterName;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASShooter|Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASShooter|Audio")
	class USoundCue* DeathSound;

	// Default abilities for this Character. These will be removed on Character death and regiven if Character respawns.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASShooter|Abilities")
	TArray<TSubclassOf<class UGSGameplayAbility>> CharacterAbilities;

	// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASShooter|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	// These effects are only applied one time on startup
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASShooter|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASShooter|Abilities")
	TSubclassOf<class UGSGameplayAbility> DeathAbility;

	UPROPERTY(EditAnywhere, Category = "GASShooter|UI")
	TSubclassOf<class UGSDamageTextWidgetComponent> DamageNumberClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimDataAsset* AnimDataAsset;

	UPROPERTY()
	UGSCharacterMovementComponent* GSCharMovement;
	// effects to track
	
	UFUNCTION()
	void OnAIChangeState(EAIState State);
	UPROPERTY(BlueprintReadOnly)
	bool bDisableInput = false;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Grant abilities on the Server. The Ability Specs will be replicated to the owning client.
	virtual void AddCharacterAbilities();

	// Initialize the Character's attributes. Must run on Server but we run it on Client too
	// so that we don't have to wait. The Server's replication to the Client won't matter since
	// the values should be the same.
	virtual void InitializeAttributes();

	virtual void AddStartupEffects();

	virtual void ShowDamageNumber();

	UFUNCTION(BlueprintCallable)
	void SetBowMaxDrawRate(float ToSet);

	UFUNCTION(BlueprintCallable)
	float GetMaxBowDrawRate() const;
	/**
	* Setters for Attributes. Only use these in special cases like Respawning, otherwise use a GE to change Attributes.
	* These change the Attribute's Base Value.
	*/

	virtual void SetHealth(float Health);
	virtual void SetMana(float Mana);
	virtual void SetStamina(float Stamina);
	virtual void SetShield(float Shield);

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect>DrawBowIncreaseEffect;
	float GetBaseDamage();
	float GetBaseDamageSpecfic(EDamageType DamageType);
	UFUNCTION(BlueprintCallable)
	void AddBaseDamageModifier(const UGSGameplayAbility* Ability, float Amount, bool bAdd);
	void OnResourceRestrictionAdd(const UGSGameplayAbility* Ability, EResourceType Type, float Amount);
	 UElementStackListener* GetElementStackListener()const
	{
		return ElementStackListener;
	}
};
