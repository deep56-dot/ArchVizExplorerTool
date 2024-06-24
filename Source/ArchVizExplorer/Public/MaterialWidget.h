// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <MaterialSelectionScrollBoxWidget.h>
#include <Components/CanvasPanel.h>
#include "MaterialWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UMaterialWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
public:

	UPROPERTY(meta = (BindWidget))
	UMaterialSelectionScrollBoxWidget* MaterialBox;	
	
	UPROPERTY(meta = (BindWidget))
	UMaterialSelectionScrollBoxWidget* BuildingMaterialBox;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;
	
};
