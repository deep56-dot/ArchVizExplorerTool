// Fill out your copyright notice in the Description page of Project Settings.


#include "TemplateActor.h"

// Sets default values
ATemplateActor::ATemplateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = Scene;
}

// Called when the game starts or when spawned
void ATemplateActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATemplateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

