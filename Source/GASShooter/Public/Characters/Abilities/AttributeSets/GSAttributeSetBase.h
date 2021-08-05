// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GSAttributeSetBase.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
class UGSGameplayAbility;
UCLASS()
class GASSHOOTER_API UGSAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UGSAttributeSetBase();

	// Current Health, when 0 we expect owner to die unless prevented by an ability. Capped by MaxHealth.
	// Positive changes can directly use this.
	// Negative changes to Health should go through Damage meta attribute
	


	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData MaxHealthAfterRestriction;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxHealthAfterRestriction)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData BaseMaxHealth;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseMaxHealth)

	// Health regen rate will passively increase Health every second
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_HealthRegenRate)
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, HealthRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Mana")
	FGameplayAttributeData MaxManaAfterRestriction;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxManaAfterRestriction)

	// Current Mana, used to execute special abilities. Capped by MaxMana.
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Mana)

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	FGameplayAttributeData BaseMaxMana;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseMaxMana)
	// Mana regen rate will passively increase Mana every second
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_ManaRegenRate)
	FGameplayAttributeData ManaRegenRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, ManaRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData MaxStaminaAfterRestriction;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxStaminaAfterRestriction)

	// Current stamina, used to execute special abilities. Capped by MaxStamina.
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina")
	FGameplayAttributeData BaseStamina;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseStamina)
	// Stamina regen rate will passively increase Stamina every second
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_StaminaRegenRate)
	FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, StaminaRegenRate)

	// Current shield acts like temporary health. When depleted, damage will drain regular health.
	UPROPERTY(BlueprintReadOnly, Category = "Shield", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Shield)

	// Maximum shield that we can have.
	UPROPERTY(BlueprintReadOnly, Category = "Shield", ReplicatedUsing = OnRep_MaxShield)
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxShield)

	// Shield regen rate will passively increase Shield every second
	UPROPERTY(BlueprintReadOnly, Category = "Shield", ReplicatedUsing = OnRep_ShieldRegenRate)
	FGameplayAttributeData ShieldRegenRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, ShieldRegenRate)

	// Armor reduces the amount of damage done by attackers
	UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Armor)

	// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Damage)

	// MoveSpeed affects how fast characters can move.
	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Character Level", ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, CharacterLevel)

	// Experience points gained from killing enemies. Used to level up (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "XP", ReplicatedUsing = OnRep_XP)
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, XP)

	// Experience points awarded to the character's killers. Used to level up (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "XP", ReplicatedUsing = OnRep_XPBounty)
	FGameplayAttributeData XPBounty;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, XPBounty)

	// Gold gained from killing enemies. Used to purchase items (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "Gold", ReplicatedUsing = OnRep_Gold)
	FGameplayAttributeData Gold;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Gold)

	// Gold awarded to the character's killer. Used to purchase items (not implemented in this project).
	UPROPERTY(BlueprintReadOnly, Category = "Gold", ReplicatedUsing = OnRep_GoldBounty)
	FGameplayAttributeData GoldBounty;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, GoldBounty)

	//stats
	UPROPERTY(BlueprintReadOnly, Category = "Stat", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Str;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Str)

	UPROPERTY(BlueprintReadOnly, Category = "Stat", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Dex;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Dex)

	UPROPERTY(BlueprintReadOnly, Category = "Stat", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Int;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, Int)

	UPROPERTY(BlueprintReadOnly, Category = "MeleeAttackRate")
	FGameplayAttributeData BaseMeleeAttackRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseMeleeAttackRate)

	UPROPERTY(BlueprintReadOnly, Category = "MeleeAttackRate")
	FGameplayAttributeData MaxMeleeAttackRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxMeleeAttackRate)

	UPROPERTY(BlueprintReadOnly, Category = "MeleeAttackRate")
	FGameplayAttributeData MeleeAttackRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MeleeAttackRate)

	UPROPERTY(BlueprintReadOnly, Category = "MagicAttackRate")
	FGameplayAttributeData BaseMagicAttackRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseMagicAttackRate)

	UPROPERTY(BlueprintReadOnly, Category = "MagicAttackRate")
	FGameplayAttributeData MagicAttackRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MagicAttackRate)

	UPROPERTY(BlueprintReadOnly, Category = "MagicAttackRate")
	FGameplayAttributeData MaxMagicAttackRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxMagicAttackRate)

	UPROPERTY(BlueprintReadOnly, Category = "DrawRate")
	FGameplayAttributeData BaseBowDrawRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseBowDrawRate)

	UPROPERTY(BlueprintReadOnly, Category = "DrawRate")
	FGameplayAttributeData BowDrawRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BowDrawRate)

	UPROPERTY(BlueprintReadOnly, Category = "DrawRate")
	FGameplayAttributeData MaxBowDrawRate;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, MaxBowDrawRate)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BaseMeleeDamage;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseMeleeDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BaseMagicDamage;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseMagicDamage)

	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData BaseRangeDamage;
	ATTRIBUTE_ACCESSORS(UGSAttributeSetBase, BaseRangeDamage)


	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void AddBaseDamageModifier(const UGSGameplayAbility* Ability, float Amount, bool bAdd);
	void AddResourceRestriction(const UGSGameplayAbility* Ability, EResourceType Type, float Amount);
	void AdjustAttributeForStrChanges(float OldStr, float NewStr);
	float GetMaxMana();
	float GetMaxHealth();
	float GetMaxStamina();
	//float GetBowDrawRate();
	float RecalculateMaxStaminaAfterRestriction();
	float RecalculateMaxHealthAfterRestriction();
	float RecalculateMaxManaAfterRestriction();

	void RecalculateSlowRatio();
	TMap<TSubclassOf<UGSGameplayAbility>,float> HealthRestrictionArray;

	TMap<TSubclassOf<UGSGameplayAbility>, float> ManaRestrictionArray;

	TMap<TSubclassOf<UGSGameplayAbility>, float> StaminaRestrictionArray;

	TMap<TSubclassOf<UGSGameplayAbility>, float> BaseDamageModifierArray;
protected:
	FGameplayTag HeadShotTag;

	// Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes.
	// (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before)
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	/**
	* These OnRep functions exist to make sure that the ability system internal representations are synchronized properly during replication
	**/

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate);

	UFUNCTION()
	virtual void OnRep_Mana(const FGameplayAttributeData& OldMana);

	UFUNCTION()
	virtual void OnRep_ManaRegenRate(const FGameplayAttributeData& OldManaRegenRate);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_StaminaRegenRate(const FGameplayAttributeData& OldStaminaRegenRate);

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);

	UFUNCTION()
	virtual void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield);

	UFUNCTION()
	virtual void OnRep_ShieldRegenRate(const FGameplayAttributeData& OldShieldRegenRate);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);

	UFUNCTION()
	virtual void OnRep_CharacterLevel(const FGameplayAttributeData& OldCharacterLevel);

	UFUNCTION()
	virtual void OnRep_XP(const FGameplayAttributeData& OldXP);

	UFUNCTION()
	virtual void OnRep_XPBounty(const FGameplayAttributeData& OldXPBounty);

	UFUNCTION()
	virtual void OnRep_Gold(const FGameplayAttributeData& OldGold);

	UFUNCTION()
	virtual void OnRep_GoldBounty(const FGameplayAttributeData& OldGoldBounty);
};
