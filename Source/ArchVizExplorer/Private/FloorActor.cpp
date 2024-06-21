// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorActor.h"

AFloorActor::AFloorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshRoot = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMeshRoot;


}

void AFloorActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

void AFloorActor::GenerateFloor(FVector Dimensions)
{
    // Array to hold the vertices of the mesh
    TArray<FVector> Vertices;

    float L = Dimensions.X / 2.0f;
    float W = Dimensions.Y / 2.0f;
    float H = Dimensions.Z / 2.0f;

    // Offset all vertices by the amount to shift the origin to the bottom-front-left corner
    FVector Offset = FVector(L, W, H);

    // Front Face
    Vertices.Add(FVector(-L, -W, -H) + Offset); // 0
    Vertices.Add(FVector(-L, W, -H) + Offset);  // 1
    Vertices.Add(FVector(-L, W, H) + Offset);   // 2
    Vertices.Add(FVector(-L, -W, H) + Offset);  // 3

    // Bottom Face
    Vertices.Add(FVector(-L, -W, -H) + Offset); // 4
    Vertices.Add(FVector(L, -W, -H) + Offset);  // 5
    Vertices.Add(FVector(L, W, -H) + Offset);   // 6
    Vertices.Add(FVector(-L, W, -H) + Offset);  // 7

    // Back Face
    Vertices.Add(FVector(L, -W, -H) + Offset);  // 8
    Vertices.Add(FVector(L, -W, H) + Offset);   // 9
    Vertices.Add(FVector(L, W, H) + Offset);    // 10
    Vertices.Add(FVector(L, W, -H) + Offset);   // 11

    // Top Face
    Vertices.Add(FVector(L, -W, H) + Offset);   // 12
    Vertices.Add(FVector(-L, -W, H) + Offset);  // 13
    Vertices.Add(FVector(-L, W, H) + Offset);   // 14
    Vertices.Add(FVector(L, W, H) + Offset);    // 15

    // Left Face
    Vertices.Add(FVector(-L, -W, H) + Offset);  // 16
    Vertices.Add(FVector(L, -W, H) + Offset);   // 17
    Vertices.Add(FVector(L, -W, -H) + Offset);  // 18
    Vertices.Add(FVector(-L, -W, -H) + Offset); // 19

    // Right Face
    Vertices.Add(FVector(-L, W, H) + Offset);   // 20
    Vertices.Add(FVector(-L, W, -H) + Offset);  // 21
    Vertices.Add(FVector(L, W, -H) + Offset);   // 22
    Vertices.Add(FVector(L, W, H) + Offset);    // 23

    // Triangles
    TArray<int32> Triangles = {
        0, 1, 3, 1, 2, 3,    // Front
        4, 5, 7, 5, 6, 7,    // Bottom
        8, 9, 11, 9, 10, 11, // Back
        12, 13, 15, 13, 14, 15, // Top
        16, 17, 19, 17, 18, 19, // Left
        20, 21, 23, 21, 22, 23  // Right
    };

    // UVs
    TArray<FVector2D> UVs = {
        FVector2D(0,1), FVector2D(1,1), FVector2D(1,0), FVector2D(0,0),
        FVector2D(0,0), FVector2D(0,1), FVector2D(1,1), FVector2D(1,0),
        FVector2D(1,1), FVector2D(1,0), FVector2D(0,0), FVector2D(0,1),
        FVector2D(0,0), FVector2D(0,1), FVector2D(1,1), FVector2D(1,0),
        FVector2D(1,0), FVector2D(0,0), FVector2D(0,1), FVector2D(1,1),
        FVector2D(0,0), FVector2D(0,1), FVector2D(1,1), FVector2D(1,0)
    };

    // Normals
    TArray<FVector> Normals = {
        FVector(-1,0,0), FVector(-1,0,0), FVector(-1,0,0), FVector(-1,0,0), // Front
        FVector(0,0,-1), FVector(0,0,-1), FVector(0,0,-1), FVector(0,0,-1), // Bottom
        FVector(1,0,0), FVector(1,0,0), FVector(1,0,0), FVector(1,0,0), // Back
        FVector(0,0,1), FVector(0,0,1), FVector(0,0,1), FVector(0,0,1), // Top
        FVector(0,-1,0), FVector(0,-1,0), FVector(0,-1,0), FVector(0,-1,0), // Left
        FVector(0,1,0), FVector(0,1,0), FVector(0,1,0), FVector(0,1,0)  // Right
    };

    ProceduralMeshRoot->CreateMeshSection_LinearColor(
        0, Vertices, Triangles, Normals, UVs,
        TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true
    );

	/*if (Material)
	{
		ProceduralMeshRoot->SetMaterial(0, Material);
	}*/
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


