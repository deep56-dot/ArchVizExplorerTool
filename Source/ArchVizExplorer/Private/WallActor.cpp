// Fill out your copyright notice in the Description page of Project Settings.


#include "WallActor.h"

// Sets default values
AWallActor::AWallActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("rootscene"));
	RootComponent = scene;

	
}

// Called when the game starts or when spawned
void AWallActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallActor::DestroyWalls() {
	for (int i = 0; i < StaticMeshComponentArr.Num(); i++) {
		if (StaticMeshComponentArr[i]) {
			StaticMeshComponentArr[i]->DestroyComponent();
			StaticMeshComponentArr[i] = nullptr;
		}
	}
	StaticMeshComponentArr.Empty();
}

void AWallActor::GenerateWall(int NoOfSegents)
{
	DestroyWalls();
	NoOfSegments = NoOfSegents;
	for (int i = 0; i < NoOfSegents; i++) {
		UStaticMeshComponent* Component = NewObject<UStaticMeshComponent>(this);
		
		if (Component) {
			float length = StaticMesh->GetBounds().GetBox().GetSize().X;
			float Height = StaticMesh->GetBounds().GetBox().GetSize().Z;
			Component->SetStaticMesh(StaticMesh);
			Component->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
			Component->RegisterComponent();
			Component->SetRelativeLocation(FVector(i * length, 0,0));
			StaticMeshComponentArr.Add(Component);
		}
	}
}

