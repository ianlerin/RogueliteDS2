// Copyright 2020 Dan Kestranek.


#include "GSBlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Component/InputHandlerComponent.h"
#include "Characters/Heroes/GSHeroCharacter.h"

FString UGSBlueprintFunctionLibrary::GetPlayerEditorWindowRole(UWorld* World)
{
	FString Prefix;
	if (World)
	{
		if (World->WorldType == EWorldType::PIE)
		{
			switch (World->GetNetMode())
			{
			case NM_Client:
				Prefix = FString::Printf(TEXT("Client %d "), GPlayInEditorID - 1);
				break;
			case NM_DedicatedServer:
			case NM_ListenServer:
				Prefix = FString::Printf(TEXT("Server "));
				break;
			case NM_Standalone:
				break;
			}
		}
	}

	return Prefix;
}

UGSGameplayAbility* UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			return Cast<UGSGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		}
	}

	return nullptr;
}

UGSGameplayAbility* UGSBlueprintFunctionLibrary::GetPrimaryAbilityInstanceFromClass(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayAbility> InAbilityClass)
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromClass(InAbilityClass);
		if (AbilitySpec)
		{
			return Cast<UGSGameplayAbility>(AbilitySpec->GetPrimaryInstance());
		}
	}

	return nullptr;
}

bool UGSBlueprintFunctionLibrary::IsPrimaryAbilityInstanceActive(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
	if (AbilitySystemComponent)
	{
		FGameplayAbilitySpec* AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);
		if (AbilitySpec)
		{
			return Cast<UGSGameplayAbility>(AbilitySpec->GetPrimaryInstance())->IsActive();
		}
	}

	return false;
}

bool UGSBlueprintFunctionLibrary::IsAbilitySpecHandleValid(FGameplayAbilitySpecHandle Handle)
{
	return Handle.IsValid();
}

bool UGSBlueprintFunctionLibrary::DoesEffectContainerSpecHaveEffects(const FGSGameplayEffectContainerSpec& ContainerSpec)
{
	return ContainerSpec.HasValidEffects();
}

bool UGSBlueprintFunctionLibrary::DoesEffectContainerSpecHaveTargets(const FGSGameplayEffectContainerSpec& ContainerSpec)
{
	return ContainerSpec.HasValidTargets();
}

void UGSBlueprintFunctionLibrary::ClearEffectContainerSpecTargets(FGSGameplayEffectContainerSpec& ContainerSpec)
{
	ContainerSpec.ClearTargets();
}

void UGSBlueprintFunctionLibrary::AddTargetsToEffectContainerSpec(FGSGameplayEffectContainerSpec& ContainerSpec, const TArray<FGameplayAbilityTargetDataHandle>& TargetData, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	ContainerSpec.AddTargets(TargetData, HitResults, TargetActors);
}

TArray<FActiveGameplayEffectHandle> UGSBlueprintFunctionLibrary::ApplyExternalEffectContainerSpec(const FGSGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;

	// Iterate list of gameplay effects
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		if (SpecHandle.IsValid())
		{
			// If effect is valid, iterate list of targets and apply to all
			for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data)
			{
				AllEffects.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
			}
		}
	}
	return AllEffects;
}

FGameplayAbilityTargetDataHandle UGSBlueprintFunctionLibrary::EffectContextGetTargetData(FGameplayEffectContextHandle EffectContextHandle)
{
	FGSGameplayEffectContext* EffectContext = static_cast<FGSGameplayEffectContext*>(EffectContextHandle.Get());
	
	if (EffectContext)
	{
		return EffectContext->GetTargetData();
	}

	return FGameplayAbilityTargetDataHandle();
}

void UGSBlueprintFunctionLibrary::EffectContextAddTargetData(FGameplayEffectContextHandle EffectContextHandle, const FGameplayAbilityTargetDataHandle& TargetData)
{
	FGSGameplayEffectContext* EffectContext = static_cast<FGSGameplayEffectContext*>(EffectContextHandle.Get());

	if (EffectContext)
	{
		EffectContext->AddTargetData(TargetData);
	}
}

void UGSBlueprintFunctionLibrary::ClearTargetData(FGameplayAbilityTargetDataHandle& TargetData)
{
	TargetData.Clear();
}



