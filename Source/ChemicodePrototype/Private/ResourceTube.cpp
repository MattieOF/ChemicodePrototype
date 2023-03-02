// copyright lolol

#include "ResourceTube.h"

#include "ChemicodePawn.h"
#include "ChemicodeStatics.h"
#include "ChemicodePrototype/ChemicodePrototype.h"
#include "Kismet/KismetMathLibrary.h"

AResourceTube::AResourceTube()
{
	PrimaryActorTick.bCanEverTick = true;

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->EndLocation = FVector(100, 0, 10);
	Cable->bAttachEnd = false;
	Cable->bEnableCollision = true;
	Cable->CastShadow = false;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	Box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Box->SetCollisionResponseToChannel(COLLISION_CHANNEL_BLOCKITEM, ECR_Block);
	Box->MoveIgnoreActors.Add(this);
	Box->SetupAttachment(Cable);
	RootComponent = Cable;
	
	bResourceLike = false;
	bRequireFreeSpace = false;
	Name = FText::FromString("Gas Tube");
	Description = FText::FromString("Connect to containers to facilitate some reactions");

	OnItemPickedUp.AddLambda([this]
	{
		DisconnectObjects();
	});

	OnItemPlaced.AddLambda([this]
	{
		OnItemPickedUp.Broadcast();
	});
}

void AResourceTube::BeginPlay()
{
	Super::BeginPlay();
}

void AResourceTube::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// -- WORLD EFFECTS ONLY BELOW HERE --
	if (bSimulated)
		return;

	// Get start and end locations
	TArray<FVector> Locations;
	Cable->GetCableParticleLocations(Locations);
	const FVector First = Locations[0];
	const FVector Last = Locations.Last();

	Box->SetWorldLocation((First + Last) / 2);
	Box->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(First, Last));
	Box->SetBoxExtent(FVector(FVector::Dist(First, Last) / 2, 8, 8));

	if (RHSConnection)
		SetActorLocation(RHSConnection->GetMainMesh()->GetSocketLocation("TubeConnection"));
}

bool AResourceTube::TransferResourceAs(AChemicodeObject* From, UResourceData* Resource, UResourceData* As,
                                       FResourceMeasurement Amount, float AsScale, bool bMultiplyByDeltaTime,
                                       float DeltaTimeOverride)
{
	// TODO: Prevent transferring more than the source has
	AChemicodeObject* To = nullptr;
	if (From == LHSConnection)
		To = RHSConnection;
	else if (From == RHSConnection)
		To = LHSConnection;
	
	if (To == nullptr)
		return false;

	if (bMultiplyByDeltaTime)
		Amount.Value *= DeltaTimeOverride == 0 ? (WorldRef ? WorldRef : GetWorld())->DeltaTimeSeconds : DeltaTimeOverride;

	Amount.Value = FMath::Min(Amount.Value, From->GetResourceAmount(Resource).Value);

	To->ReceiveResource(As, Amount * AsScale);
	Amount.Value = -Amount.Value;
	From->ReceiveResource(Resource, Amount);
	
	return true;
}

bool AResourceTube::TransferResources(AChemicodeObject* From,
                                      TArray<FMeasuredResource> Resources, float Scale, bool bMultiplyByDeltaTime, float DeltaTimeOverride)
{
	for (const auto Element : Resources)
	{
		if (!TransferResource(From, Element.Resource, Element.Amount, Scale, bMultiplyByDeltaTime, DeltaTimeOverride))
			return false;	
	}

	return true;
}

bool AResourceTube::ConnectObject(AChemicodeObject* Object)
{
	if (!LHSConnection)
	{
		LHSConnection = Object;
		LHSConnection->OnDestroyed.AddDynamic(this, &AResourceTube::OnConnectionDestroyed);
		if (!bSimulated)
		{
			Cable->SetAttachEndToComponent(Object->GetMainMesh(), "TubeConnection");
			Cable->bAttachEnd = true;
			Cable->EndLocation = FVector::ZeroVector;
		}
		return true;
	} else if (!RHSConnection)
	{
		RHSConnection = Object;
		RHSConnection->OnDestroyed.AddDynamic(this, &AResourceTube::OnConnectionDestroyed);
		if (!bSimulated)
		{
			SetActorLocation(Object->GetMainMesh()->GetSocketLocation("TubeConnection"));
			UChemicodeStatics::GetChemicodePawn(WorldRef ? WorldRef : GetWorld())->DropItem();
		}
		return true;
	}
	
	return false;
}

void AResourceTube::DisconnectObjects()
{
	if (LHSConnection && !LHSConnection->IsActorBeingDestroyed())
		LHSConnection->OnDestroyed.RemoveDynamic(this, &AResourceTube::OnConnectionDestroyed);
	if (RHSConnection && !RHSConnection->IsActorBeingDestroyed())
		RHSConnection->OnDestroyed.RemoveDynamic(this, &AResourceTube::OnConnectionDestroyed);
	LHSConnection = nullptr;
	RHSConnection = nullptr;
	Cable->bAttachEnd = false;
	Cable->EndLocation = FVector(100, 0, 10);
}

void AResourceTube::GetActorBounds(bool bOnlyCollidingComponents, FVector& Origin, FVector& BoxExtent,
                                   bool bIncludeFromChildActors) const
{
	const auto Bounds = Cable->GetLocalBounds();
	Origin = Cable->GetComponentLocation();
	BoxExtent = Bounds.BoxExtent;
}

void AResourceTube::OnConnectionDestroyed(AActor* Connection)
{
	DisconnectObjects();
}
