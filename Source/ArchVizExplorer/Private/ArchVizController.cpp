
// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include <Kismet/KismetMathLibrary.h>

AArchVizController::AArchVizController()
{
	CurrWallMaterial = LoadObject<UMaterialInstance>(this, TEXT("/Script/Engine.Material'/Engine/MaterialTemplates/PerfTricks/Spherical2dReflection.Spherical2dReflection'"));
}

void AArchVizController::SetupInputComponent()
{
	Super::SetupInputComponent();
	RoadMappingContext = NewObject<UInputMappingContext>(this);

	RoadLeftClickAction = NewObject<UInputAction>(this);
	RoadLeftClickAction->ValueType = EInputActionValueType::Boolean;
	RoadMappingContext->MapKey(RoadLeftClickAction, EKeys::LeftMouseButton);

	RoadRightClickAction = NewObject<UInputAction>(this);
	RoadRightClickAction->ValueType = EInputActionValueType::Boolean;
	RoadMappingContext->MapKey(RoadRightClickAction, EKeys::RightMouseButton);



	WallMappingContext = NewObject<UInputMappingContext>(this);

	WallLeftClickAction = NewObject<UInputAction>(this);
	WallLeftClickAction->ValueType = EInputActionValueType::Boolean;
	WallMappingContext->MapKey(WallLeftClickAction, EKeys::LeftMouseButton);

	WallRClickAction = NewObject<UInputAction>(this);
	WallRClickAction->ValueType = EInputActionValueType::Boolean;
	WallMappingContext->MapKey(WallRClickAction, EKeys::R);

	DoorMappingContext = NewObject<UInputMappingContext>(this);

	DoorLeftClickAction = NewObject<UInputAction>(this);
	DoorLeftClickAction->ValueType = EInputActionValueType::Boolean;
	DoorMappingContext->MapKey(DoorLeftClickAction, EKeys::LeftMouseButton);

	FloorMappingContext = NewObject<UInputMappingContext>(this);

	FloorLeftClickAction = NewObject<UInputAction>(this);
	FloorLeftClickAction->ValueType = EInputActionValueType::Boolean;
	FloorMappingContext->MapKey(FloorLeftClickAction, EKeys::LeftMouseButton);

	ModifyComponentMappingContext = NewObject<UInputMappingContext>(this);

	ModifyComponentLeftClickAction = NewObject<UInputAction>(this);
	ModifyComponentLeftClickAction->ValueType = EInputActionValueType::Boolean;
	ModifyComponentMappingContext->MapKey(ModifyComponentLeftClickAction, EKeys::LeftMouseButton);

	MaterialMappingContext = NewObject<UInputMappingContext>(this);

	MaterialLeftClickAction = NewObject<UInputAction>(this);
	MaterialLeftClickAction->ValueType = EInputActionValueType::Boolean;
	MaterialMappingContext->MapKey(MaterialLeftClickAction, EKeys::LeftMouseButton);


	InteriorMappingContext = NewObject<UInputMappingContext>(this);

	InteriorLeftClickAction = NewObject<UInputAction>(this);
	InteriorLeftClickAction->ValueType = EInputActionValueType::Boolean;
	InteriorMappingContext->MapKey(InteriorLeftClickAction, EKeys::LeftMouseButton);



	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	if (EIC) {
		EIC->BindAction(RoadLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::RoadLeftClick);
		EIC->BindAction(RoadRightClickAction, ETriggerEvent::Completed, this, &AArchVizController::RoadRightClick);

		EIC->BindAction(WallLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::WallLeftClick);
		EIC->BindAction(WallRClickAction, ETriggerEvent::Completed, this, &AArchVizController::WallRClick);

		EIC->BindAction(DoorLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::DoorLeftClick);

		EIC->BindAction(FloorLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::FloorLeftClick);

		EIC->BindAction(ModifyComponentLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::ModifyComponentLeftClick);

		EIC->BindAction(MaterialLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::MaterialLeftClick);

		EIC->BindAction(InteriorLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::InteriorLeftClick);
	}

	LocalPlayer = GetLocalPlayer();

	SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();



}