FComboChainData UGSBlueprintFunctionLibrary::InitialFindNextAnimToPlay(UDataTable* Table, EWeaponType WeaponType, TArray< EGSAbilityInputID>InputCollection,AActor* AvatarActor, TArray <FComboChainData>&FutureChainData)
{
	FComboChainData ToReturn;
	if (!Table) { return ToReturn; }

	TArray<FMeleeWeaponData*>MeleeWeaponDataCollection;
	Table->GetAllRows("Combo", MeleeWeaponDataCollection);
	for(auto Data: MeleeWeaponDataCollection)
	{
		if (WeaponType == Data->Type)
		{
			return FindNextAnimToPlay(Data->MyComboCollection, WeaponType, InputCollection, AvatarActor, FutureChainData);
		}
	}
	return ToReturn;
}

FComboChainData UGSBlueprintFunctionLibrary::FindNextAnimToPlay(TArray< FComboChainData>PossibleChainCombo, EWeaponType WeaponType, TArray< EGSAbilityInputID>InputCollection, AActor* AvatarActor, TArray <FComboChainData>&FutureChainData)
{
	UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::FindNextAnimToPlay"));
	FComboChainData ToReturn;
	AGSHeroCharacter* MyHero = Cast<AGSHeroCharacter>(AvatarActor);
	if (!MyHero) { return ToReturn; }
	UInputHandlerComponent* InputHandler = MyHero->GetInputHandlerComp();
	TArray< FComboChainData> MatchingComboChain;
	CompareComboChain(PossibleChainCombo, InputCollection, MatchingComboChain, FutureChainData);

	UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::FindNextAnimToPlay,MatchingCombo:%i,InputHandler->GetPressedInput():%i"), MatchingComboChain.Num(), InputHandler->GetPressedInput().Num());
	for (auto MatchingCombo : MatchingComboChain)
	{
		// find best combo chain based on pressed key
		if (InputHandler->GetPressedInput().Num() == MatchingCombo.AdditionalKeyRequired.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::FindNextAnimToPlay,num match"));
			bool bAllInputMatch = true;
			for (int i = 0; i < InputHandler->GetPressedInput().Num(); i++)
			{
				if (InputHandler->GetPressedInput()[i] != MatchingCombo.AdditionalKeyRequired[i])
				{
					UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::FindNextAnimToPlay,input not match"));
					bAllInputMatch = false;
				}
			}
			if (bAllInputMatch)	
			{
				return MatchingCombo;
			}
		}
	}
	
	// no best case found, return a random one
	if (MatchingComboChain.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::FindNextAnimToPlay,Found:%s"),*MatchingComboChain[0].MontageToPlay->GetName());
		ToReturn = MatchingComboChain[0];
	}
	
	return ToReturn;
}

void UGSBlueprintFunctionLibrary::CompareComboChain(TArray< FComboChainData> DTComboChain, TArray< EGSAbilityInputID>InputCollection, TArray<FComboChainData>&MatchingComboChain,TArray<FComboChainData>&FutureChainData)
{
		for (auto DTCombo : DTComboChain)
		{
			if (DTCombo.InputCollection == InputCollection)
			{
				UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::CompareComboChain,found matching:%s"),*DTCombo.MontageToPlay->GetName());
				MatchingComboChain.Add(DTCombo);
			}

			else if (DTCombo.InputCollection.Num() > InputCollection.Num())
			{
				bool bPotentialFutureMatch = true;
				for (int i = 0; i < InputCollection.Num(); i++)
				{
					if (DTCombo.InputCollection[i] != InputCollection[i])
					{
						bPotentialFutureMatch = false;
						break;
					}
				}
				if (bPotentialFutureMatch)
				{
					UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::CompareComboChain,found future matching:%s"), *DTCombo.MontageToPlay->GetName());
					FutureChainData.Add(DTCombo);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::CompareComboChain,not found future matching:%s"), *DTCombo.MontageToPlay->GetName());
				}
			}


		}
	
}

