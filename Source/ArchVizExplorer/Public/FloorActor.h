// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ProceduralMeshComponent.h>
#include "FloorActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AFloorActor : public AActor
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Sets default values for this actor's properties
	AFloorActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void GenerateFloor(FVector Dimensions,FName Name);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	FVector Dimentions;

	UPROPERTY()
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	UProceduralMeshComponent* ProceduralMeshRoot;

};