void AArchVizController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bWallMove) {
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, WallActorInstance);
		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
			FVector Location = HitResult.Location;
			Location.Z = 0;
			if (WallActorInstance) {
				WallActorInstance->SetActorLocation(Location);
				SnapActor(20, WallActorInstance);
			}
		}
	}

	if (bFloorMove && CurrFloorActor) {
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(CurrFloorActor);

		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
			FVector Location = HitResult.Location;
			//Location.Z = 0;
			CurrFloorActor->SetActorLocation(Location);

			SnapActor(20, CurrFloorActor);
		}
	}
	//bool bMove = 
	if (bInteriorMove && CurrInteriorActor) {

		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(CurrInteriorActor);

		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
			FVector Location = HitResult.Location;
			Location.Z = 0;
			CurrInteriorActor->SetActorLocation(Location);

			//SnapActor(20, CurrFloorActor);
		}
	}

}
void AArchVizController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	bFirstClick = true;
	bFirstRoad = true;
	bEditorMode = false;
	bWallMove = true;
	bFloorMove = false;
	bInteriorMove = false;
	//bMove_ModifyMode = false;

	RoadWidgetInstance = CreateWidget<URoadWidget>(this, RoadWidget);
	RoadWidgetInstance->MaterialBox->RoadMaterialController.BindUObject(this, &AArchVizController::RoadMateialApply);

	MaterialWidgetInstance = CreateWidget<UMaterialWidget>(this, MaterialWidget);
	MaterialWidgetInstance->MaterialBox->RoadMaterialController.BindUObject(this, &AArchVizController::RoadMateialApply);
	MaterialWidgetInstance->BuildingMaterialBox->BuildingMaterialController.BindUObject(this, &AArchVizController::BuildingMateialApply);

	InteriorWidgetInstance = CreateWidget<UInteriorWidget>(this, InteriorWidget);
	InteriorWidgetInstance->FloorMeshesScrollBox->InteriorFloorMeshController.BindUObject(this, &AArchVizController::InteriorFloorGenerator);
	InteriorWidgetInstance->WallMeshesScrollBox->InteriorWallMeshController.BindUObject(this, &AArchVizController::InteriorWallGenerator);
	InteriorWidgetInstance->RoofMeshesScrollBox->InteriorRoofMeshController.BindUObject(this, &AArchVizController::InteriorRoofGenerator);

	ArchVizWidgetInstance = CreateWidget<UArchVizWidget>(this, ArchVizWidget);

	BuildingWidgetInstance = CreateWidget<UBuildingWidget>(this, BuildingWidget);
	BuildingWidgetInstance->DoorMeshBox->DoorMeshController.BindUObject(this, &AArchVizController::DoorMeshGeneration);
	BuildingWidgetInstance->XOffset->OnValueChanged.AddDynamic(this, &AArchVizController::XoffsetChanged);
	BuildingWidgetInstance->YOffset->OnValueChanged.AddDynamic(this, &AArchVizController::YoffsetChanged);
	BuildingWidgetInstance->ZOffset->OnValueChanged.AddDynamic(this, &AArchVizController::ZoffsetChanged);
	BuildingWidgetInstance->MoveButton->OnClicked.AddDynamic(this, &AArchVizController::OnMoveButtonClicked);
	BuildingWidgetInstance->DestroyButton->OnClicked.AddDynamic(this, &AArchVizController::OnDestroyButtonClicked);
	BuildingWidgetInstance->RotateFloorButton->OnClicked.AddDynamic(this, &AArchVizController::OnFloorRotationChanged);
	if (ArchVizWidgetInstance) {
		ArchVizWidgetInstance->AddToViewport();
		ArchVizWidgetInstance->SetPlayerController(this);
	}

}

