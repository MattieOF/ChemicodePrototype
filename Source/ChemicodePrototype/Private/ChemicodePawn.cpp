// copyright lolol


#include "ChemicodePawn.h"

#include "ChemicodeGameMode.h"
#include "ResourceInfoWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AChemicodePawn::AChemicodePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChemicodePawn::BeginPlay()
{
	Super::BeginPlay();

	// Init references
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	GameMode = Cast<AChemicodeGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// Show the cursor
	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	// Set view target to initial CamPlane
	PlayerController->SetViewTarget(GameMode->GetTableCamPlane()->GetCamPositionActorChecked());
	CurrentCamPlane = GameMode->GetTableCamPlane();

	// Create resource info widget and hide it
	InfoWidget = CreateWidget<UResourceInfoWidget>(GetWorld(), ResourceInfoWidgetClass);
	InfoWidget->AddToViewport();
	InfoWidget->Hide(true); // True param hides the UI instantly without animating
}

// Called every frame
void AChemicodePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LookCooldown -= DeltaTime;

	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel1, false, HitResult))
	{
		TargetItemPosition = HitResult.ImpactPoint;
		if (HeldItem)
		{
			// Move the item in direction
			const auto Direction = (HitResult.ImpactPoint - HeldItem->GetActorLocation()).GetUnsafeNormal();
			auto Position = HeldItem->GetActorLocation() + (Direction * ItemMoveSpeed);
			FVector Center, Bounds;
			HeldItem->GetActorBounds(true, Center, Bounds);
			Position.Z = TargetItemPosition.Z + Bounds.Z + 1;
			HeldItem->SetActorLocation(Position);
		}
	}
	
}

void AChemicodePawn::LookUp()
{
	if (CurrentCamPlane == GameMode->GetTableCamPlane() && LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetCabinetCamPlane());
		LookCooldown = .75f;
	}
}

void AChemicodePawn::LookDown()
{
	if (CurrentCamPlane == GameMode->GetCabinetCamPlane() && LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetTableCamPlane());
		LookCooldown = .75f;
	}
}

void AChemicodePawn::LookLeft()
{
	if ((CurrentCamPlane == GameMode->GetCabinetCamPlane() || CurrentCamPlane == GameMode->GetTableCamPlane()) && LookCooldown <= 0)
	{
		SetCamPlane(GameMode->GetComputerCamPlane());
		LookCooldown = .75f;	
	}
}

void AChemicodePawn::LookRight()
{
	if (CurrentCamPlane == GameMode->GetComputerCamPlane() && LookCooldown <= 0)
	{
		ACameraPlane* Target;
		if (!PrevCamPlane || PrevCamPlane == GameMode->GetComputerCamPlane())
			Target = GameMode->GetTableCamPlane();
		else
			Target = PrevCamPlane;
		SetCamPlane(Target);
		LookCooldown = .75f;	
	}
}

// Called to bind functionality to input
void AChemicodePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("LookUp", EInputEvent::IE_DoubleClick, this, &AChemicodePawn::LookUp);
	PlayerInputComponent->BindAction("LookDown", EInputEvent::IE_DoubleClick, this, &AChemicodePawn::LookDown);
	PlayerInputComponent->BindAction("LookLeft", EInputEvent::IE_DoubleClick, this, &AChemicodePawn::LookLeft);
	PlayerInputComponent->BindAction("LookRight", EInputEvent::IE_DoubleClick, this, &AChemicodePawn::LookRight);
	PlayerInputComponent->BindAction("Use", EInputEvent::IE_Pressed, this, &AChemicodePawn::OnUse);
	PlayerInputComponent->BindAxis("Horizontal", this, &AChemicodePawn::MoveHorizontal);
	PlayerInputComponent->BindAxis("Vertical", this, &AChemicodePawn::MoveVertical);
}

void AChemicodePawn::SetCamPlane(ACameraPlane* NewCamPlane, float BlendTime)
{
	PrevCamPlane = CurrentCamPlane;
	CurrentCamPlane = NewCamPlane;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(NewCamPlane->GetCamPositionActor(),
		BlendTime, EViewTargetBlendFunction::VTBlend_EaseInOut, 2);
	ResourceLostHover();
}

bool AChemicodePawn::ResourceHovered(UResourceData* Resource)
{
	// Only show resource UI if in cabinet
	if (CurrentCamPlane != GameMode->GetCabinetCamPlane())
		return false;

	// Set resource values in UI
	InfoWidget->SetResource(Resource);
	
	if (bResourceInfoVisible) // Don't play show anim again if we're already visible.
		return true;
	InfoWidget->Show();
	bResourceInfoVisible = true;
	return true;
}

void AChemicodePawn::TryBuyResource(UResourceData* Resource)
{
	LookDown(); // Look back at the table
	HeldItem = Cast<AResourceItem>(GetWorld()->SpawnActor(Resource->ResourceItemClass, &TargetItemPosition, &FRotator::ZeroRotator));
	FVector Center, Bounds;
	HeldItem->GetActorBounds(true, Center, Bounds);
	HeldItem->AddActorLocalOffset(FVector(0, 0, Bounds.Z + 1));
	HeldItem->SetResource(Resource);
}

void AChemicodePawn::ResourceLostHover()
{
	if (!bResourceInfoVisible)
		return;
	InfoWidget->Hide();
	bResourceInfoVisible = false;
}

void AChemicodePawn::MoveHorizontal(float Value)
{
	CurrentCamPlane->MoveOnPlane(Value * Speed, 0);
}

void AChemicodePawn::MoveVertical(float Value)
{
	CurrentCamPlane->MoveOnPlane(0, Value * Speed);
}

void AChemicodePawn::OnUse()
{
	if (HeldItem)
		HeldItem->Use();
}
