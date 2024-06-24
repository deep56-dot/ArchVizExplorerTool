// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialSelectionScrollBoxWidget.h"

TSharedRef<SWidget> UMaterialSelectionScrollBoxWidget::RebuildWidget()
{
	MaterialSelectionScrollBox = SNew(SMaterialSelectionScrollBox).InMaterialAsset(MaterialAsset).InMeshType(MeshType);
	
	MaterialSelectionScrollBox->OnRoadMaterialThubnailSelected.BindUObject(this, &UMaterialSelectionScrollBoxWidget::HandleRoadMaterialAssetThubnailSelected);
	MaterialSelectionScrollBox->OnDoorMeshThubnailSelected.BindUObject(this, &UMaterialSelectionScrollBoxWidget::HandleDoorMeshAssetThubnailSelected);
	MaterialSelectionScrollBox->OnInteriorWallMeshThubnailSelected.BindUObject(this, &UMaterialSelectionScrollBoxWidget::HandleInteriorWallMeshAssetThubnailSelected);
	MaterialSelectionScrollBox->OnInteriorRoofMeshThubnailSelected.BindUObject(this, &UMaterialSelectionScrollBoxWidget::HandleInteriorRoofMeshAssetThubnailSelected);
	MaterialSelectionScrollBox->OnInteriorFloorMeshThubnailSelected.BindUObject(this, &UMaterialSelectionScrollBoxWidget::HandleInteriorFloorMeshAssetThubnailSelected);
	MaterialSelectionScrollBox->OnBuildingMaterialThubnailSelected.BindUObject(this, &UMaterialSelectionScrollBoxWidget::HandleBuildingMaterialAssetThubnailSelected);

	return MaterialSelectionScrollBox.ToSharedRef();
}



void UMaterialSelectionScrollBoxWidget::HandleRoadMaterialAssetThubnailSelected(const FRoadMaterialData& MeshData)
{
	RoadMaterialController.ExecuteIfBound(MeshData);
}

void  UMaterialSelectionScrollBoxWidget::HandleDoorMeshAssetThubnailSelected(const FDoorMeshData& DoorMeshData) {
	DoorMeshController.ExecuteIfBound(DoorMeshData);
}

void  UMaterialSelectionScrollBoxWidget::HandleInteriorWallMeshAssetThubnailSelected(const FInteriorWallMeshData& InteriorWallMeshData) {
	InteriorWallMeshController.ExecuteIfBound(InteriorWallMeshData);
}

void  UMaterialSelectionScrollBoxWidget::HandleInteriorRoofMeshAssetThubnailSelected(const FInteriorRoofMeshData& InteriorWallMeshData) {
	InteriorRoofMeshController.ExecuteIfBound(InteriorWallMeshData);
}

void  UMaterialSelectionScrollBoxWidget::HandleInteriorFloorMeshAssetThubnailSelected(const FInteriorFloorMeshData& InteriorWallMeshData) {
	InteriorFloorMeshController.ExecuteIfBound(InteriorWallMeshData);
}

void UMaterialSelectionScrollBoxWidget::HandleBuildingMaterialAssetThubnailSelected(const FBuildingMaterialData& MeshData)
{
	BuildingMaterialController.ExecuteIfBound(MeshData);
}



const FText UMaterialSelectionScrollBoxWidget::GetPaletteCategory()
{
	return FText::FromString("Panel");
}

void UMaterialSelectionScrollBoxWidget::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);

	MaterialSelectionScrollBox.Reset();
}
