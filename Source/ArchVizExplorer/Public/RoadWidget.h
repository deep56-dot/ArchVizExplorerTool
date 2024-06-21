// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include <Components/SpinBox.h>
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/HorizontalBox.h>
#include <Components/Border.h>
#include <MaterialSelectionScrollBoxWidget.h>
#include "RoadWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API URoadWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;

public:

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	USpinBox* WidthBox;

	UPROPERTY(meta = (BindWidget))
	USpinBox* LocX;

	UPROPERTY(meta = (BindWidget))
	USpinBox* LocY;

	UPROPERTY(meta = (BindWidget))
	UButton* EditorButton;

	UPROPERTY(meta = (BindWidget))
	UButton* DestroyButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EditorButtonText;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border_L;

	UPROPERTY(meta = (BindWidget))
	UBorder* Border_W;

	UPROPERTY(meta = (BindWidget))
	UMaterialSelectionScrollBoxWidget* MaterialBox;

	UFUNCTION()
	void OnModeChanged();

	UFUNCTION()
	void OnDestroyButton();

	UFUNCTION()
	void OnLocXChanged(float LocationX);

	UFUNCTION()
	void OnLocYChanged(float LocationY);

	UFUNCTION()
	void OnWidthChanged(float width);

	UFUNCTION()
	void SetPlayerController(class AArchVizController* PlayerController);
private:
	UPROPERTY()
	class AArchVizController* ArchVizController;
};