void AArchVizController::RoadLeftClick()
{

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (bEditorMode) {
		if (CurrRoadActor) {
			CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		}

		CurrRoadActor = Cast<ARoadActor>(HitResult.GetActor());
		if (CurrRoadActor) {
			RoadWidgetInstance->WidthBox->SetValue(CurrRoadActor->GetActorRelativeScale3D().Y * 100);
			RoadWidgetInstance->LocX->SetValue(CurrRoadActor->GetActorLocation().X);
			RoadWidgetInstance->LocY->SetValue(CurrRoadActor->GetActorLocation().Y);
			CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(true);
			CurrRoadActor->ProceduralMeshRoot->CustomDepthStencilValue = 2.0;
		}
	}
	else {

		if (bFirstRoad)
		{
			if (bFirstClick) {


				if (HitResult.bBlockingHit)
				{
					FirstClickLocation = HitResult.Location;
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FirstClickLocation.ToString());

					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = this;


					RoadActor = GetWorld()->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
					CurrRoadActor = RoadActor;
				}
				bFirstClick = false;
			}
			else {


				if (HitResult.bBlockingHit)
				{
					SecondClickLocation = HitResult.Location;

					FVector Dimension;
					Dimension.X = FVector::Dist(FirstClickLocation, SecondClickLocation);

					Dimension.Y = 50;
					Dimension.Z = 5;
					FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FirstClickLocation, SecondClickLocation);
					if (RoadActor) {
						GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, SecondClickLocation.ToString());
						RoadActor->SetActorLocation((SecondClickLocation + FirstClickLocation) / 2);
						RoadActor->SetActorRotation(Rotation);
						RoadActor->GenerateRoad(Dimension);
					}
				}
				bFirstClick = true;
				bFirstRoad = false;
			}
		}
		else {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			RoadActor = GetWorld()->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			FVector Direction = (SecondClickLocation - FirstClickLocation).GetSafeNormal();
			FVector EndPointDirection;
			CurrRoadActor = RoadActor;
			if (HitResult.bBlockingHit)
			{
				PrevClickLocation = FirstClickLocation;
				FirstClickLocation = SecondClickLocation;
				SecondClickLocation = HitResult.Location;

				FVector LeftDirection = FVector::CrossProduct(Direction, FVector::UpVector);
				FVector RightDirection = FVector::CrossProduct(FVector::UpVector, Direction);

				float Angle = GetAngle(Direction, SecondClickLocation - FirstClickLocation);

				if (OnRightOrleft(FirstClickLocation, SecondClickLocation, PrevClickLocation)) {
					if (Angle >= 0 && Angle <= 45)
						EndPointDirection = Direction;
					else if (Angle > 45 && Angle < 180)
						EndPointDirection = LeftDirection;
				}
				else {
					if (Angle >= 0 && Angle <= 45)
						EndPointDirection = Direction;
					else if (Angle > 45 && Angle < 180)
						EndPointDirection = RightDirection;
				}



				FVector Dimension;
				Dimension.X = FVector::Dist(FirstClickLocation, SecondClickLocation);
				Dimension.Y = 50;
				Dimension.Z = 5;
				SecondClickLocation = FirstClickLocation + (EndPointDirection.GetSafeNormal() * Dimension.X);
				FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FirstClickLocation, SecondClickLocation);
				if (RoadActor) {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, SecondClickLocation.ToString());
					RoadActor->SetActorLocation((SecondClickLocation + FirstClickLocation) / 2);
					RoadActor->SetActorRotation(Rotation);
					RoadActor->GenerateRoad(Dimension);
				}
			}

		}
		if (CurrRoadActor) {
			RoadWidgetInstance->LocX->SetValue(CurrRoadActor->GetActorLocation().X);
			RoadWidgetInstance->LocY->SetValue(CurrRoadActor->GetActorLocation().Y);
		}
	}
}

void AArchVizController::RoadMateialApply(const FRoadMaterialData& MaterialData)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Materila applied");
	if (CurrRoadActor) {
		CurrRoadActor->ProceduralMeshRoot->SetMaterial(0, MaterialData.RoadMaterial);
	}
}

void AArchVizController::BuildingMateialApply(const FBuildingMaterialData& MaterialData)
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Materila applied");
	if (WallActorInstance) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent->GetStaticMesh()->GetStaticMaterials().Num()==4) {
				MeshComponent->SetMaterial(3, MaterialData.BuildingMaterial);
			}
			else {
				MeshComponent->SetMaterial(0, MaterialData.BuildingMaterial);

			}
				
		}
	}
}

void AArchVizController::RoadRightClick()
{
	bFirstRoad = true;
}

void AArchVizController::OnRoadWidthChanged(float Width) {
	if (CurrRoadActor) {
		FVector Scale = CurrRoadActor->GetActorRelativeScale3D();
		Scale.Y = (Width / 100);
		CurrRoadActor->SetActorRelativeScale3D(Scale);
	}
}

void AArchVizController::OnRoadLocXChanged(float LocationX)
{
	if (CurrRoadActor)
	{
		FVector Loc = CurrRoadActor->GetActorLocation();
		Loc.X = LocationX;
		CurrRoadActor->SetActorLocation(Loc);
	}
}

void AArchVizController::OnRoadLocYChanged(float LocationY)
{
	if (CurrRoadActor)
	{
		FVector Loc = CurrRoadActor->GetActorLocation();
		Loc.Y = LocationY;
		CurrRoadActor->SetActorLocation(Loc);
	}
}

