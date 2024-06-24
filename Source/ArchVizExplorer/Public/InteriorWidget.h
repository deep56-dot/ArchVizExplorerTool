// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <MaterialSelectionScrollBoxWidget.h>
#include <Components/CanvasPanel.h>
#include "InteriorWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UInteriorWidget : public UUserWidget
{
	GENERATED_BODY()
public:


	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UMaterialSelectionScrollBoxWidget* FloorMeshesScrollBox;	
	
	UPROPERTY(meta = (BindWidget))
	UMaterialSelectionScrollBoxWidget* WallMeshesScrollBox;	
	
	UPROPERTY(meta = (BindWidget))
	UMaterialSelectionScrollBoxWidget* RoofMeshesScrollBox;
	
};
