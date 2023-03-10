// copyright lolol

#include "ChemicodeGameMode.h"

#include "CameraPlane.h"
#include "ChemicodePawn.h"
#include "Kismet/GameplayStatics.h"

// TODO: Improve the system for loading and getting CamPlanes:
// Instead, have a Name variable for each CamPlane and find all CamPlane actors,
// and put them into a String->CamPlane* map and use the name to find the CamPlane ref
// Can check if they need to be loaded if Map.Num() is 0

AChemicodeGameMode::AChemicodeGameMode()
{
	DefaultPawnClass = AChemicodePawn::StaticClass();
}

void AChemicodeGameMode::BeginPlay()
{
	Super::BeginPlay();

	NotificationPanel = Cast<UNotificationPanelWidget>(CreateWidget(GetWorld(), NotificationPanelWidgetClass));
	NotificationPanel->AddToViewport(6);
}

ACameraPlane* AChemicodeGameMode::GetTableCamPlane()
{
	if (!TableCamPlane)
		FindCamPlanes();
	return TableCamPlane;
}

ACameraPlane* AChemicodeGameMode::GetCabinetCamPlane()
{
	if (!CabinetCamPlane)
		FindCamPlanes();
	return CabinetCamPlane;
}

ACameraPlane* AChemicodeGameMode::GetComputerCamPlane()
{
	if (!ComputerCamPlane)
		FindCamPlanes();
	return ComputerCamPlane;
}

// ACameraPlane* AChemicodeGameMode::GetBinCamPlane()
// {
// 	if (!BinCamPlane)
// 		FindCamPlanes();
// 	return BinCamPlane;
// }

ACameraPlane* AChemicodeGameMode::GetSubmissionCamPlane()
{
	if (!SubmissionCamPlane)
		FindCamPlanes();
	return SubmissionCamPlane;
}

void AChemicodeGameMode::FindCamPlanes()
{
	// Get Cam Plane references
	TArray<AActor*> Result;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraPlane::StaticClass(), "TableCamPlane", Result);
	check(Result.Num() == 1);
	TableCamPlane = Cast<ACameraPlane>(Result[0]);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraPlane::StaticClass(), "CabinetCamPlane", Result);
	check(Result.Num() == 1);
	CabinetCamPlane = Cast<ACameraPlane>(Result[0]);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraPlane::StaticClass(), "ComputerCamPlane", Result);
	check(Result.Num() == 1);
	ComputerCamPlane = Cast<ACameraPlane>(Result[0]);
	// UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraPlane::StaticClass(), "BinCamPlane", Result);
	// check(Result.Num() == 1);
	// BinCamPlane = Cast<ACameraPlane>(Result[0]);
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ACameraPlane::StaticClass(), "SubmissionCamPlane", Result);
	check(Result.Num() == 1);
	SubmissionCamPlane = Cast<ACameraPlane>(Result[0]);
}