void AArchVizController::EditorMode() {
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Button");
	if (!bEditorMode) {
		bEditorMode = true;
		RoadWidgetInstance->WidthBox->SetVisibility(ESlateVisibility::Visible);
		RoadWidgetInstance->Border_L->SetVisibility(ESlateVisibility::Visible);
		RoadWidgetInstance->Border_W->SetVisibility(ESlateVisibility::Visible);
		RoadWidgetInstance->DestroyButton->SetVisibility(ESlateVisibility::Visible);
		RoadWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Visible);
		RoadWidgetInstance->EditorButtonText->SetText(FText::FromString("Go To Construction Mode"));
	}
	else {
		bEditorMode = false;
		if (CurrRoadActor) {
			CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		}
		RoadWidgetInstance->WidthBox->SetVisibility(ESlateVisibility::Hidden);
		RoadWidgetInstance->Border_L->SetVisibility(ESlateVisibility::Hidden);
		RoadWidgetInstance->Border_W->SetVisibility(ESlateVisibility::Hidden);
		RoadWidgetInstance->DestroyButton->SetVisibility(ESlateVisibility::Hidden);
		RoadWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Hidden);

		RoadWidgetInstance->EditorButtonText->SetText(FText::FromString("Go To Editor Mode"));

	}

}

void AArchVizController::OnModeChanged(FString Mode)
{
	//bMove_ModifyMode = false;
	if (WallActorInstance && bWallMove)
		WallActorInstance->Destroy();

	if (CurrFloorActor && bFloorMove)
		CurrFloorActor->Destroy();

	if (CurrFloorActor) {
		CurrFloorActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrFloorActor = nullptr;
	}
	if (WallActorInstance) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		WallActorInstance = nullptr;
	}
	if (CurrRoadActor) {
		CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrRoadActor = nullptr;
	}

	if (Mode == "RoadConstruction") {
		if (BuildingWidgetInstance && BuildingWidgetInstance->IsInViewport()) {
			BuildingWidgetInstance->RemoveFromParent();
		}
		if (MaterialWidgetInstance && MaterialWidgetInstance->IsInViewport())
		{
			MaterialWidgetInstance->RemoveFromParent();
		}
		if (InteriorWidgetInstance && InteriorWidgetInstance->IsInViewport())
		{
			InteriorWidgetInstance->RemoveFromParent();
		}
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(RoadMappingContext, 0);
		if (RoadWidgetInstance)
		{
			RoadWidgetInstance->AddToViewport();
			RoadWidgetInstance->SetPlayerController(this);
			RoadWidgetInstance->WidthBox->SetVisibility(ESlateVisibility::Hidden);
			RoadWidgetInstance->Border_L->SetVisibility(ESlateVisibility::Hidden);
			RoadWidgetInstance->Border_W->SetVisibility(ESlateVisibility::Hidden);
			RoadWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Hidden);
			RoadWidgetInstance->DestroyButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (Mode == "BuildingConstruction") {
		if (RoadWidgetInstance && RoadWidgetInstance->IsInViewport())
		{
			RoadWidgetInstance->RemoveFromParent();
		}
		if (MaterialWidgetInstance && MaterialWidgetInstance->IsInViewport())
		{
			MaterialWidgetInstance->RemoveFromParent();
		}
		if (InteriorWidgetInstance && InteriorWidgetInstance->IsInViewport())
		{
			InteriorWidgetInstance->RemoveFromParent();
		}
		if (BuildingWidgetInstance)
		{
			BuildingWidgetInstance->SetPlayerController(this);
			BuildingWidgetInstance->AddToViewport();
			BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Collapsed);
			BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Collapsed);
			BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Collapsed);
			BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Collapsed);
			BuildingWidgetInstance->VerticalBox_Modify->SetVisibility(ESlateVisibility::Hidden);
			BuildingWidgetInstance->RotateFloorButton->SetVisibility(ESlateVisibility::Collapsed);


		}
	}

	if (Mode == "Material") {
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(MaterialMappingContext, 0);
		if (RoadWidgetInstance && RoadWidgetInstance->IsInViewport())
		{
			RoadWidgetInstance->RemoveFromParent();
		}
		if (BuildingWidgetInstance && BuildingWidgetInstance->IsInViewport())
		{
			BuildingWidgetInstance->RemoveFromParent();
		}
		if (InteriorWidgetInstance && InteriorWidgetInstance->IsInViewport())
		{
			InteriorWidgetInstance->RemoveFromParent();
		}
		if (MaterialWidgetInstance)
		{

			MaterialWidgetInstance->AddToViewport();
			MaterialWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Collapsed); 
			MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (Mode == "Interior") {
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(InteriorMappingContext, 0);
		if (RoadWidgetInstance && RoadWidgetInstance->IsInViewport())
		{
			RoadWidgetInstance->RemoveFromParent();
		}
		if (BuildingWidgetInstance && BuildingWidgetInstance->IsInViewport())
		{
			BuildingWidgetInstance->RemoveFromParent();
		}
		if (MaterialWidgetInstance && MaterialWidgetInstance->IsInViewport())
		{
			MaterialWidgetInstance->RemoveFromParent();
		}
		if (InteriorWidgetInstance)
		{

			InteriorWidgetInstance->AddToViewport();
		}
	}
}

