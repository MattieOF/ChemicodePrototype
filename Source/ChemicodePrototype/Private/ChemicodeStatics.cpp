// copyright lolol

#include "ChemicodeStatics.h"

#include "ChemicodeGameMode.h"
#include "Kismet/GameplayStatics.h"

AChemicodePawn* UChemicodeStatics::GetChemicodePawn(UObject* World)
{
	return Cast<AChemicodePawn>(UGameplayStatics::GetPlayerPawn(World, 0));
}

AChemicodeGameMode* UChemicodeStatics::GetChemicodeGameMode(UObject* World)
{
	return Cast<AChemicodeGameMode>(UGameplayStatics::GetGameMode(World));
}
