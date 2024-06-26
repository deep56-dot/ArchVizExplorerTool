// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorActor.h"

// Sets default values
AInteriorActor::AInteriorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneCompnent"));
	RootComponent = SceneComponent;
}

// Called when the game starts or when spawned
void AInteriorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteriorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteriorActor::GenerateInterior(UStaticMesh* StaticMesh)
{
	//InteriorStaticMesh = StaticMesh;
	InteriorStaticMesh = StaticMesh;
	UStaticMeshComponent* StaticMeshComponnet = NewObject<UStaticMeshComponent>(this);
	StaticMeshComponnet->SetStaticMesh(StaticMesh);
	StaticMeshComponnet->RegisterComponentWithWorld(GetWorld());
	StaticMeshComponnet->AttachToComponent(SceneComponent,FAttachmentTransformRules::KeepRelativeTransform);

}