float AArchVizController::GetAngle(FVector V1, FVector V2)
{
	FVector NormVector1 = V1.GetSafeNormal();
	FVector NormVector2 = V2.GetSafeNormal();

	float DotProduct = FVector::DotProduct(NormVector1, NormVector2);

	DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f);

	float AngleRadians = FMath::Acos(DotProduct);

	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

	return AngleDegrees;
}

bool AArchVizController::OnRightOrleft(FVector FirstPoint, FVector SecondPoint, FVector Prev)
{
	FVector V1 = Prev - FirstPoint;
	FVector V2 = Prev - SecondPoint;
	FVector CrossProduct = FVector::CrossProduct(V1, V2);

	if (CrossProduct.Z > 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Right");
		return false;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Left");
		return true;
	}
}

void AArchVizController::SnapActor(float SnapValue, AActor* CurrActor)
{

	if (CurrActor) {
		auto CurrentLocation = CurrActor->GetActorLocation();

		CurrentLocation.X = FMath::RoundToFloat(CurrentLocation.X / SnapValue) * SnapValue;

		CurrentLocation.Y = FMath::RoundToFloat(CurrentLocation.Y / SnapValue) * SnapValue;

		CurrActor->SetActorLocation(CurrentLocation);
	}

}

void AArchVizController::WallLeftClick()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	FVector Location;
	if (WallActorInstance)
		Location = WallActorInstance->GetActorLocation();
	WallActorInstance = GetWorld()->SpawnActor<AWallActor>(WallActor, Location, FRotator::ZeroRotator, SpawnParams);
	if (WallActorInstance && TypeOfComponent == EBuildingComponentType::Wall)
	{
		WallActorInstance->GenerateWall(BuildingWidgetInstance->NoOfSegments->GetValue());
	}
}

void AArchVizController::WallRClick()
{
	if (WallActorInstance) {
		FRotator Rotation = WallActorInstance->GetActorRotation();
		Rotation.Yaw += 90;
		WallActorInstance->SetActorRotation(Rotation);
	}
}

void AArchVizController::OnWallSegmentsChanged(float Segments)
{
	if (WallActorInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "hdszbfjkc");
		WallActorInstance->GenerateWall(BuildingWidgetInstance->NoOfSegments->GetValue());
	}
}

void AArchVizController::XoffsetChanged(float off)
{
	if (CurrOffsetActor) {
		FVector Location = CurrOffsetActor->GetActorLocation();
		Location.X += off;
		CurrOffsetActor->SetActorLocation(Location);
	}
}

void AArchVizController::YoffsetChanged(float off)
{
	if (CurrOffsetActor) {
		FVector Location = CurrOffsetActor->GetActorLocation();
		Location.Y += off;
		CurrOffsetActor->SetActorLocation(Location);
	}
}

void AArchVizController::ZoffsetChanged(float off)
{
	if (CurrOffsetActor) {
		FVector Location = CurrOffsetActor->GetActorLocation();
		Location.Z += off;
		CurrOffsetActor->SetActorLocation(Location);
	}
}

void AArchVizController::OnMoveButtonClicked()
{
	//bMove_ModifyMode = true;
	if (WallActorInstance)
		bWallMove = true;
	else if (CurrFloorActor)
		bFloorMove = true;

}

void AArchVizController::OnDestroyButtonClicked()
{
	/*if (WallActorInstance)
		WallActorInstance->Destroy();
	else if (CurrFloorActor)
		CurrFloorActor->Destroy();*/
	if (CurrOffsetActor) {
		CurrOffsetActor->Destroy();
	}

}

void AArchVizController::FloorLeftClick()
{
	bFloorMove = false;


}

void AArchVizController::OnFloorLengthChanged(float Length)
{

	/*if (CurrFloorActor && (TypeOfComponent == EBuildingComponentType::Floor || TypeOfComponent == EBuildingComponentType::Roof))*/
	if (CurrFloorActor)
	{
		FVector Scale = CurrFloorActor->GetActorRelativeScale3D();
		Scale.X = Length / 100.0f;
		CurrFloorActor->SetActorRelativeScale3D(Scale);
	}
}

