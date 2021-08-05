// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Weapons/GSWeapon.h"
#include "GSShield.generated.h"

/**
 * 
 */
UCLASS()
class GASSHOOTER_API AGSShield : public AGSWeapon 
{
	GENERATED_BODY()
protected:
		// Default attributes for a character for initializing on spawn/respawn.
	// This is an instant GE that overrides the values for attributes that get reset on spawn/respawn.
		UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GASShooter|Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;
		// Reference to the AttributeSetBase. It will live on the PlayerState or here if the character doesn't have a PlayerState.
		UPROPERTY()
		class UGSAttributeSetBase* AttributeSetBase;
		void InitializeAttributes();
		FDelegateHandle HealthChangedDelegateHandle;
		void HealthChanged(const FOnAttributeChangeData& Data);
		virtual void BeginPlay() override;
		AGSShield();
		UPROPERTY(EditDefaultsOnly)
			float Health = 150;
		UFUNCTION(BlueprintCallable)
			float GetHealth();
		UAbilitySystemComponent* GetAbilitySystemComponent() const override;
		UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<UGameplayEffect>>OwnerShieldBreakEffectClass;
};