EDirection UGSBlueprintFunctionLibrary::GetRollDirection(UDataTable* RollTable, EGSAbilityInputID PrimaryRollKey, AActor* AvatarActor)
{
	EDirection ToReturn = EDirection::ED_None;
	TArray<FRollTable*>RollDataCollection;
	AGSHeroCharacter* MyHero = Cast<AGSHeroCharacter>(AvatarActor);
	if (!MyHero) { return ToReturn; }
	UInputHandlerComponent* InputHandler = MyHero->GetInputHandlerComp();
	RollTable->GetAllRows("Roll", RollDataCollection);
	for (auto Data : RollDataCollection)
	{
		if (PrimaryRollKey == Data->PrimaryKey)
		{
			for (auto PressedInput : InputHandler->GetPressedInput())
			{
				if (PressedInput == Data->AdditionalInput)
				{
					//found perfect match
					return Data->RollDirection;
				}
			}
			if (Data->AdditionalInput == EAdditionalHelperInput::None)
			{
				ToReturn = Data->RollDirection;
			}
		
		}
	}
	return ToReturn;
}

float UGSBlueprintFunctionLibrary::AdditionalYawToRotateToFaceLocation(AActor* ActorToRotate, FVector LocToFace, bool bAllPositive)
{
	if (!ActorToRotate) { return 0; }
	float RotateAngle = (LocToFace - ActorToRotate->GetActorLocation()).Rotation().Yaw;
	RotateAngle -= ActorToRotate->GetActorRotation().Yaw;

	if (RotateAngle > 180)
	{
		RotateAngle = RotateAngle - 360;
	}
	if (RotateAngle < -180)
	{
		RotateAngle = RotateAngle + 360;
	}

	if (bAllPositive)
	{
		if (RotateAngle < 0)
		{
			RotateAngle = 360 + RotateAngle;
		}
	}

	return RotateAngle;
}



EDirection UGSBlueprintFunctionLibrary::ConvertRotationToDirection(FRotator Rot)
{
	float Yaw = Rot.Yaw;
	if (Yaw > -45 && Yaw < 45)
	{
		return EDirection::ED_Forward;
	}
	if (Yaw<-45 && Yaw>-67.5)
	{
		return EDirection::ED_ForwardLeft;
	}
	if (Yaw>45 && Yaw<67.5)
	{
		return EDirection::ED_ForwardLeft;
	}
	if (Yaw>-90 && Yaw<-67.5)
	{
		return EDirection::ED_Left;
	}
	if (Yaw <90 && Yaw >67.5)
	{
		return EDirection::ED_Right;
	}
	if (Yaw <-90 && Yaw >-135)
	{
		return EDirection::ED_BackwardLeft;
	}
	if (Yaw >90 && Yaw <135)
	{
		return EDirection::ED_BackwardRight;
	}
	return EDirection::ED_Backward;
}

FGameplayAbilityTargetDataHandle UGSBlueprintFunctionLibrary::MakeGameplayAbilityTargetDataHandleFromHitResults(const TArray<FHitResult> HitResults)
{
	FGameplayAbilityTargetDataHandle TargetData;

	for (const FHitResult& HitResult : HitResults)
	{
		FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
		TargetData.Add(NewData);
	}

	return TargetData;
}

ETeamAttitude::Type UGSBlueprintFunctionLibrary::GetAttitudeBetweenActors(AActor* A, AActor* B)
{
	if (!A) {
		UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::GetAttitudeBetweenActors, no A")); 
		return ETeamAttitude::Neutral; }


	if (!B) {
		UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::GetAttitudeBetweenActors, no B"));
		return ETeamAttitude::Neutral; }

	IGenericTeamAgentInterface* AInterface = Cast<IGenericTeamAgentInterface>(A);

	if (!AInterface) {

		UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::GetAttitudeBetweenActors, no A interface"));
		return ETeamAttitude::Neutral; }

	IGenericTeamAgentInterface* BInterface = Cast<IGenericTeamAgentInterface>(B);

	if (!BInterface) { 

		UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::GetAttitudeBetweenActors, no B interface"));
		return ETeamAttitude::Neutral; }

	if (AInterface->GetGenericTeamId().GetId() != BInterface->GetGenericTeamId().GetId())
	{
		UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::GetAttitudeBetweenActors Hostile between %s and %s"), *A->GetName(), *B->GetName());
		return ETeamAttitude::Hostile;
	}

	UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::GetAttitudeBetweenActors, AID:%i, BID:%i"),AInterface->GetGenericTeamId().GetId(), BInterface->GetGenericTeamId().GetId());
	return ETeamAttitude::Neutral;
}