void AArchVizController::OnFloorWidthChanged(float Width)
{
	/*if (CurrFloorActor && (TypeOfComponent == EBuildingComponentType::Floor || TypeOfComponent == EBuildingComponentType::Roof))*/
	if (CurrFloorActor)
	{
		FVector Scale = CurrFloorActor->GetActorRelativeScale3D();
		Scale.Y = Width / 100.0f;
		CurrFloorActor->SetActorRelativeScale3D(Scale);


	}
}

void AArchVizController::OnFloorRotationChanged()
{
	if (CurrFloorActor)
	{
		FRotator Rotation_ = CurrFloorActor->GetActorRotation();
		Rotation_.Yaw += 90;
		CurrFloorActor->SetActorRotation(Rotation_);
	}
}

void AArchVizController::DoorLeftClick()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);

	// Cast to AWallActor
	AWallActor* CurrWallSegment = Cast<AWallActor>(HitResult.GetActor());
	
	if (CurrWallSegment)
	{
		if (ClickedComponent) {
			ClickedComponent->SetRenderCustomDepth(false);
		}
		ClickedComponent = Cast<UStaticMeshComponent>(HitResult.GetComponent());
		
		if (ClickedComponent)
		{
			if (ClickedComponent->GetMaterials().Num()==4)
				CurrWallMaterial = ClickedComponent->GetMaterial(3);
			else
				CurrWallMaterial = ClickedComponent->GetMaterial(0);

			ClickedComponent->SetRenderCustomDepth(true);
			ClickedComponent->CustomDepthStencilValue = 2.0;
			BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AArchVizController::DoorMeshGeneration(const FDoorMeshData& DoorMeshData)
{
	if (ClickedComponent) {
		FVector Location = ClickedComponent->GetRelativeLocation();

		UStaticMesh* DoorMesh = DoorMeshData.DoorMesh;

		float DoorLength = DoorMesh->GetBounds().GetBox().GetSize().X;

		if (DoorMesh)
		{
			UMaterialInterface* CurrFrameMaterial = DoorMesh->GetStaticMaterials()[0].MaterialInterface;
			ClickedComponent->SetStaticMesh(DoorMesh);
			if(CurrWallMaterial)
			    ClickedComponent->SetMaterial(3, CurrWallMaterial);
			if(CurrFrameMaterial)
				ClickedComponent->SetMaterial(0, CurrFrameMaterial);

			ClickedComponent->SetRenderCustomDepth(false);

		}
	}
}

void AArchVizController::OnFloorButtonClicked()
{
	//bMove_ModifyMode = false;
	if ((TypeOfComponent != EBuildingComponentType::None) && WallActorInstance && bWallMove)
	{
		WallActorInstance->Destroy();
		bWallMove = false;
	}

	if (CurrFloorActor) {
		CurrFloorActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrFloorActor = nullptr;
	}

	TypeOfComponent = EBuildingComponentType::Floor;

	if (WallActorInstance) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		WallActorInstance = nullptr;
	}
	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(FloorMappingContext, 0);

	if (BuildingWidgetInstance) {
		BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Visible);
		BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Visible);
		BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->VerticalBox_Modify->SetVisibility(ESlateVisibility::Hidden);
		BuildingWidgetInstance->RotateFloorButton->SetVisibility(ESlateVisibility::Visible);


	}



	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrFloorActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrFloorActor)
	{
		BuildingWidgetInstance->FLength->SetValue(CurrFloorActor->GetActorRelativeScale3D().X * 100);
		BuildingWidgetInstance->FWidth->SetValue(CurrFloorActor->GetActorRelativeScale3D().Y * 100);
		bFloorMove = true;
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(CurrFloorActor);

		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {

			FVector Location = HitResult.Location;
			//Location.Z = 0;

			CurrFloorActor->GenerateFloor(FVector(100, 100, 10), FName("Floor"));
			CurrFloorActor->SetActorLocation(Location);

		}
	}
}

