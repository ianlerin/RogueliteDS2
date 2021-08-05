// Copyright 2020 Dan Kestranek.


#include "Animation/SwordChargeTransitionNotify.h"
#include "AI/SwordChargeState.h"
#include "AI/AIStateHandlerComponent.h"

void USwordChargeTransitionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (!MeshComp) { return; }
	ACharacter* Char = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Char) { return; }
	AController* Controller = Char->GetController();
	if (Controller)
	{
		UAIStateHandlerComponent* StateHandler = Controller->FindComponentByClass<UAIStateHandlerComponent>();
		USwordChargeState* SwordCharge = Cast<USwordChargeState>(StateHandler->GetCurrentBaseState());
		if (SwordCharge)
		{
			UE_LOG(LogTemp, Warning, TEXT("UBowStateTransitionNotify::set new Stage"));
			SwordCharge->SetStage(SwordChargeState);
		}
		else
		{

			UE_LOG(LogTemp, Warning, TEXT("UBowStateTransitionNotify::Not Bow Stage"));
		}
	}
}