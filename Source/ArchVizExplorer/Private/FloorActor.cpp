// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorActor.h"

AFloorActor::AFloorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

    ProceduralMeshFloor= CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Floor"));
    ProceduralMeshFloor->SetupAttachment(Scene);

    ProceduralMeshRoof= CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Roof"));
    ProceduralMeshRoof->SetupAttachment(Scene);
}

void AFloorActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void AFloorActor::GenerateFloor(FVector Dimensions)
{
    TypeOfActor = "Floor";
    FDimentions = Dimensions;

   /* ProceduralMeshRoot->RegisterComponentWithWorld(GetWorld());*/
    ProceduralMeshFloor->ClearAllMeshSections();

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    // Front
    Vertices.Add(FVector(0, Dimensions.Y, 0));
    Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

    // Back
    Vertices.Add(FVector(0, 0, 0));
    Vertices.Add(FVector(0, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, 0, 0));

    // Left
    Vertices.Add(FVector(0, 0, 0));
    Vertices.Add(FVector(0, 0, Dimensions.Z));
    Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(0, Dimensions.Y, 0));

    // Right
    Vertices.Add(FVector(Dimensions.X, 0, 0));
    Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

    // Top
    Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(0, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));

    // Bottom
    Vertices.Add(FVector(0, Dimensions.Y, 0));
    Vertices.Add(FVector(0, 0, 0));
    Vertices.Add(FVector(Dimensions.X, 0, 0));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

    // Triangles (two triangles per face)
    Triangles.Append({
        0, 3, 1, 1, 3, 2,
        4, 5, 7, 5, 6, 7,
        8, 11, 9, 9, 11, 10,
        12, 13, 15, 13, 14, 15,
        16, 19, 17, 17, 19, 18,
        20, 21, 23, 21, 22, 23
        });

    // Normals (perpendicular to each face)
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(0, 1, 0));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(0, -1, 0));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(-1, 0, 0));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(1, 0, 0));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(0, 0, 1));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(0, 0, -1));
    }

    // UV Mapping
    for (int32 i{}; i < 3; ++i)
    {
        UVs.Add(FVector2D(0, 1));
        UVs.Add(FVector2D(0, 0));
        UVs.Add(FVector2D(1, 0));
        UVs.Add(FVector2D(1, 1));

        UVs.Add(FVector2D(0, 1));
        UVs.Add(FVector2D(0, 0));
        UVs.Add(FVector2D(-1, 0));
        UVs.Add(FVector2D(-1, 1));
    }

    ProceduralMeshFloor->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
}

void AFloorActor::GenerateRoof(FVector Dimensions)
{
    TypeOfActor = "Roof";
    FDimentions = Dimensions;
  
    ProceduralMeshRoof->ClearAllMeshSections();

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> VertexColors;
    TArray<FProcMeshTangent> Tangents;

    // Front
    Vertices.Add(FVector(0, Dimensions.Y, 0));
    Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

    // Back
    Vertices.Add(FVector(0, 0, 0));
    Vertices.Add(FVector(0, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, 0, 0));

    // Left
    Vertices.Add(FVector(0, 0, 0));
    Vertices.Add(FVector(0, 0, Dimensions.Z));
    Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(0, Dimensions.Y, 0));

    // Right
    Vertices.Add(FVector(Dimensions.X, 0, 0));
    Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

    // Top
    Vertices.Add(FVector(0, Dimensions.Y, Dimensions.Z));
    Vertices.Add(FVector(0, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, 0, Dimensions.Z));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, Dimensions.Z));

    // Bottom
    Vertices.Add(FVector(0, Dimensions.Y, 0));
    Vertices.Add(FVector(0, 0, 0));
    Vertices.Add(FVector(Dimensions.X, 0, 0));
    Vertices.Add(FVector(Dimensions.X, Dimensions.Y, 0));

    // Triangles (two triangles per face)
    Triangles.Append({
        0, 3, 1, 1, 3, 2,
        4, 5, 7, 5, 6, 7,
        8, 11, 9, 9, 11, 10,
        12, 13, 15, 13, 14, 15,
        16, 19, 17, 17, 19, 18,
        20, 21, 23, 21, 22, 23
        });

    // Normals (perpendicular to each face)
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(0, 1, 0));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(0, -1, 0));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(-1, 0, 0));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(1, 0, 0));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(0, 0, 1));
    }
    for (int32 i{}; i < 4; i++)
    {
        Normals.Add(FVector(0, 0, -1));
    }

    // UV Mapping
    for (int32 i{}; i < 3; ++i)
    {
        UVs.Add(FVector2D(0, 1));
        UVs.Add(FVector2D(0, 0));
        UVs.Add(FVector2D(1, 0));
        UVs.Add(FVector2D(1, 1));

        UVs.Add(FVector2D(0, 1));
        UVs.Add(FVector2D(0, 0));
        UVs.Add(FVector2D(-1, 0));
        UVs.Add(FVector2D(-1, 1));
    }

    ProceduralMeshRoof->CreateMeshSection_LinearColor(
        0, Vertices, Triangles, Normals, UVs,
        TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true
    );

   
}

// Called when the game starts or when spawned
void AFloorActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFloorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


