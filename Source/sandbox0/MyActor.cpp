// MyActor.cpp
#include "MyActor.h"
#include "Components/StaticMeshComponent.h"   // UStaticMeshComponent
#include "UObject/ConstructorHelpers.h"       // ConstructorHelpers::FObjectFinder
#include "Engine/StaticMesh.h"                // UStaticMesh

AMyActor::AMyActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the mesh and make it the root so it has a valid attachment parent
    VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualMesh"));
    SetRootComponent(VisualMesh);

    // Use an engine mesh that always exists (no StarterContent dependency)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
        TEXT("/Engine/BasicShapes/Cube.Cube"));

    if (MeshAsset.Succeeded())
    {
        VisualMesh->SetStaticMesh(MeshAsset.Object);
        VisualMesh->SetRelativeLocation(FVector::ZeroVector);
    }
}

void AMyActor::BeginPlay()
{
    Super::BeginPlay();
}

void AMyActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Simple bob + spin to verify it's alive
    FVector NewLocation = GetActorLocation();
    FRotator NewRotation = GetActorRotation();

    const float RunningTime = GetGameTimeSinceCreation();
    const float DeltaHeight = FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime);

    NewLocation.Z += DeltaHeight * 20.0f;   // bob
    NewRotation.Yaw += DeltaTime * 20.0f;   // spin

    SetActorLocationAndRotation(NewLocation, NewRotation);
}
