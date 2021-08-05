// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"

#define ACTOR_ROLE_FSTRING *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(GetLocalRole()))
#define GET_ACTOR_ROLE_FSTRING(Actor) *(FindObject<UEnum>(ANY_PACKAGE, TEXT("ENetRole"), true)->GetNameStringByValue(Actor->GetLocalRole()))

#define COLLISION_ABILITY						ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_PROJECTILE					ECollisionChannel::ECC_GameTraceChannel2
#define COLLISION_ABILITYOVERLAPPROJECTILE		ECollisionChannel::ECC_GameTraceChannel3
#define COLLISION_PICKUP						ECollisionChannel::ECC_GameTraceChannel4
#define COLLISION_INTERACTABLE					ECollisionChannel::ECC_GameTraceChannel5

UENUM(BlueprintType)
enum class EGSAbilityInputID : uint8
{
	// 0 None
	None				UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm				UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel				UMETA(DisplayName = "Cancel"),
	// 3 Sprint
	Sprint				UMETA(DisplayName = "Sprint"),
	// 4 Jump
	Jump				UMETA(DisplayName = "Jump"),
	// 5 PrimaryFire
	PrimaryFire			UMETA(DisplayName = "Primary Fire"),
	// 6 SecondaryFire
	SecondaryFire		UMETA(DisplayName = "Secondary Fire"),
	// 7 Alternate Fire
	AlternateFire		UMETA(DisplayName = "Alternate Fire"),
	// 8 Reload
	Reload				UMETA(DisplayName = "Reload"),
	// 9 NextWeapon
	NextWeapon			UMETA(DisplayName = "Next Weapon"), 
	// 10 PrevWeapon
	PrevWeapon			UMETA(DisplayName = "Previous Weapon"),
	// 11 Interact
	Interact			UMETA(DisplayName = "Interact"),

	Up,

	Down,

	Left,

	Right,
	
	Focus,

	Test,
	
	Test2
};

UENUM(BlueprintType)
enum class EAdditionalHelperInput : uint8
{
	None, 

	UpKey,

	DownKey,

	RightKey,

	LeftKey,
	
	AltKey,

	CtrlKey
};


UENUM(BlueprintType)
enum class EDirection : uint8
{
	ED_Forward,
	ED_ForwardLeft,
	ED_ForwardRight,
	ED_Backward,
	ED_BackwardLeft,
	ED_BackwardRight,
	ED_Left,
	ED_Right,
	ED_None
};

UENUM(BlueprintType)
enum class EAIState : uint8
{
	EAS_None,
	EAS_Idle,
	EAS_Attack,
	EAS_PreCast,
	EAS_Cast,
	EAS_BowAim,
	EAS_SwordCharge,
	EAS_Dodge,
	EAS_Roll,
	EAS_Follow,
	EAS_PreAttack,
	EAS_Stun,
	EAS_SlamDown,
	EAS_Death
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_UnArmed,
	EWT_Sword,
	EWT_Bow,
	EWT_Shield
};


UENUM(BlueprintType)
enum class EBowDrawState : uint8
{
	EBDS_None,
	EBDS_Start,
	EBDS_Draw,
	EBDS_Aim,
	EBDS_Release
};

UENUM(BlueprintType)
enum class ESwordChargeState : uint8
{
	ECS_None,
    ECS_Start,
	ECS_Loop,
	ECS_End
};


UENUM(BlueprintType)
enum class ESlamDownState : uint8
{
	ESDS_None,
	ESDS_Start,
	ESDS_Loop,
	ESDS_End
};


UENUM(BlueprintType)
enum class EWeaponEquipSlot : uint8
{
	EWES_Mainhand,
	EWES_OffHand
};

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	EDT_Melee,
	EDT_Range,
	EDT_Magic
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	ERT_Health,
	ERT_Mana,
	ERT_Stamina

};

UENUM(BlueprintType)
enum class EProjectileImpact : uint8
{
	None,
	Explode
};

UENUM(BlueprintType)
enum class EElementalType : uint8
{
	None,
	Fire,
	Ice,
	Lightning
};