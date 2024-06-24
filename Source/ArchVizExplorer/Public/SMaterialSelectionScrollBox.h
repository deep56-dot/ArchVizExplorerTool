// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <MaterialDataAsset.h>
#include <Widgets/Layout/SScrollBox.h>
#include <Brushes/SlateColorBrush.h>
#include "Widgets/SCompoundWidget.h"


DECLARE_DELEGATE_OneParam(FOnRoadMaterialThubnailSelected, const FRoadMaterialData&);
DECLARE_DELEGATE_OneParam(FOnDoorMeshThubnailSelected, const FDoorMeshData&);
DECLARE_DELEGATE_OneParam(FOnInteriorWallMeshThubnailSelected, const FInteriorWallMeshData&);
DECLARE_DELEGATE_OneParam(FOnInteriorFloorMeshThubnailSelected, const FInteriorFloorMeshData&);
DECLARE_DELEGATE_OneParam(FOnInteriorRoofMeshThubnailSelected, const FInteriorRoofMeshData&);
DECLARE_DELEGATE_OneParam(FOnBuildingMaterialThubnailSelected, const FBuildingMaterialData&);

UENUM()
enum class EAssetType {
	InteriorMaterialData,
	BuildingMaterialData,
	RoadMaterialData,
	DoorMeshData,
	InteriorWallMeshData,
	InteriorRoofMeshData,
	InteriorFloorMeshData
};

class ARCHVIZEXPLORER_API SMaterialSelectionScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMaterialSelectionScrollBox){}
	SLATE_ARGUMENT(TWeakObjectPtr < UMaterialDataAsset>, InMaterialAsset)
	SLATE_ARGUMENT(EAssetType, InMeshType)
	SLATE_END_ARGS()


	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);


	FOnRoadMaterialThubnailSelected OnRoadMaterialThubnailSelected;
	FOnDoorMeshThubnailSelected OnDoorMeshThubnailSelected;
	FOnInteriorWallMeshThubnailSelected OnInteriorWallMeshThubnailSelected;
	FOnInteriorFloorMeshThubnailSelected OnInteriorFloorMeshThubnailSelected;
	FOnInteriorRoofMeshThubnailSelected OnInteriorRoofMeshThubnailSelected;
	FOnBuildingMaterialThubnailSelected OnBuildingMaterialThubnailSelected;


private:
	
	void RefreshAssetRoadMaterialThubnail();
	void RefreshAssetDoorMeshThubnail();

	void RefreshAssetInteriorWallMeshThubnail();
	void RefreshAssetInteriorFloorMeshThubnail();
	void RefreshAssetInteriorRoofMeshThubnail();

	void RefreshAssetBuildingMaterialThubnail();


	TWeakObjectPtr <UMaterialDataAsset> MaterialAsset;
	EAssetType MeshType;
	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SVerticalBox> ParentVBox;


};
