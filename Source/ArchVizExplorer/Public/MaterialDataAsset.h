// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MaterialDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FInteriorFloorMeshData {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FString InteriorFloorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* InteriorFloorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* InteriorFloorThumbnail;
};

USTRUCT(BlueprintType)
struct FInteriorRoofMeshData {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FString InteriorRoofName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* InteriorRoofMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* InteriorRoofThumbnail;
};

USTRUCT(BlueprintType)
struct FInteriorWallMeshData {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FString InteriorWallName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* InteriorWallMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* InteriorWallThumbnail;
};

USTRUCT(BlueprintType)
struct FDoorMeshData {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FString DoorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* DoorMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* DoorThumbnail;
};

USTRUCT(BlueprintType)
struct FRoadMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FString RoadName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstance* RoadMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UTexture2D* RoadThumbnail;
};

USTRUCT(BlueprintType)
struct FBuildingMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FString BuildingName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstance* BuildingMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UTexture2D* BuildingThumbnail;
};

USTRUCT(BlueprintType)
struct FInteriorMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	FString InteriorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UMaterialInstance* InteriorMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	UTexture2D* InteriorThumbnail;
};

UCLASS()
class ARCHVIZEXPLORER_API UMaterialDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FInteriorFloorMeshData> InteriorFloorMeshDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FInteriorMaterialData> InteriorMaterialDataArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FInteriorRoofMeshData> InteriorRoofMeshDataArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FInteriorWallMeshData> InteriorWallMeshDataArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FDoorMeshData> DoorMeshDataArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FRoadMaterialData> RoadMaterialDataArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FBuildingMaterialData> BuildingMaterialDataArray;
	
};
