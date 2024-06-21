// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizWidget.h"
#include "ArchVizController.h"


void UArchVizWidget::NativeConstruct()
{
	ModeString->OnSelectionChanged.AddDynamic(this, &UArchVizWidget::ModeChanged);
}

void UArchVizWidget::ModeChanged(FString MString,ESelectInfo::Type SelectionType) {
	ArchVizController->OnModeChanged(MString);
}

void UArchVizWidget::SetPlayerController(AArchVizController* PlayerController) {
	ArchVizController = PlayerController;
}