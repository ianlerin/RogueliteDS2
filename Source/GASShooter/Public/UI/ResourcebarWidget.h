// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "GASShooter/GASShooter.h"
#include "Blueprint/UserWidget.h"
#include "ResourcebarWidget.generated.h"

class UImage;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class GASSHOOTER_API UResourcebarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* MyResourceBar;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void Setup(AGSCharacterBase* MyPawn);

	UPROPERTY(EditAnywhere)
	EResourceType MyResource;

	UPROPERTY(EditAnywhere)
	FLinearColor FillColor;

	UPROPERTY(EditAnywhere)
	FLinearColor RestrictColor;

	UPROPERTY(EditAnywhere)
	FLinearColor EmptyColor;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* CurrentResourceText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* MaxResourceText;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInstanceDynamic* MaterialInst = nullptr;

	UFUNCTION(BlueprintCallable)
	void OnResourceChange();
	UPROPERTY()
	AGSCharacterBase* CharBase = nullptr;




};
