// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialSelectionScrollBoxWidget.h"

TSharedRef<SWidget> UMaterialSelectionScrollBoxWidget::RebuildWidget()
{
	MaterialSelectionScrollBox = SNew(SMaterialSelectionScrollBox).InMaterialAsset(MaterialAsset).InMeshType(MeshType);
	
	MaterialSelectionScrollBox->OnRoadMaterialThubnailSelected.BindUObject(this, &UMaterialSelectionScrollBoxWidget::HandleRoadMaterialAssetThubnailSelected);
	MaterialSelectionScrollBox->OnDoorMeshThubnailSelected.BindUObject(this, &UMaterialSelectionScrollBoxWidget::HandleDoorMeshAssetThubnailSelected);

	return MaterialSelectionScrollBox.ToSharedRef();
}



void UMaterialSelectionScrollBoxWidget::HandleRoadMaterialAssetThubnailSelected(const FRoadMaterialData& MeshData)
{
	RoadMaterialController.ExecuteIfBound(MeshData);
}

void  UMaterialSelectionScrollBoxWidget::HandleDoorMeshAssetThubnailSelected(const FDoorMeshData& DoorMeshData) {
	DoorMeshController.ExecuteIfBound(DoorMeshData);
}



const FText UMaterialSelectionScrollBoxWidget::GetPaletteCategory()
{
	return FText::FromString("Panel");
}

void UMaterialSelectionScrollBoxWidget::ReleaseSlateResources(bool bReleaseChildren) {
	Super::ReleaseSlateResources(bReleaseChildren);

	MaterialSelectionScrollBox.Reset();
}
