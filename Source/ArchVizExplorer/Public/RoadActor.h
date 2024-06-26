// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ProceduralMeshComponent.h>
#include "RoadActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API ARoadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoadActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	

	UFUNCTION(BlueprintCallable)
	void GenerateRoad(FVector Dimensions);



protected:
	

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	FVector Dimentions; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	UProceduralMeshComponent* ProceduralMeshRoot; 

	UPROPERTY()
	int count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	UMaterialInstance* Material; 

	UPROPERTY()
	FVector RoadDimensions;
	
};
