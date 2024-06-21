// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingWidget.h"
#include "ArchVizController.h"


void UBuildingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	NoOfSegments->OnValueChanged.AddDynamic(this, &UBuildingWidget::SegmentsChanged);
	WallButton->OnClicked.AddDynamic(this, &UBuildingWidget::WallButtonClicked);
	RoofButton->OnClicked.AddDynamic(this, &UBuildingWidget::RoofButtonClicked);
	FloorButton->OnClicked.AddDynamic(this, &UBuildingWidget::FloorButtonClicked);
	DoorButton->OnClicked.AddDynamic(this, &UBuildingWidget::DoorButtonClicked);

	FLength->OnValueChanged.AddDynamic(this, &UBuildingWidget::FloorLengthChanged);
	FWidth->OnValueChanged.AddDynamic(this, &UBuildingWidget::FloorWidthChanged);

	ModifyComponentButton->OnClicked.AddDynamic(this, &UBuildingWidget::ModifyComponentButtonClicked);


}

void UBuildingWidget::SegmentsChanged(float Segments)
{
	if (ArchVizController) {
		ArchVizController->OnWallSegmentsChanged(Segments);
	}
}

void UBuildingWidget::FloorLengthChanged(float Length)
{


	if (ArchVizController) {
	
		ArchVizController->OnFloorLengthChanged(Length);
	}
}

void UBuildingWidget::FloorWidthChanged(float Width)
{
	if (ArchVizController) {
		ArchVizController->OnFloorWidthChanged(Width);
	}
}

void UBuildingWidget::WallButtonClicked()
{
	if (ArchVizController) {
		ArchVizController->OnWallButtonClicked();
	}
}

void UBuildingWidget::FloorButtonClicked()
{
	if (ArchVizController) {
		ArchVizController->OnFloorButtonClicked();
	}
}

void UBuildingWidget::RoofButtonClicked()
{
	if (ArchVizController) {
		ArchVizController->OnRoofButtonClicked();
	}
}

void UBuildingWidget::DoorButtonClicked()
{
	if (ArchVizController) {
		ArchVizController->OnDoorButtonClicked();
	}
}

void UBuildingWidget::ModifyComponentButtonClicked()
{
	if (ArchVizController) {
		ArchVizController->OnModifyComponentButtonClicked();
	}
}



void UBuildingWidget::SetPlayerController(AArchVizController* PlayerController)
{
	ArchVizController = PlayerController;
}
