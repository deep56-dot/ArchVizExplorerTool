// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadActor.h"

// Sets default values
ARoadActor::ARoadActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMeshRoot = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = ProceduralMeshRoot;
    Material = LoadObject<UMaterialInstance>(nullptr, TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Megascans/Surfaces/Fine_American_Road_sjfnch0a/MI_Fine_American_Road_sjfnch0a_2K.MI_Fine_American_Road_sjfnch0a_2K'"));

}

void ARoadActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
}

void ARoadActor::GenerateRoad(FVector Dimensions)
{
	RoadDimensions = Dimensions;
	TArray<FVector> Vertices;

	float L = Dimensions.X / 2.0f;
	float W = Dimensions.Y / 2.0f;
	float H = Dimensions.Z / 2.0f;

	//Front Face
	Vertices.Add(FVector(-L, -W, -H)); //0
	Vertices.Add(FVector(-L, W, -H)); //1
	Vertices.Add(FVector(-L, W, H)); //2
	Vertices.Add(FVector(-L, -W, H)); //3

	//Bottom Face
	Vertices.Add(FVector(-L, -W, -H)); //4
	Vertices.Add(FVector(L, -W, -H));  //5
	Vertices.Add(FVector(L, W, -H));   //6
	Vertices.Add(FVector(-L, W, -H));  //7

	//Back Face
	Vertices.Add(FVector(L, -W, -H)); //8
	Vertices.Add(FVector(L, -W, H)); //9
	Vertices.Add(FVector(L, W, H)); //10
	Vertices.Add(FVector(L, W, -H)); //11

	//Top Face
	Vertices.Add(FVector(L, -W, H)); //12
	Vertices.Add(FVector(-L, -W, H)); //13
	Vertices.Add(FVector(-L, W, H));  //14
	Vertices.Add(FVector(L, W, H));  //15

	//Left Face
	Vertices.Add(FVector(-L, -W, H)); //16
	Vertices.Add(FVector(L, -W, H)); //17
	Vertices.Add(FVector(L, -W, -H)); //18
	Vertices.Add(FVector(-L, -W, -H)); //19

	//Right Face
	Vertices.Add(FVector(-L, W, H)); //20
	Vertices.Add(FVector(-L, W, -H)); //21 
	Vertices.Add(FVector(L, W, -H)); //22
	Vertices.Add(FVector(L, W, H)); //23

	// Triangles

	TArray<int32> Triangles = { 0,1,3, 1,2,3, 4,5,7, 5,6,7 ,8,9,11, 9,10,11, 12,13,15, 13,14,15, 16,17,19, 17,18,19, 20,21,23, 21,22,23 };

	TArray<FVector2D>  UVs = { FVector2D(0,1), FVector2D(1,1), FVector2D(1,0), FVector2D(0,0),FVector2D(0,0),FVector2D(0,1),FVector2D(1,1),FVector2D(1,0),FVector2D(1,1),FVector2D(1,0),FVector2D(0,0),FVector2D(0,1),FVector2D(0,0),FVector2D(0,1),FVector2D(1,1),FVector2D(1,0),FVector2D(1,0),FVector2D(0,0),FVector2D(0,1),FVector2D(1,1), FVector2D(0,0),FVector2D(0,1),FVector2D(1,1),FVector2D(1,0) };

	TArray<FVector> Normals = {
		FVector(-1,0,0), FVector(-1,0,0), FVector(-1,0,0), FVector(-1,0,0),
		FVector(0,0,-1),FVector(0,0,-1),FVector(0,0,-1),FVector(0,0,-1),
		FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0), FVector(1, 0, 0),
		FVector(0,0,1) ,FVector(0,0,1),FVector(0,0,1),FVector(0,0,1),
		FVector(0,-1,0),FVector(0,-1,0),FVector(0,-1,0),FVector(0,-1,0),
		FVector(0,1,0),FVector(0,1,0),FVector(0,1,0),FVector(0,1,0)
	};
	ProceduralMeshRoot->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	if(Material)
	{
		ProceduralMeshRoot->SetMaterial(0, Material);
	}
}

// Called when the game starts or when spawned
void ARoadActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

