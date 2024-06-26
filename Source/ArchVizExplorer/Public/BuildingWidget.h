// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include <Components/SpinBox.h>
#include <Components/Button.h>
#include <Components/Border.h>
#include <MaterialSelectionScrollBoxWidget.h>

#include <Components/VerticalBox.h>
#include <Components/TextBlock.h>
#include "BuildingWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UBuildingWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;


public:
	UFUNCTION()
	void SegmentsChanged(float Segments);

	UFUNCTION()
	void FloorLengthChanged(float Length);

	UFUNCTION()
	void FloorWidthChanged(float Width);

	UFUNCTION()
	void WallButtonClicked();

	UFUNCTION()
	void FloorButtonClicked();
	
	UFUNCTION()
	void RoofButtonClicked();
	
	UFUNCTION()
	void DoorButtonClicked();

	UFUNCTION()
	void ModifyComponentButtonClicked();


	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	USpinBox* NoOfSegments;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border_Seg;	
	
	UPROPERTY(meta = (BindWidget))
	UBorder* Border_FloorL;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* Border_FloorW;

	UPROPERTY(meta = (BindWidget))
	USpinBox* FLength;
	
	UPROPERTY(meta = (BindWidget))
	USpinBox* FWidth;

	UPROPERTY(meta = (BindWidget))
	UButton* WallButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DoorButton;

	UPROPERTY(meta = (BindWidget))
	UButton* FloorButton;

	UPROPERTY(meta = (BindWidget))
	UButton* RotateFloorButton;

	

	UPROPERTY(meta = (BindWidget))
	UButton* RoofButton;	
	
	UPROPERTY(meta = (BindWidget))
	UButton* ModifyComponentButton;

	UPROPERTY(meta = (BindWidget))
	UMaterialSelectionScrollBoxWidget* DoorMeshBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox_Modify;


	UPROPERTY(meta = (BindWidget))
	USpinBox* XOffset;

	UPROPERTY(meta = (BindWidget))
	USpinBox* YOffset;

	UPROPERTY(meta = (BindWidget))
	USpinBox* ZOffset;

	UPROPERTY(meta = (BindWidget))
	UButton* MoveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DestroyButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OpenDoorButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DoorToggle;

	UFUNCTION()
	void SetPlayerController(class AArchVizController* PlayerController);

private:
	UPROPERTY()
	class AArchVizController* ArchVizController;
	
};
