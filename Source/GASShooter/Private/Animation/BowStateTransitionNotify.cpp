// Copyright 2020 Dan Kestranek.


#include "Animation/BowStateTransitionNotify.h"
#include "AI/BowState.h"
#include "AI/AIStateHandlerComponent.h"

void UBowStateTransitionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UE_LOG(LogTemp, Warning, TEXT("UBowStateTransitionNotify::Notify"));
	Super::Notify(MeshComp, Animation);
	if (!MeshComp) { return; }
	ACharacter* Char = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Char) { return; }
	AController* Controller = Char->GetController();
	if (Controller)
	{
		UAIStateHandlerComponent* StateHandler = Controller->FindComponentByClass<UAIStateHandlerComponent>();
		UBowState* BowState=Cast<UBowState>(StateHandler->GetCurrentBaseState());
		if (BowState)
		{
			UE_LOG(LogTemp, Warning, TEXT("UBowStateTransitionNotify::set new Stage"));
			BowState->SetStage(BowAimState);
		}
		else
		{

			UE_LOG(LogTemp, Warning, TEXT("UBowStateTransitionNotify::Not Bow Stage"));
		}
	}
}