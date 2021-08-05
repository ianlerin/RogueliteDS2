// Copyright 2020 Dan Kestranek.


#include "UI/ResourcebarWidget.h"
#include "Characters/GSCharacterBase.h"
#include "GSBlueprintFunctionLibrary.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Player/GSPlayerController.h"


void UResourcebarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0);
	AGSPlayerController* GSController = Cast<AGSPlayerController>(Controller);
	if (GSController)
	{
		if (GSController->GetPawn())
		{
			AGSCharacterBase* GSChar = Cast<AGSCharacterBase>(GSController->GetPawn());
			Setup(GSChar);
		}
		else
		{
			GSController->OnPossessPawnDelegate.AddDynamic(this, &UResourcebarWidget::Setup);
		}
	}

	if (MaterialInst)
	{
		UE_LOG(LogTemp, Warning, TEXT("UResourcebarWidget::NativeConstruct, MaterialInst found"));

		MaterialInst->SetVectorParameterValue("RestrictColour", RestrictColor);

		MaterialInst->SetVectorParameterValue("FillColour", FillColor);

		MaterialInst->SetVectorParameterValue("EmptyColour", EmptyColor);
	}
	else
	{

		UE_LOG(LogTemp, Warning, TEXT("UResourcebarWidget::NativeConstruct, MaterialInst not found"));
	}
}


void UResourcebarWidget::Setup(AGSCharacterBase* MyPawn)
{
	if (!MyPawn) { return; }
	CharBase = MyPawn;
	if (CharBase)
	{
		switch (MyResource)
		{
		case(EResourceType::ERT_Stamina):
			//CharBase->OnStaminaChangeDelegate.AddDynamic(this, &UResourcebarWidget::OnResourceChange);
			break;
		case(EResourceType::ERT_Health):
			//CharBase->OnHealthChangeDelegate.AddDynamic(this, &UResourcebarWidget::OnResourceChange);
			break;
		case(EResourceType::ERT_Mana):
			//CharBase->OnManaChangeDelegate.AddDynamic(this, &UResourcebarWidget::OnResourceChange);
			break;
		}
	}
}

void UResourcebarWidget::OnResourceChange()
{
	float CurrentResource = 0;
	float CurrentRestrictedResource = 0;
	float CurrentMaxResource = 0;
	switch (MyResource)
	{
	case(EResourceType::ERT_Stamina):
		CurrentResource = CharBase->GetStamina();
		CurrentRestrictedResource = CharBase->GetRestrictedStamina();
		CurrentMaxResource = CharBase->GetMaxStamina();
		break;
	case(EResourceType::ERT_Health):
		CurrentResource = CharBase->GetHealth();
		CurrentRestrictedResource = CharBase->GetRestrictedHealth();
		CurrentMaxResource = CharBase->GetMaxHealth();

		break;
	case(EResourceType::ERT_Mana):
		CurrentResource = CharBase->GetMana();
		CurrentRestrictedResource = CharBase->GetRestrictedMana();
		CurrentMaxResource = CharBase->GetMaxMana();
		break;
	}

	if (MaterialInst)
	{
		MaterialInst->SetScalarParameterValue("Restrict", CurrentRestrictedResource/ CurrentMaxResource);

		MaterialInst->SetScalarParameterValue("Left", CurrentResource / CurrentMaxResource);
	}
	FText CurrentText=UGSBlueprintFunctionLibrary::GetFloatAsTextWithPrecision(CurrentResource, 0,false);
	FText CurrentMaxText = UGSBlueprintFunctionLibrary::GetFloatAsTextWithPrecision(CurrentMaxResource, 0,false);
	CurrentResourceText->SetText(CurrentText);
	MaxResourceText->SetText(CurrentMaxText);
}