USoundCue* UGSBlueprintFunctionLibrary::GetHitSoundCue(UDataTable* WeaponHitTable,AActor* SourceActor,UPhysicalMaterial* HitMat, bool bIsCritical)
{
	if (!HitMat) { return nullptr; }
	if (!WeaponHitTable) { return nullptr; }

	TArray< FWeaponHitData*> WeaponHitArray;
	WeaponHitTable->GetAllRows<FWeaponHitData>("Name", WeaponHitArray);
	for (auto WeaponHit : WeaponHitArray)
	{
		if (WeaponHit->WeaponType == SourceActor->GetClass())
		{
			if (WeaponHit->SurfaceSoundMap.Contains(HitMat->SurfaceType))
			{
				if (bIsCritical)
				{

					return WeaponHit->SurfaceSoundMap[HitMat->SurfaceType].CriticalSound;
				}
				else
				{

					return WeaponHit->SurfaceSoundMap[HitMat->SurfaceType].NormalSound;
				}
			}
		}
	}
	return nullptr;
}

TArray<FHitResult> UGSBlueprintFunctionLibrary::ArcLineTargetForObjects_Front(AActor* MyActor,FVector StartLoc, float ForwardRange, int SmashArc, int IncrementArc, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectsToTraceFor, TArray<AActor*>ActorsToIgnore)
{
	TArray<FHitResult> ToReturn;
	int MinArc = -SmashArc / 2;
	int MaxArc = SmashArc / 2;
	for (int i = MinArc; i <= MaxArc; i = i + IncrementArc)
	{
		TArray<FHitResult> MyRes;
		FVector AdjustedForward = UKismetMathLibrary::GreaterGreater_VectorRotator(MyActor->GetActorForwardVector(), FRotator(0, i, 0));
		FVector EndLoc = StartLoc + AdjustedForward * ForwardRange;

		UE_LOG(LogTemp, Warning, TEXT("UGSBlueprintFunctionLibrary::ArcLineTargetForObjects_Front, StartLoc:%s, EndLoc:%s"),*StartLoc.ToString(),*EndLoc.ToString());
		UKismetSystemLibrary::LineTraceMultiForObjects(MyActor, StartLoc, EndLoc, ObjectsToTraceFor, false, ActorsToIgnore, EDrawDebugTrace::None, MyRes, true);
		ToReturn.Append(MyRes);
	}
	return ToReturn;
}

FText UGSBlueprintFunctionLibrary::GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero)
{
	//Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	if (FMath::Abs(TheFloat - Rounded) < FMath::Pow(10, -1 * Precision))
	{
		TheFloat = Rounded;
	}
	FNumberFormattingOptions NumberFormat; //Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	return FText::AsNumber(TheFloat, &NumberFormat);
}

FCastExtraData UGSBlueprintFunctionLibrary::GetCasterMontage(TSubclassOf<UGameplayAbility> Class, UDataTable* Table, AGSWeapon* CurrentWeapon)
{
	TArray< FCastAbilityAnimData*> CastAbilityArray;
	Table->GetAllRows<FCastAbilityAnimData>("Name", CastAbilityArray);
	for (auto Data: CastAbilityArray)
	{
		if (Class == Data->CastAbility.LoadSynchronous())
		{
			if (CurrentWeapon)
			{

				if (Data->DataCollection.Contains(CurrentWeapon->GetWeaponType()))
				{

					return Data->DataCollection[CurrentWeapon->GetWeaponType()];
				}
				else
				{
					//If the weapon is not set in DT, will play the default anim instead

					if (Data->DataCollection.Contains(EWeaponType::EWT_UnArmed))
					{
						//Alex, 12 Aug - Debug for default enum for DT_CastAnim
						UE_LOG(LogTemp, Warning, TEXT("Alex: default anim"));

						return Data->DataCollection[EWeaponType::EWT_UnArmed];
					}
				}
			}
			else
			{

				if (Data->DataCollection.Contains(EWeaponType::EWT_UnArmed))
				{
					return Data->DataCollection[EWeaponType::EWT_UnArmed];
				}
			}
		}
	}

	//Alex, 12 Aug - Debug for default enum for DT_CastAnim
	UE_LOG(LogTemp, Error, TEXT("return ExtraData;"));

	FCastExtraData ExtraData;
	return ExtraData;
}

FVector UGSBlueprintFunctionLibrary::GetVelocityToReachTarget(FVector Start, FVector End, FVector Acceleration, float T)
{
	FVector Diff = End - Start;
	return (Diff - 0.5*Acceleration*T*T) / T;
}