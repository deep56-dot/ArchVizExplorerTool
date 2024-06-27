// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ArchVizSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FRoadActorData {

	GENERATED_BODY()

	UPROPERTY()
	FVector Dimenstion;

	UPROPERTY()
	FTransform RoadTransform;

	UPROPERTY()
	UMaterialInterface* Material;
};

USTRUCT(BlueprintType)
struct FWallActorData {

	GENERATED_BODY()

	UPROPERTY()
	FTransform WallTransform;

	UPROPERTY()
	TArray<UStaticMesh*> WallWarray;

	UPROPERTY()
	UMaterialInterface* Material;

};



USTRUCT(BlueprintType)
struct FFloorActorData {

	GENERATED_BODY()

	UPROPERTY()
	FTransform FloorTransform;

	UPROPERTY()
	FString ActorType;

	UPROPERTY()
	FVector Dimenstion;

	UPROPERTY()
	UMaterialInterface* Material;

};

USTRUCT(BlueprintType)
struct FInteriorActorData {

	GENERATED_BODY()

	UPROPERTY()
	FTransform InteriorTransform;

	UPROPERTY()
	UStaticMesh* StaticMesh;

	UPROPERTY()
	FString InteriorType;


};
UCLASS()
class ARCHVIZEXPLORER_API UArchVizSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FRoadActorData> RoadActorArray;

	UPROPERTY()
	TArray<FWallActorData> WallActorArray;

	UPROPERTY()
	TArray<FFloorActorData> FloorActorArray;

	UPROPERTY()
	TArray<FInteriorActorData> InteriorActorArray;

};
