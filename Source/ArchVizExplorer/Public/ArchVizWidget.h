// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/CanvasPanel.h>
#include <Components/Button.h>
#include <Components/ComboBoxString.h>
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
	void ModeChanged(FString MString, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void SetPlayerController(class AArchVizController* PlayerController);

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ModeString;

	UPROPERTY(meta = (BindWidget))
	UButton* SaveButton;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadButton;



	UPROPERTY()
	class AArchVizController* ArchVizController;
};
