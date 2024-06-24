// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include <SMaterialSelectionScrollBox.h>
#include "MaterialSelectionScrollBoxWidget.generated.h"

DECLARE_DELEGATE_OneParam(FRoadMaterialController, const FRoadMaterialData& MaterialData);
DECLARE_DELEGATE_OneParam(FDoorMeshController, const FDoorMeshData& DoorMeshData);
DECLARE_DELEGATE_OneParam(FInteriorWallMeshController, const FInteriorWallMeshData& DoorMeshData);
DECLARE_DELEGATE_OneParam(FInteriorFloorMeshController, const FInteriorFloorMeshData& DoorMeshData);
DECLARE_DELEGATE_OneParam(FInteriorRoofMeshController, const FInteriorRoofMeshData& DoorMeshData);
DECLARE_DELEGATE_OneParam(FBuildingMaterialController, const FBuildingMaterialData& MaterialData);


UCLASS()
class ARCHVIZEXPLORER_API UMaterialSelectionScrollBoxWidget : public UWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MaterialData")
	UMaterialDataAsset* MaterialAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	EAssetType MeshType ;

	FRoadMaterialController RoadMaterialController;

	FDoorMeshController DoorMeshController;

	FInteriorWallMeshController InteriorWallMeshController;

	FInteriorFloorMeshController InteriorFloorMeshController;

	FInteriorRoofMeshController InteriorRoofMeshController;

	FBuildingMaterialController BuildingMaterialController;




	
	UFUNCTION()
	void HandleRoadMaterialAssetThubnailSelected(const FRoadMaterialData& MaterialData);
	
	UFUNCTION()
	void HandleDoorMeshAssetThubnailSelected(const FDoorMeshData& DoorMeshData);
	
	UFUNCTION()
	void HandleInteriorWallMeshAssetThubnailSelected(const FInteriorWallMeshData& InteriorWallMeshData);

    UFUNCTION()
	void HandleInteriorFloorMeshAssetThubnailSelected(const FInteriorFloorMeshData& InteriorFloorMeshData);

    UFUNCTION()
	void HandleInteriorRoofMeshAssetThubnailSelected(const FInteriorRoofMeshData& InteriorRoofMeshData);

	UFUNCTION()
	void HandleBuildingMaterialAssetThubnailSelected(const FBuildingMaterialData& MaterialData);


	const FText GetPaletteCategory();
	void ReleaseSlateResources(bool bReleaseChildren);
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
private:

	TSharedPtr<SMaterialSelectionScrollBox> MaterialSelectionScrollBox;
	
};
