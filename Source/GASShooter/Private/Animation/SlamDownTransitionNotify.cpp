// Copyright 2020 Dan Kestranek.


#include "Animation/SlamDownTransitionNotify.h"
#include "AI/SlamDownState.h"
void USlamDownTransitionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	if (!MeshComp) { return; }
	ACharacter* Char = Cast<ACharacter>(MeshComp->GetOwner());
	if (!Char) { return; }
	AController* Controller = Char->GetController();
	if (Controller)
	{
		UAIStateHandlerComponent* StateHandler = Controller->FindComponentByClass<UAIStateHandlerComponent>();
		USlamDownState* SlamDown = Cast<USlamDownState>(StateHandler->GetCurrentBaseState());
		if (SlamDown)
		{
			SlamDown->SetStage(SlamDownState);
		}
	}
}