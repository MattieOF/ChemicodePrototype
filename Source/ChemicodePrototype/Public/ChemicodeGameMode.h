// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "CameraPlane.h"
#include "GameFramework/GameModeBase.h"
#include "ChemicodeGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class CHEMICODEPROTOTYPE_API AChemicodeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChemicodeGameMode();

	FORCEINLINE ACameraPlane* GetTableCamPlane();
	FORCEINLINE ACameraPlane* GetCabinetCamPlane();
	FORCEINLINE ACameraPlane* GetComputerCamPlane();
	
private:
	UPROPERTY()
	ACameraPlane *TableCamPlane;
	UPROPERTY()
	ACameraPlane *CabinetCamPlane;
	UPROPERTY()
	ACameraPlane *ComputerCamPlane;
	
	void FindCamPlanes();

protected:
	virtual void BeginPlay() override;
};