void AArchVizController::OnWallButtonClicked()
{
	//bMove_ModifyMode = false;
	if ((TypeOfComponent != EBuildingComponentType::None) && CurrFloorActor && bFloorMove)
	{
		CurrFloorActor->Destroy();
		bFloorMove = false;
	}

	TypeOfComponent = EBuildingComponentType::Wall;

	if (CurrFloorActor) {
		CurrFloorActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrFloorActor = nullptr;
	}
	if (WallActorInstance) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		WallActorInstance = nullptr;
	}
	if (BuildingWidgetInstance) {
		BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Visible);
		BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->RotateFloorButton->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->VerticalBox_Modify->SetVisibility(ESlateVisibility::Hidden);
	}


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	WallActorInstance = GetWorld()->SpawnActor<AWallActor>(WallActor, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (WallActorInstance && TypeOfComponent == EBuildingComponentType::Wall)
	{
		WallActorInstance->GenerateWall(BuildingWidgetInstance->NoOfSegments->GetValue());
	}

	bWallMove = true;

	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(WallMappingContext, 0);
}

void AArchVizController::OnRoofButtonClicked()
{
	//bMove_ModifyMode = false;
	if ((TypeOfComponent != EBuildingComponentType::None) && WallActorInstance && bWallMove)
	{
		WallActorInstance->Destroy();
		bWallMove = false;
	}

	TypeOfComponent = EBuildingComponentType::Roof;

	if (CurrFloorActor) {
		CurrFloorActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrFloorActor = nullptr;

	}
	if (WallActorInstance) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		WallActorInstance = nullptr;
	}
	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(FloorMappingContext, 0);

	if (BuildingWidgetInstance) {
		BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Visible);
		BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Visible);
		BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->VerticalBox_Modify->SetVisibility(ESlateVisibility::Hidden);
		BuildingWidgetInstance->RotateFloorButton->SetVisibility(ESlateVisibility::Visible);

	}



	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrFloorActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrFloorActor)
	{
		bFloorMove = true;
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(this);
		TraceParams.AddIgnoredActor(CurrFloorActor);

		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {

			FVector Location = HitResult.Location;
			//Location.Z = 0;
			CurrFloorActor->GenerateFloor(FVector(100, 100, 10), FName("Roof"));
			CurrFloorActor->SetActorLocation(Location);

		}
	}

}

void AArchVizController::OnDoorButtonClicked()
{
	if (BuildingWidgetInstance) {
		BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->VerticalBox_Modify->SetVisibility(ESlateVisibility::Hidden);
		BuildingWidgetInstance->RotateFloorButton->SetVisibility(ESlateVisibility::Collapsed);


	}

	if ((TypeOfComponent != EBuildingComponentType::None) && WallActorInstance && bWallMove)
	{
		WallActorInstance->Destroy();
		bWallMove = false;
	}
	if ((TypeOfComponent != EBuildingComponentType::None) && CurrFloorActor && bFloorMove)
	{
		CurrFloorActor->Destroy();
		bFloorMove = false;
	}
	TypeOfComponent = EBuildingComponentType::Door;
	//bMove_ModifyMode = false;
	if (CurrFloorActor) {
		CurrFloorActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrFloorActor = nullptr;

	}
	if (WallActorInstance) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		WallActorInstance = nullptr;
	}

	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(DoorMappingContext, 0);
}

void AArchVizController::ModifyComponentLeftClick()
{
	//bMove_ModifyMode = false;
	bWallMove = false;
	bFloorMove = false;


	if (CurrOffsetActor)
	{
		CurrOffsetActor = nullptr;
	}
	if (CurrFloorActor) {
		CurrFloorActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrFloorActor = nullptr;
	}
	if (WallActorInstance) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		WallActorInstance = nullptr;
	}

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	WallActorInstance = Cast<AWallActor>(HitResult.GetActor());
	CurrFloorActor = Cast<AFloorActor>(HitResult.GetActor());
	if (WallActorInstance) {


		CurrOffsetActor = WallActorInstance;
		//TypeOfComponent = EBuildingComponentType::Wall;


		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(true);
				MeshComponent->CustomDepthStencilValue = 2.0;
			}
		}


		if (BuildingWidgetInstance) {
			BuildingWidgetInstance->VerticalBox_Modify->SetVisibility(ESlateVisibility::Visible);
			BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Collapsed);
			BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Collapsed);
			BuildingWidgetInstance->RotateFloorButton->SetVisibility(ESlateVisibility::Collapsed);

		}
	}
	else if (CurrFloorActor) {

		CurrOffsetActor = CurrFloorActor;
		/*CurrFloorActor = CurrRoofFloorActor;
		TypeOfComponent = EBuildingComponentType::Floor;*/


		CurrFloorActor->ProceduralMeshRoot->SetRenderCustomDepth(true);
		CurrFloorActor->ProceduralMeshRoot->CustomDepthStencilValue = 2.0;



		if (BuildingWidgetInstance) {
			BuildingWidgetInstance->VerticalBox_Modify->SetVisibility(ESlateVisibility::Visible);
			BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Visible);
			BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Visible);
			BuildingWidgetInstance->RotateFloorButton->SetVisibility(ESlateVisibility::Visible);
			BuildingWidgetInstance->FLength->SetValue(CurrFloorActor->GetActorRelativeScale3D().X * 100);
			BuildingWidgetInstance->FWidth->SetValue(CurrFloorActor->GetActorRelativeScale3D().Y * 100);

		}

	}
}

