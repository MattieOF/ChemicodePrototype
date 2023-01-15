// copyright lolol

#pragma once

#include "CoreMinimal.h"
#include "ChemicodeGameMode.h"
#include "ChemicodePawn.h"
#include "Interaction.h"
#include "Notification.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ChemicodeStatics.generated.h"

/**
 * Helper functions for interacting with the Chemicode systems, or helper functions for the engine used by Chemicode. 
 */
UCLASS(ClassGroup=(Chemicode))
class CHEMICODEPROTOTYPE_API UChemicodeStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Gets the players pawn as a Chemicode Pawn object
	 * @param World World context object
	 * @return The chemicode pawn object
	 */
	UFUNCTION(BlueprintCallable, Category = "Actor References", BlueprintPure, meta=(WorldContext="World"))
	static FORCEINLINE AChemicodePawn* GetChemicodePawn(UObject* World) { return Cast<AChemicodePawn>(UGameplayStatics::GetPlayerPawn(World, 0)); };

	/**
	 * @brief Gets the current gamemode object as a Chemicode gamemode object
	 * @param World World context object
	 * @return The chemicode gamemode object
	 */
	UFUNCTION(BlueprintCallable, Category = "Actor References", BlueprintPure, meta=(WorldContext="World"))
	static FORCEINLINE AChemicodeGameMode* GetChemicodeGameMode(UObject* World) { return Cast<AChemicodeGameMode>(UGameplayStatics::GetGameMode(World)); };

	/**
	 * @brief Clamps a vector per-element between a minimum and a maximum vector
	 * @param Vector Vector to be clamped
	 * @param Min Minimum vector
	 * @param Max Maximum vector
	 * @return Vector clamped between Min and Max
	 */
	UFUNCTION(BlueprintCallable, Category = "Math", BlueprintPure)
	static FVector2D ClampVector2D(FVector2D Vector, FVector2D Min, FVector2D Max);

	/**
	 * @brief Utility function to add a debug error notification, but only if UE_BUILD_SHIPPING is false.
	 * @param World World context object
	 * @param Title Title of the notification
	 * @param Desc Description of the notification
	 * @param Time How long the notification should be on screen, in seconds. <b>0 will create a notification requiring manual dismissal.</b>
	 */
	static FORCEINLINE void DebugErrorNotification(UObject* World, FString Title, FString Desc, float Time = 3)
	{
#if !UE_BUILD_SHIPPING
		GetChemicodeGameMode(World)->AddNotification(FNotification(FText::FromString(Title), FText::FromString(Desc), Time, NTError));
#endif
	}

	/**
	 * @brief Utility function similar to GetHitResultUnderCursor, but also includes an IgnoredActors parameter.
	 * @param Controller Player controller to do the check for
	 * @param ObjectTypes Array of of object type queries to check for
	 * @param bTraceComplex If the trace should be against complex collision
	 * @param HitResult Reference to an FHitResult to store data in
	 * @param IgnoredActors Array of actors (AActor pointers) to be ignored by the trace
	 * @return True if the trace hit something, false if not OR if there was an error.
	 */
	UFUNCTION(BlueprintCallable)
	static bool GetHitResultAtCursor(const APlayerController* Controller, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, bool bTraceComplex, FHitResult& HitResult, const TArray<AActor*>& IgnoredActors);

	/**
	 * @return An FInteraction object that is invalid. Useful for when there is no interaction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FORCEINLINE FInteraction GetInvalidInteraction() { return FInteraction(); }

	/**
	 * @brief Returns an FString representing an FResourceMeasurement.\n
	 * For example, take an FResourceMeasurement with a unit of Millilitres and a value of 123.4.\n
	 * If bShorthand is false, the output will be "123.4 Millilitres".
	 * If bShorthand is true, the output will be "123.4mL".
	 * @param Measurement Measurement to process
	 * @param bShorthand Use shorthand measurement symbols. See brief for more info
	 * @return FString containing the processed string
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString MeasurementAsString(FResourceMeasurement Measurement, bool bShorthand = false);

	/**
	 * @brief Iterates through all actors in the world until it finds one with the provided tag, and returns it. Expensive function; don't call every frame!
	 * @param WorldContext World context object
	 * @param Tag Tag to look for
	 * @return If one was found, the relevant AActor pointer. If not, nullptr.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(WorldContext="WorldContext"))
	static AActor* GetFirstActorWithTag(UObject* WorldContext, FName Tag);

	/**
	 * @brief Enables or disables all outlines by enabling or disabling the outline post process volume.
	 * @param WorldContext World context object
	 * @param NewOutlinesEnabled If the outlines should be enabled or not
	 * @param OutlineVolumeTag Tag on the outline post process volume, by default "OutlinePPVolume".
	 * Only the volume should have this tag, as only 1 actor with this tag is considered.
	 */
	UFUNCTION(BlueprintCallable, meta=(WorldContext="WorldContext"))
	static void SetOutlinesEnabled(UObject* WorldContext, bool NewOutlinesEnabled, FName OutlineVolumeTag = "OutlinePPVolume");

	UFUNCTION(BlueprintCallable, BlueprintPure, meta=(WorldContext="WorldContext"))
	static float GetCurrentInteractionProgress(UObject* WorldContext);
};
