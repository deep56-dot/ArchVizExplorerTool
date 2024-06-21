// Fill out your copyright notice in the Description page of Project Settings.


#include "RoadWidget.h"
#include "ArchVizController.h"

void URoadWidget::NativeConstruct()
{
	Super::NativeConstruct();
	WidthBox->OnValueChanged.AddDynamic(this, &URoadWidget::OnWidthChanged);
	LocX->OnValueChanged.AddDynamic(this, &URoadWidget::OnLocXChanged);
	LocY->OnValueChanged.AddDynamic(this, &URoadWidget::OnLocYChanged);
	EditorButton->OnClicked.AddDynamic(this, &URoadWidget::OnModeChanged);
	DestroyButton->OnClicked.AddDynamic(this, &URoadWidget::OnDestroyButton);

}
void URoadWidget::OnModeChanged() {
	
	if (ArchVizController) {
		ArchVizController->EditorMode();
	}
}
void URoadWidget::OnDestroyButton()
{

	if (ArchVizController) {
		if(ArchVizController->CurrRoadActor)
		{
			ArchVizController->CurrRoadActor->Destroy();
		}
	}
}
void URoadWidget::OnWidthChanged(float width)
{
	if (ArchVizController) {
		ArchVizController->OnRoadWidthChanged(width);
	}
}
void URoadWidget::SetPlayerController(AArchVizController* PlayerController) {
	ArchVizController = PlayerController;

}
void URoadWidget::OnLocXChanged(float LocationX) {
	if (ArchVizController) {
		ArchVizController->OnRoadLocXChanged(LocationX);
	}
}
void URoadWidget::OnLocYChanged(float LocationY)
{
	if (ArchVizController) {
		ArchVizController->OnRoadLocYChanged(LocationY);
	}
}


