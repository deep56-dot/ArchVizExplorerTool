// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteriorActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AInteriorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteriorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	USceneComponent* SceneComponent;

	UPROPERTY()
	UStaticMesh* InteriorStaticMesh;

	UPROPERTY()
	FString TypeOfInterior;

	void GenerateInterior(UStaticMesh* StaticMesh);

};
