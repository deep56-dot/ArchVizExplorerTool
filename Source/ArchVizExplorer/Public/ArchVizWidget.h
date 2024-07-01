// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include <Components/Button.h>
#include <Components/Border.h>
#include <Components/TextBlock.h>
#include <Components/ComboBoxString.h>
#include <Components/EditableText.h>
#include "ArchVizWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHVIZEXPLORER_API UArchVizWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;


public:
	UFUNCTION()
	void SaveButtonClicked();
	UFUNCTION()
	void NewHouseButtonClicked();
	UFUNCTION()
	void SaveButtonNameClicked();
	UFUNCTION()
	void LoadButtonClicked();
	UFUNCTION()
	void ModeChanged(FString MString, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void LoadOptionChanged(FString MString, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void SetPlayerController(class AArchVizController* PlayerController);

	UFUNCTION()
	void PopulateComboBox(TArray<FString>& InMapNames);

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanelStart;

	UPROPERTY(meta = (BindWidget))
	UButton* NewHouseButton;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ModeString;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;	
	
	UPROPERTY(meta = (BindWidget))
	UButton* InstructionButton;

	UPROPERTY(meta = (BindWidget))
	UBorder* InstructionBox;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* CustomTextBorder;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LeftMouseText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CustomText;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* LoadOptions;

	UPROPERTY(meta = (BindWidget))
	UBorder* SaveOption;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButtonName;
	
	UPROPERTY(meta = (BindWidget))
	UEditableText* SaveName;

	UPROPERTY()
	class AArchVizController* ArchVizController;
};
