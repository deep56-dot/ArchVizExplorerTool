// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizWidget.h"
#include "ArchVizController.h"


void UArchVizWidget::NativeConstruct()
{
	ModeString->OnSelectionChanged.AddDynamic(this, &UArchVizWidget::ModeChanged);
	LoadOptions->OnSelectionChanged.AddDynamic(this, &UArchVizWidget::LoadOptionChanged);
	SaveButton->OnClicked.AddDynamic(this, &UArchVizWidget::SaveButtonClicked);
	LoadButton->OnClicked.AddDynamic(this, &UArchVizWidget::LoadButtonClicked);
	SaveButtonName->OnClicked.AddDynamic(this, &UArchVizWidget::SaveButtonNameClicked);
}

void UArchVizWidget::SaveButtonClicked() {
	SaveButton->SetVisibility(ESlateVisibility::Collapsed);
	SaveOption->SetVisibility(ESlateVisibility::Visible);
}

void UArchVizWidget::SaveButtonNameClicked() {
	FString Name = SaveName->GetText().ToString();
	ArchVizController->SaveGame(Name);
	SaveOption->SetVisibility(ESlateVisibility::Collapsed);
	SaveButton->SetVisibility(ESlateVisibility::Visible);

}

void UArchVizWidget::LoadButtonClicked() {
	LoadButton->SetVisibility(ESlateVisibility::Collapsed);
	LoadOptions->SetVisibility(ESlateVisibility::Visible);
}

void UArchVizWidget::ModeChanged(FString MString,ESelectInfo::Type SelectionType) {
	ArchVizController->OnModeChanged(MString);
}

void UArchVizWidget::LoadOptionChanged(FString MString, ESelectInfo::Type SelectionType) {
	ArchVizController->LoadGame(MString);
	LoadOptions->SetVisibility(ESlateVisibility::Collapsed);
	LoadButton->SetVisibility(ESlateVisibility::Visible);

}

void UArchVizWidget::SetPlayerController(AArchVizController* PlayerController) {
	ArchVizController = PlayerController;
}

void UArchVizWidget::PopulateComboBox(TArray<FString>& InMapNames)
{
	if (LoadOptions) {
		LoadOptions->ClearOptions();
		for (const FString& MapName : InMapNames)
		{
			LoadOptions->AddOption(MapName);
			GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, MapName);
		}

	}
}