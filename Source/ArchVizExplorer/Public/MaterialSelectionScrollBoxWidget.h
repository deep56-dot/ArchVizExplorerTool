// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include <SMaterialSelectionScrollBox.h>
#include "MaterialSelectionScrollBoxWidget.generated.h"

DECLARE_DELEGATE_OneParam(FRoadMaterialController, const FRoadMaterialData& MaterialData);
DECLARE_DELEGATE_OneParam(FDoorMeshController, const FDoorMeshData& DoorMeshData);

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
	
	UFUNCTION()
	void HandleRoadMaterialAssetThubnailSelected(const FRoadMaterialData& MaterialData);
	
	UFUNCTION()
	void HandleDoorMeshAssetThubnailSelected(const FDoorMeshData& DoorMeshData);


	const FText GetPaletteCategory();
	void ReleaseSlateResources(bool bReleaseChildren);
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
private:

	TSharedPtr<SMaterialSelectionScrollBox> MaterialSelectionScrollBox;
	
};
