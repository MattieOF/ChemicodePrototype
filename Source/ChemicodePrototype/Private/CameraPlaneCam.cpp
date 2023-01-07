// copyright lolol

#include "CameraPlaneCam.h"

// Sets default values
ACameraPlaneCam::ACameraPlaneCam()
{
	PrimaryActorTick.bCanEverTick = false;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	RootComponent = CameraComponent;
}

void ACameraPlaneCam::SetFOV(float NewFOV)
{
	CameraComponent->SetFieldOfView(NewFOV);
}