void AArchVizController::OnModifyComponentButtonClicked()
{
	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(ModifyComponentMappingContext, 0);

	if (WallActorInstance && bWallMove)
		WallActorInstance->Destroy();
	if (CurrFloorActor && bFloorMove)
		CurrFloorActor->Destroy();

	TypeOfComponent = EBuildingComponentType::None;
	if (BuildingWidgetInstance)
	{
		BuildingWidgetInstance->SetPlayerController(this);
		BuildingWidgetInstance->AddToViewport();
		BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->RotateFloorButton->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Hidden);


	}
}

void AArchVizController::InteriorLeftClick()
{
	FCollisionQueryParams TraceParams;
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(this);
	TraceParams.AddIgnoredActor(CurrInteriorActor);


	FHitResult HitResult;
	FVector CursorWorldLocation;
	FVector CursorWorldDirection;
	DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
		AFloorActor* FActor = Cast<AFloorActor>(HitResult.GetActor());
		AWallActor* WActor = Cast<AWallActor>(HitResult.GetActor());

		bool bCon = ((HitResult.GetComponent()->GetName() == "Floor") && TypeOfInterior == EInteriorComponentType::Floor) || ((HitResult.GetComponent()->GetName() == "Roof") && TypeOfInterior == EInteriorComponentType::Roof);

		if ((bCon && FActor) || (WActor && TypeOfInterior == EInteriorComponentType::Wall)) {
			bInteriorMove = false;
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Interior Left Click");
		}

	}
}

void AArchVizController::InteriorFloorGenerator(const FInteriorFloorMeshData& InteriorFloorMeshData)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrInteriorActor) {
		TypeOfInterior = EInteriorComponentType::Floor;
		bInteriorMove = true;
		CurrInteriorActor->GenerateInterior(InteriorFloorMeshData.InteriorFloorMesh);
	}
}
void AArchVizController::InteriorRoofGenerator(const FInteriorRoofMeshData& InteriorRoofMeshData)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrInteriorActor) {
		TypeOfInterior = EInteriorComponentType::Roof;
		bInteriorMove = true;
		CurrInteriorActor->GenerateInterior(InteriorRoofMeshData.InteriorRoofMesh);
	}
}
void AArchVizController::InteriorWallGenerator(const FInteriorWallMeshData& InteriorWallMeshData)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrInteriorActor) {
		TypeOfInterior = EInteriorComponentType::Wall;
		bInteriorMove = true;
		CurrInteriorActor->GenerateInterior(InteriorWallMeshData.InteriorWallMesh);
	}
}

void AArchVizController::MaterialLeftClick()
{
	//bMove_ModifyMode = false;
	bWallMove = false;
	bFloorMove = false;


	if (CurrRoadActor) {
		CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrRoadActor = nullptr;

	}
	if (CurrFloorActor) {
		CurrFloorActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrFloorActor = nullptr;
	}
	if (WallActorInstance) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		WallActorInstance = nullptr;
	}

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	WallActorInstance = Cast<AWallActor>(HitResult.GetActor());
	CurrFloorActor = Cast<AFloorActor>(HitResult.GetActor());
	CurrRoadActor = Cast<ARoadActor>(HitResult.GetActor());

	if (WallActorInstance) {

		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(true);
				MeshComponent->CustomDepthStencilValue = 2.0;
			}
		}

		if (MaterialWidgetInstance) {
			MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Visible);
		}

	}
	else if (CurrFloorActor) {

		CurrFloorActor->ProceduralMeshRoot->SetRenderCustomDepth(true);
		CurrFloorActor->ProceduralMeshRoot->CustomDepthStencilValue = 2.0;

	}
	else if (CurrRoadActor) {

		CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(true);
		CurrRoadActor->ProceduralMeshRoot->CustomDepthStencilValue = 2.0;

		if (MaterialWidgetInstance) {
			MaterialWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Visible);
		}

	}
}



