// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include <RoadActor.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "Kismet/GameplayStatics.h"
#include <ArchVizWidget.h>
#include <RoadWidget.h>
#include <WallActor.h>
#include <BuildingWidget.h>
#include <FloorActor.h>
#include <MaterialWidget.h>
#include <InteriorActor.h>
#include <InteriorWidget.h>
#include "ArchVizSaveGame.h"
#include "ArchVizController.generated.h"


UENUM(BlueprintType) 
enum class EBuildingComponentType {
	None,
	Wall,
	Door,
	Floor,
	Roof

};

//UENUM(BlueprintType) 
//enum class EInteriorComponentType {
//	Wall,
//	Floor,
//	Roof
//};
UCLASS()
class ARCHVIZEXPLORER_API AArchVizController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AArchVizController();

	virtual void SetupInputComponent() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProcMesh")
    ARoadActor*  RoadActor;

	virtual void BeginPlay() override;

	UPROPERTY()
	ULocalPlayer* LocalPlayer;

	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* SubSystem;

	UPROPERTY()
	 UInputMappingContext* RoadMappingContext;

	/** Action to update location. */
	 UPROPERTY()
	 UInputAction* RoadLeftClickAction;	
	 
	 UPROPERTY()
	 UInputAction* RoadRightClickAction;

	 UPROPERTY()
	 ARoadActor* CurrRoadActor;

	 UPROPERTY()
	 bool bFirstClick;

	 UPROPERTY()
	 bool bFirstRoad;

	 UPROPERTY()
	 bool bEditorMode;

	 UPROPERTY()
	 FVector  PrevClickLocation;

	 UPROPERTY()
	 FVector FirstClickLocation;

	 UPROPERTY()
	 FVector SecondClickLocation;

	 UFUNCTION()
	 void RoadLeftClick(); 

	 UFUNCTION()
	 void RoadMateialApply(const FRoadMaterialData& MaterialData);

	 UFUNCTION()
	 void BuildingMateialApply(const FBuildingMaterialData& MaterialData);
	 
	 UFUNCTION()
	 void RoadRightClick();

	 UFUNCTION()
	 void OnRoadWidthChanged(float Width); 

	 UFUNCTION()
	 void OnRoadLocXChanged(float LocationX);
	 
	 UFUNCTION()
	 void OnRoadLocYChanged(float LocationY);



	 UFUNCTION()
	 void EditorMode();


	 UFUNCTION()
	 void OnModeChanged(FString Mode);

	 UFUNCTION()
	 float GetAngle(FVector V1, FVector V2);

	 UFUNCTION()
	 bool OnRightOrleft(FVector FirstPoint, FVector SecondPoint, FVector Prev);

	 UFUNCTION()
	 void SnapActor(float SnapValue,AActor* CurrActor);

	/* float GetAngle(FVector V1, FVector V2);

	 bool OnRightOrleft(FVector FirstPoint, FVector SecondPoint, FVector Prev);*/

	 UPROPERTY()
	 URoadWidget* RoadWidgetInstance;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<URoadWidget> RoadWidget;

	 UPROPERTY()
	 AWallActor* WallActorInstance;

	 UPROPERTY(EditAnywhere,BlueprintReadWrite)
	 TSubclassOf<AWallActor> WallActor;

	 UPROPERTY()
	 UMaterialInterface* CurrWallMaterial;

	 UPROPERTY()
	 bool bWallMove;

	 UPROPERTY()
	 UInputMappingContext* WallMappingContext;

	 /** Action to update location. */
	 UPROPERTY()
	 UInputAction* WallLeftClickAction;
	 
	 UPROPERTY()
	 UInputAction* WallRClickAction;	 
	 UPROPERTY()
	 UInputAction* WallSClickAction;

	 UFUNCTION()
	 void WallLeftClick();
	 
	 UFUNCTION()
	 void WallRClick(); 
	 


	 UFUNCTION()
	 void OnWallSegmentsChanged(float Segments);

	 UPROPERTY()
	 AActor* CurrOffsetActor;
	 
	 /*UPROPERTY()
	 bool bMove_ModifyMode;*/

	 UFUNCTION()
	 void XoffsetChanged(float off);

	 UFUNCTION()
	 void YoffsetChanged(float off);

	 UFUNCTION()
	 void ZoffsetChanged(float off);

	 UFUNCTION()
	 void OnMoveButtonClicked();

	 UFUNCTION()
	 void OnDestroyButtonClicked();

	 

	 UPROPERTY()
	 UInputMappingContext* DoorMappingContext;

	 UPROPERTY()
	 UInputAction* DoorLeftClickAction;

	 UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ProcMesh")
	 AFloorActor* CurrFloorActor;

	 UPROPERTY()
	 UInputMappingContext* FloorMappingContext;

	 /** Action to update location. */
	 UPROPERTY()
	 UInputAction* FloorLeftClickAction;

	 UPROPERTY()
	 bool bFloorMove;

	 UFUNCTION()
	 void FloorLeftClick();

	 UFUNCTION()
	 void OnFloorLengthChanged(float Length);
	 
	 UFUNCTION()
	 void OnFloorWidthChanged(float Width); 
	 
	 UFUNCTION()
	 void OnFloorRotationChanged();

	 UPROPERTY()
	 UStaticMeshComponent* ClickedComponent;	
	 
	UPROPERTY()
	bool bDoorOpen;

	 UFUNCTION()
	 void DoorLeftClick();

	 UFUNCTION()
	 void OpenDoorButtonClick();

	 UFUNCTION()
	 void DoorMeshGeneration(const FDoorMeshData& DoorMeshData);

	 UFUNCTION()
	 void OnFloorButtonClicked();

	 UFUNCTION()
	 void OnWallButtonClicked();

	 
	 UFUNCTION()
	 void OnRoofButtonClicked();

	 UFUNCTION()
	 void OnDoorButtonClicked();

	 UPROPERTY()
	 UInputMappingContext* ModifyComponentMappingContext;

	 /** Action to update location. */
	 UPROPERTY()
	 UInputAction* ModifyComponentLeftClickAction;


	 UFUNCTION()
	 void ModifyComponentLeftClick();

	 UFUNCTION()
	 void OnModifyComponentButtonClicked();

	 EBuildingComponentType TypeOfComponent;

	 UPROPERTY()
	 UBuildingWidget* BuildingWidgetInstance;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<UBuildingWidget> BuildingWidget;

	 UPROPERTY()
	 UArchVizWidget* ArchVizWidgetInstance;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<UArchVizWidget> ArchVizWidget;


	 UPROPERTY()
	 UInputMappingContext* InteriorMappingContext;

	 UPROPERTY()
	 UInputAction* InteriorLeftClickAction;

	 UPROPERTY()
	 UInputAction* InteriorRClickAction;

	 UFUNCTION()
	 void InteriorLeftClick();

	 UFUNCTION()
	 void InteriorRClick();

	 UFUNCTION()
	 void InteriorFloorGenerator(const FInteriorFloorMeshData& InteriorFloorMeshData);

	 UFUNCTION()
	 void InteriorRoofGenerator(const FInteriorRoofMeshData& InteriorRoofMeshData);

	 UFUNCTION()
	 void InteriorWallGenerator(const FInteriorWallMeshData& InteriorWallMeshData);

	 UPROPERTY()
	 bool bInteriorMove; 
	 

	 UPROPERTY()
	 UInteriorWidget* InteriorWidgetInstance;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<UInteriorWidget> InteriorWidget;


	 UPROPERTY()
	 AInteriorActor* CurrInteriorActor;

	/* UPROPERTY()
	 EInteriorComponentType InteriorType;*/


	 UPROPERTY()
	 UInputMappingContext* MaterialMappingContext;

	 UPROPERTY()
	 UInputAction* MaterialLeftClickAction;

	 UFUNCTION()
	 void MaterialLeftClick();

	 UPROPERTY()
	 UMaterialWidget* MaterialWidgetInstance;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<UMaterialWidget> MaterialWidget;

		 UFUNCTION(BlueprintCallable)
		 void SaveGame();

		 UFUNCTION(BlueprintCallable)
		 void LoadGame();


};
