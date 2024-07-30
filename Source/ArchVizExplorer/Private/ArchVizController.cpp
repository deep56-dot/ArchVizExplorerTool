
// Fill out your copyright notice in the Description page of Project Settings.


#include "ArchVizController.h"
#include "TimerManager.h"
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

	DeleteClickAction = NewObject<UInputAction>(this);
	DeleteClickAction->ValueType = EInputActionValueType::Boolean;

	RClickAction = NewObject<UInputAction>(this);
	RClickAction->ValueType = EInputActionValueType::Boolean;

	WallMappingContext->MapKey(DeleteClickAction, EKeys::Delete);
	WallMappingContext->MapKey(RClickAction, EKeys::R);
	

	WallLeftClickAction = NewObject<UInputAction>(this);
	WallLeftClickAction->ValueType = EInputActionValueType::Boolean;
	WallMappingContext->MapKey(WallLeftClickAction, EKeys::LeftMouseButton);

	DoorMappingContext = NewObject<UInputMappingContext>(this);
	DoorMappingContext->MapKey(DeleteClickAction, EKeys::Delete);

	DoorLeftClickAction = NewObject<UInputAction>(this);
	DoorLeftClickAction->ValueType = EInputActionValueType::Boolean;
	DoorMappingContext->MapKey(DoorLeftClickAction, EKeys::LeftMouseButton);

	FloorMappingContext = NewObject<UInputMappingContext>(this);
	FloorMappingContext->MapKey(DeleteClickAction, EKeys::Delete);
	FloorMappingContext->MapKey(RClickAction, EKeys::R);

	FloorLeftClickAction = NewObject<UInputAction>(this);
	FloorLeftClickAction->ValueType = EInputActionValueType::Boolean;
	FloorMappingContext->MapKey(FloorLeftClickAction, EKeys::LeftMouseButton);

	ModifyComponentMappingContext = NewObject<UInputMappingContext>(this);
	ModifyComponentMappingContext->MapKey(DeleteClickAction, EKeys::Delete);
	ModifyComponentMappingContext->MapKey(RClickAction, EKeys::R);

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

	InteriorRClickAction = NewObject<UInputAction>(this);
	InteriorRClickAction->ValueType = EInputActionValueType::Boolean;
	InteriorMappingContext->MapKey(InteriorRClickAction, EKeys::R);
	
	InteriorDeleteClickAction = NewObject<UInputAction>(this);
	InteriorDeleteClickAction->ValueType = EInputActionValueType::Boolean;
	InteriorMappingContext->MapKey(InteriorDeleteClickAction, EKeys::Delete);


	TemplateMappingContext = NewObject<UInputMappingContext>(this);

	TemplateLeftClickAction = NewObject<UInputAction>(this);
	TemplateLeftClickAction->ValueType = EInputActionValueType::Boolean;
	TemplateMappingContext->MapKey(TemplateLeftClickAction, EKeys::LeftMouseButton);
	
	TemplateRClickAction = NewObject<UInputAction>(this);
	TemplateRClickAction->ValueType = EInputActionValueType::Boolean;
	TemplateMappingContext->MapKey(TemplateRClickAction, EKeys::R);


	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	if (EIC) {
		EIC->BindAction(DeleteClickAction, ETriggerEvent::Completed, this, &AArchVizController::DeleteClicked);	
		EIC->BindAction(RClickAction, ETriggerEvent::Completed, this, &AArchVizController::RClicked);

		EIC->BindAction(RoadLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::RoadLeftClick);
		EIC->BindAction(RoadRightClickAction, ETriggerEvent::Completed, this, &AArchVizController::RoadRightClick);

		EIC->BindAction(WallLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::WallLeftClick);
	
		EIC->BindAction(DoorLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::DoorLeftClick);

		EIC->BindAction(FloorLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::FloorLeftClick);
		
		EIC->BindAction(ModifyComponentLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::ModifyComponentLeftClick);

		EIC->BindAction(MaterialLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::MaterialLeftClick);

		EIC->BindAction(InteriorLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::InteriorLeftClick);
		EIC->BindAction(InteriorRClickAction, ETriggerEvent::Completed, this, &AArchVizController::InteriorRClick);
		EIC->BindAction(InteriorDeleteClickAction, ETriggerEvent::Completed, this, &AArchVizController::InteriorDeleteClick);

		EIC->BindAction(TemplateLeftClickAction, ETriggerEvent::Completed, this, &AArchVizController::TemplateLeftClick);
		EIC->BindAction(TemplateRClickAction, ETriggerEvent::Completed, this, &AArchVizController::TemplateRClick);


	}

	LocalPlayer = GetLocalPlayer();

	SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(TemplateMappingContext, 0);

}

void AArchVizController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrFloorNo == 0) {
		BuildingWidgetInstance->FloorButton->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		BuildingWidgetInstance->FloorButton->SetVisibility(ESlateVisibility::Hidden);

	}
	if (bWallMove) {
		FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, WallActorInstance);
		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
			FVector Location = HitResult.Location;
			Location.Z = (CurrFloorNo *320);
			auto Rotation = PlayerCameraManager->GetCameraRotation();


			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, FString::FromInt(Rotation.Yaw));
			if (IsValid(WallActorInstance)) {
				if ((Rotation.Yaw >= 0 && Rotation.Yaw < 45) || (Rotation.Yaw >= 315 && Rotation.Yaw < 360))
				{
					WallActorInstance->SetActorRotation(FRotator(0, 90, 0));
				}
				else if (Rotation.Yaw >= 45 && Rotation.Yaw < 135)
				{
					WallActorInstance->SetActorRotation(FRotator(0, 180, 0));

				}
				else if (Rotation.Yaw >= 135 && Rotation.Yaw < 225)
				{
					WallActorInstance->SetActorRotation(FRotator(0, -90, 0));

				}
				else
				{
					WallActorInstance->SetActorRotation(FRotator(0, 0, 0));
				}
				WallActorInstance->SetActorLocation(Location);

				SnapActor(20, WallActorInstance);
			}
		}
	}

	if (!bWallMove && TypeOfComponent==EBuildingComponentType::Wall) {
		if (bWallGenearting && IsValid(WallActorInstance)) {
			FVector StartLocation = WallActorInstance->GetActorLocation();
			FVector EndLocation;

			FCollisionQueryParams TraceParams;
			TraceParams.bTraceComplex = true;
			TraceParams.AddIgnoredActor(GetPawn());
			TraceParams.AddIgnoredActor(WallActorInstance);
			FHitResult HitResult;
			FVector CursorWorldLocation;
			FVector CursorWorldDirection;
			DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

			if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
				EndLocation = HitResult.Location;
				EndLocation.Z = (CurrFloorNo * 320);
			}

			int X = EndLocation.X - StartLocation.X;
			int Y = EndLocation.Y - StartLocation.Y;

			int32 NoOFSegment = FVector::Dist(StartLocation, EndLocation) / 200;
			
			if (NoOFSegment == WallActorInstance->NoOfSegments)
				return;
			

			if (abs(X) > abs(Y))
			{
				if (X >= 0)
				{
					WallActorInstance->SetActorRotation(FRotator(0, 0, 0));
				}
				else
				{
					WallActorInstance->SetActorRotation(FRotator(0, 180, 0));
				}
			}
			else
			{
				if (Y >= 0)
				{
					WallActorInstance->SetActorRotation(FRotator(0, 90, 0));
				}
				else
				{
					WallActorInstance->SetActorRotation(FRotator(0, -90, 0));
				}
			}

			WallActorInstance->GenerateWall(NoOFSegment);
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
			if (CurrFloorActor->TypeOfActor == "Floor")
				Location.Z = 0;
			if (CurrFloorActor->TypeOfActor == "Roof")
				Location.Z = ((CurrFloorNo + 1)*300)+(CurrFloorNo*20);
			auto Rotation = PlayerCameraManager->GetCameraRotation();



			if (IsValid(CurrFloorActor)) {
				if ((Rotation.Yaw >= 0 && Rotation.Yaw < 45) || (Rotation.Yaw >= 315 && Rotation.Yaw < 360))
				{
					CurrFloorActor->SetActorRotation(FRotator(0, 90, 0));
				}
				else if (Rotation.Yaw >= 45 && Rotation.Yaw < 135)
				{
					CurrFloorActor->SetActorRotation(FRotator(0, 180, 0));

				}
				else if (Rotation.Yaw >= 135 && Rotation.Yaw < 225)
				{
					CurrFloorActor->SetActorRotation(FRotator(0, -90, 0));

				}
				else
				{
					CurrFloorActor->SetActorRotation(FRotator(0, 0, 0));
				}
				CurrFloorActor->SetActorLocation(Location);

				SnapActor(20, CurrFloorActor);
			}
			
		}
	}

	if (!bFloorMove && (TypeOfComponent == EBuildingComponentType::Floor || TypeOfComponent == EBuildingComponentType::Roof)) {
		if (bFloorGenerating && CurrFloorActor) {

			FVector StartLocation = CurrFloorActor->GetActorLocation();
			if (CurrFloorActor->TypeOfActor == "Floor")
				StartLocation.Z = 0;
			if (CurrFloorActor->TypeOfActor == "Roof")
				StartLocation.Z = ((CurrFloorNo + 1) * 300) + (CurrFloorNo * 20);
			FVector EndLocation;

			FCollisionQueryParams TraceParams;
			TraceParams.bTraceComplex = true;
			TraceParams.AddIgnoredActor(GetPawn());
			TraceParams.AddIgnoredActor(CurrFloorActor);
			FHitResult HitResult;
			FVector CursorWorldLocation;
			FVector CursorWorldDirection;
			DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

			if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
				EndLocation = HitResult.Location;
				if (CurrFloorActor->TypeOfActor == "Floor")
					EndLocation.Z = 0;
				if (CurrFloorActor->TypeOfActor == "Roof")
					EndLocation.Z = ((CurrFloorNo + 1) * 300) + (CurrFloorNo * 20);
			}

			int X = EndLocation.X - StartLocation.X;
			int Y = EndLocation.Y - StartLocation.Y;

		/*	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, FString::FromInt(X) + "X");
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Black, FString::FromInt(Y) + "Y");*/

			if (X > 0 && Y > 0) {
				CurrFloorActor->SetActorRotation(FRotator(0, 0, 0));
				if (CurrFloorActor->TypeOfActor == "Floor")
					CurrFloorActor->GenerateFloor(FVector(abs(X), abs(Y), 10));
				else
					CurrFloorActor->GenerateRoof(FVector(abs(X), abs(Y), 10));

			}
			else if (X > 0 && Y < 0) {
				CurrFloorActor->SetActorRotation(FRotator(0, 270, 0));
				if (CurrFloorActor->TypeOfActor == "Floor")
					CurrFloorActor->GenerateFloor(FVector(abs(Y), abs(X), 10));
				else
					CurrFloorActor->GenerateRoof(FVector(abs(Y), abs(X), 10));
			}
			else if (X < 0 && Y > 0) {
				CurrFloorActor->SetActorRotation(FRotator(0, 90, 0));
				if (CurrFloorActor->TypeOfActor == "Floor")
					CurrFloorActor->GenerateFloor(FVector(abs(Y), abs(X), 10));
				else
					CurrFloorActor->GenerateRoof(FVector(abs(Y), abs(X), 10));
			}
			else if (X < 0 && Y < 0) {
				CurrFloorActor->SetActorRotation(FRotator(0, 180, 0));
				if (CurrFloorActor->TypeOfActor == "Floor")
					CurrFloorActor->GenerateFloor(FVector(abs(X), abs(Y), 10));
				else
					CurrFloorActor->GenerateRoof(FVector(abs(X), abs(Y), 10));
			}

		}
	}

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
			CurrInteriorActor->SetActorLocation(Location);

		}
	}

	if (bTemplateMove && CurrTemplateActor) {

		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(GetPawn());
		TraceParams.AddIgnoredActor(CurrTemplateActor);
		TraceParams.AddIgnoredActors(IgonoreActors);
	
		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
			FVector Location = HitResult.Location;
			//Location.Z = 0;
			CurrTemplateActor->SetActorLocation(Location);

		}
	}

}
void AArchVizController::IncFloorButtonClicked()
{
	if (BuildingWidgetInstance) {
		CurrFloorNo++;
		SetCustomText(FText::FromString("You Are Creating Floor" + FString::FromInt(CurrFloorNo)));

		BuildingWidgetInstance->CurrFloorValue->SetText(FText::AsNumber(CurrFloorNo));
	}
}
void AArchVizController::DecFloorButtonClicked()
{
	if (BuildingWidgetInstance) {
		if(CurrFloorNo >0){
			CurrFloorNo--;
			SetCustomText(FText::FromString("You Are Creating Floor" + FString::FromInt(CurrFloorNo)));
			BuildingWidgetInstance->CurrFloorValue->SetText(FText::AsNumber(CurrFloorNo));
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
	bWallGenearting = false;
	bFloorMove = false;
	bInteriorMove = false;
	bTemplateMove = false;
	bDoorOpen = false;
	CurrFloorNo = 0;

	//For Template
	TArray<FString> Filenames;
	FString DirectoryPath = FPaths::ProjectSavedDir() / TEXT("SaveGames");
	

	Filenames = FindFiles(DirectoryPath, ".sav");
	for (int i = 0; i < Filenames.Num(); i++) {
		Filenames[i] = Filenames[i].LeftChop(4);
	}

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
	
	ArchVizWidgetInstance->SaveOption->SetVisibility(ESlateVisibility::Collapsed);
	ArchVizWidgetInstance->LoadOptions->SetVisibility(ESlateVisibility::Collapsed);
	ArchVizWidgetInstance->CustomTextBorder->SetVisibility(ESlateVisibility::Hidden);
	ArchVizWidgetInstance->InstructionButton->OnClicked.AddDynamic(this, &AArchVizController::OnInstructionButtonClicked);
	if (ArchVizWidgetInstance) {
		ArchVizWidgetInstance->PopulateComboBox(Filenames);
	}

	BuildingWidgetInstance = CreateWidget<UBuildingWidget>(this, BuildingWidget);
	BuildingWidgetInstance->DoorMeshBox->DoorMeshController.BindUObject(this, &AArchVizController::DoorMeshGeneration);

	BuildingWidgetInstance->XOffset->OnValueChanged.AddDynamic(this, &AArchVizController::XoffsetChanged);
	BuildingWidgetInstance->YOffset->OnValueChanged.AddDynamic(this, &AArchVizController::YoffsetChanged);
	BuildingWidgetInstance->ZOffset->OnValueChanged.AddDynamic(this, &AArchVizController::ZoffsetChanged);
	BuildingWidgetInstance->MoveButton->OnClicked.AddDynamic(this, &AArchVizController::OnMoveButtonClicked);	
	BuildingWidgetInstance->IncFloorButton->OnClicked.AddDynamic(this, &AArchVizController::IncFloorButtonClicked);	
	BuildingWidgetInstance->DecFloorButton->OnClicked.AddDynamic(this, &AArchVizController::DecFloorButtonClicked);
	
	
	BuildingWidgetInstance->OpenDoorButton->OnClicked.AddDynamic(this, &AArchVizController::OpenDoorButtonClick);
	if (ArchVizWidgetInstance) {
		ArchVizWidgetInstance->AddToViewport();
		
		ArchVizWidgetInstance->CanvasPanelStart->SetVisibility(ESlateVisibility::Visible);
		ArchVizWidgetInstance->SetPlayerController(this);
	}

}

void AArchVizController::RoadLeftClick()
{

	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if (bEditorMode) {
		if (IsValid(CurrRoadActor)) {
			CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		}

		CurrRoadActor = Cast<ARoadActor>(HitResult.GetActor());
		if (IsValid(CurrRoadActor)) {
			RoadWidgetInstance->WidthBox->SetValue(CurrRoadActor->GetActorRelativeScale3D().Y * 100);
			RoadWidgetInstance->LocX->SetValue(CurrRoadActor->GetActorLocation().X);
			RoadWidgetInstance->LocY->SetValue(CurrRoadActor->GetActorLocation().Y);
			RoadWidgetInstance->WidthBox->SetVisibility(ESlateVisibility::Visible);
			RoadWidgetInstance->Border_L->SetVisibility(ESlateVisibility::Visible);
			RoadWidgetInstance->Border_W->SetVisibility(ESlateVisibility::Visible);
			RoadWidgetInstance->DestroyButton->SetVisibility(ESlateVisibility::Visible);
			RoadWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Visible);
			CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(true);
			CurrRoadActor->ProceduralMeshRoot->CustomDepthStencilValue = 2.0;
		}
		else {
			RoadWidgetInstance->WidthBox->SetVisibility(ESlateVisibility::Hidden);
			RoadWidgetInstance->Border_L->SetVisibility(ESlateVisibility::Hidden);
			RoadWidgetInstance->Border_W->SetVisibility(ESlateVisibility::Hidden);
			RoadWidgetInstance->DestroyButton->SetVisibility(ESlateVisibility::Hidden);
			RoadWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Hidden);

		}
	}
	else {
		if (HitResult.GetActor() && !(HitResult.GetActor()->IsA<AFloorActor>())) {

			if (bFirstRoad)
			{
				if (bFirstClick) {
					if (HitResult.bBlockingHit)
					{
						FirstClickLocation = HitResult.Location;
						

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

						Dimension.Y = 100;
						Dimension.Z = 5;
						FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FirstClickLocation, SecondClickLocation);
						if (RoadActor) {
							
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
						if (Angle >= 0 && Angle <= 45) {
							EndPointDirection = Direction;
							FirstClickLocation -= (Direction.GetSafeNormal() * (CurrRoadActor->GetActorScale3D().Y*100 / 2));

						}
						else if (Angle > 45 && Angle < 180) {
							EndPointDirection = LeftDirection;
							FirstClickLocation -= (LeftDirection.GetSafeNormal() * (CurrRoadActor->GetActorScale3D().Y*100 / 2));

						}
					}
					else {
						if (Angle >= 0 && Angle <= 45) {
							EndPointDirection = Direction;
							FirstClickLocation -= (Direction.GetSafeNormal() * (CurrRoadActor->GetActorScale3D().Y*100/ 2));

						}
						else if (Angle > 45 && Angle < 180)
						{
							EndPointDirection = RightDirection;
							GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::SanitizeFloat(CurrRoadActor->GetActorScale3D().Y * 100));
							FirstClickLocation -= (RightDirection.GetSafeNormal() * (CurrRoadActor->GetActorScale3D().Y*100 / 2));
							GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FirstClickLocation.ToString());

						}

					}



					FVector Dimension;
					Dimension.X = FVector::Dist(FirstClickLocation, SecondClickLocation);
					Dimension.Y = 100;
					Dimension.Z = 5;
					SecondClickLocation = FirstClickLocation + (EndPointDirection.GetSafeNormal() * Dimension.X);
					FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FirstClickLocation, SecondClickLocation);
					if (RoadActor) {
						
						RoadActor->SetActorLocation((SecondClickLocation + FirstClickLocation) / 2);
						RoadActor->SetActorRotation(Rotation);
						RoadActor->GenerateRoad(Dimension);
					}
				}

			}
			if (IsValid(CurrRoadActor)) {
				RoadWidgetInstance->LocX->SetValue(CurrRoadActor->GetActorLocation().X);
				RoadWidgetInstance->LocY->SetValue(CurrRoadActor->GetActorLocation().Y);
			}
		}
	}
	
}

void AArchVizController::RoadMateialApply(const FRoadMaterialData& MaterialData)
{
	if (IsValid(CurrRoadActor)) {
		CurrRoadActor->ProceduralMeshRoot->SetMaterial(0, MaterialData.RoadMaterial);
	}
	if (MaterialWidgetInstance) {
		MaterialWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	SetCustomText(FText::FromString("Material Applied Sucessfully"));
}

void AArchVizController::BuildingMateialApply(const FBuildingMaterialData& MaterialData)
{
	
	SetCustomText(FText::FromString("Material Applied Sucessfully"));

	if (IsValid(WallActorInstance)) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent->GetStaticMesh()->GetStaticMaterials().Num() == 4) {
				MeshComponent->SetMaterial(3, MaterialData.BuildingMaterial);
			}
			else {
				MeshComponent->SetMaterial(0, MaterialData.BuildingMaterial);

			}

		}
	}
	if (IsValid(CurrFloorActor)) {
		if (CurrFloorActor->TypeOfActor == "Floor")
			CurrFloorActor->ProceduralMeshFloor->SetMaterial(0, MaterialData.BuildingMaterial);
		else
			CurrFloorActor->ProceduralMeshRoof->SetMaterial(0, MaterialData.BuildingMaterial);
	}
	if (MaterialWidgetInstance) {
		MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AArchVizController::RoadRightClick()
{
	bFirstRoad = true;
}

void AArchVizController::OnRoadWidthChanged(float Width) {
	if (IsValid(CurrRoadActor)) {
		FVector Scale = CurrRoadActor->GetActorRelativeScale3D();
		Scale.Y = (Width / 100);
		CurrRoadActor->SetActorRelativeScale3D(Scale);
	}
}

void AArchVizController::OnRoadLocXChanged(float LocationX)
{
	if (IsValid(CurrRoadActor))
	{
		FVector Loc = CurrRoadActor->GetActorLocation();
		Loc.X = LocationX;
		CurrRoadActor->SetActorLocation(Loc);
	}
}

void AArchVizController::OnRoadLocYChanged(float LocationY)
{
	if (IsValid(CurrRoadActor))
	{
		FVector Loc = CurrRoadActor->GetActorLocation();
		Loc.Y = LocationY;
		CurrRoadActor->SetActorLocation(Loc);
	}
}

void AArchVizController::EditorMode() {
	
	if (!bEditorMode) {
		bEditorMode = true;
	
		RoadWidgetInstance->EditorButtonText->SetText(FText::FromString("Go To Construction Mode"));
	}
	else {
		bEditorMode = false;
		if (IsValid(CurrRoadActor)) {
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
	{
		WallActorInstance->Destroy();
		WallActorInstance = nullptr;
	}

	if (CurrFloorActor && bFloorMove)
	{
		CurrFloorActor->Destroy();
		CurrFloorActor = nullptr;
	}

	if (CurrInteriorActor && bInteriorMove)
	{
		bInteriorMove = false;
		CurrInteriorActor->Destroy();
		CurrInteriorActor = nullptr;
	}

	if (IsValid(CurrFloorActor)) {
		if (CurrFloorActor->TypeOfActor == "Floor")
			CurrFloorActor->ProceduralMeshFloor->SetRenderCustomDepth(false);
		else
			CurrFloorActor->ProceduralMeshRoof->SetRenderCustomDepth(false);

		CurrFloorActor = nullptr;
	}
	if (IsValid(WallActorInstance)) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		WallActorInstance = nullptr;
	}
	if (IsValid(CurrRoadActor)) {
		CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrRoadActor = nullptr;
	}
	if (IsValid(ClickedComponent)) {
		ClickedComponent->SetRenderCustomDepth(false);
		ClickedComponent = nullptr;
	}

	if (Mode == "ViewMode") {
	
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "ajsdkh");
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(TemplateMappingContext, 0);
	
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
			BuildingWidgetInstance->OpenDoorButton->SetVisibility(ESlateVisibility::Collapsed);

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
	if(WallActorInstance){
	bWallMove = false;
		if (bWallGenearting) {
			WallActorInstance = nullptr;
			bWallGenearting = false;
		}
		else {
			bWallGenearting = true;
		}
	}
}

void AArchVizController::WallRClick()
{
	if (IsValid(WallActorInstance)) {
		FRotator Rotation = WallActorInstance->GetActorRotation();
		Rotation.Yaw += 90;
		WallActorInstance->SetActorRotation(Rotation);
	}
}

void AArchVizController::OnWallSegmentsChanged(float Segments)
{
	if (IsValid(WallActorInstance))
	{
	
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
	if (IsValid(WallActorInstance))
		bWallMove = true;
	else if (IsValid(CurrFloorActor))
		bFloorMove = true;

}

void AArchVizController::DeleteClicked()
{

	if (WallActorInstance && TypeOfComponent == EBuildingComponentType::Wall) {
		WallActorInstance->Destroy();
		WallActorInstance = nullptr;
	}
	else if (CurrFloorActor && (TypeOfComponent == EBuildingComponentType::Floor || TypeOfComponent == EBuildingComponentType::Roof)) {
		CurrFloorActor->Destroy();
		CurrFloorActor = nullptr;
	}
	else if (WallActorInstance && TypeOfComponent == EBuildingComponentType::Wall) {
		WallActorInstance->Destroy();
		WallActorInstance = nullptr;
	}
	else if (CurrOffsetActor && TypeOfComponent == EBuildingComponentType::None) {
		CurrOffsetActor->Destroy();
		CurrOffsetActor = nullptr;
	}
	else if (ClickedComponent && TypeOfComponent == EBuildingComponentType::Door) {
		if(ClickedComponent->GetMaterials().Num()>1){
			UStaticMesh* WallMesh = LoadObject<UStaticMesh>(this, TEXT("/Script/Engine.StaticMesh'/Game/_GENERATED/ZTI/WallSegment.WallSegment'"));
			UMaterialInterface* WallMaterial = ClickedComponent->GetMaterial(3);
			BuildingWidgetInstance->DoorToggle->SetText(FText::FromString("Open Door"));

			ClickedComponent->SetStaticMesh(WallMesh);
			ClickedComponent->SetMaterial(0, WallMaterial);
		}
		
	}
	SetCustomText(FText::FromString("Deleted Sucessfully"));
}

void AArchVizController::FloorLeftClick()
{
	if (CurrFloorActor) {
		bFloorMove = false;
		if (bFloorGenerating) {
			CurrFloorActor = nullptr;
			bFloorGenerating = false;
		}
		else {
			bFloorGenerating = true;
		}
	}

}

void AArchVizController::OnFloorLengthChanged(float Length)
{
	if (IsValid(CurrFloorActor))
	{
		FVector Dim = CurrFloorActor->FDimentions;
		Dim.X = Length ;
		if(CurrFloorActor->TypeOfActor== "Floor")
		    CurrFloorActor->GenerateFloor(Dim);
		else
			CurrFloorActor->GenerateRoof(Dim);

	}
}

void AArchVizController::OnFloorWidthChanged(float Width)
{
	if (IsValid(CurrFloorActor))
	{
		FVector Dim = CurrFloorActor->FDimentions;
		Dim.Y = Width;
		if (CurrFloorActor->TypeOfActor == "Floor")
			CurrFloorActor->GenerateFloor(Dim);
		else
			CurrFloorActor->GenerateRoof(Dim);
	}
}

void AArchVizController::RClicked()
{
	if (WallActorInstance && TypeOfComponent == EBuildingComponentType::Wall) {
		FRotator Rotation_ = WallActorInstance->GetActorRotation();
		Rotation_.Yaw += 90;
		WallActorInstance->SetActorRotation(Rotation_);
	}
	else if (CurrFloorActor && (TypeOfComponent == EBuildingComponentType::Floor || TypeOfComponent == EBuildingComponentType::Roof)) {
		FRotator Rotation_ = CurrFloorActor->GetActorRotation();
		Rotation_.Yaw += 90;
		CurrFloorActor->SetActorRotation(Rotation_);
	}
	else if (WallActorInstance && TypeOfComponent == EBuildingComponentType::Wall) {
		FRotator Rotation_ = WallActorInstance->GetActorRotation();
		Rotation_.Yaw += 90;
		WallActorInstance->SetActorRotation(Rotation_);
	}
	else if (CurrOffsetActor && TypeOfComponent == EBuildingComponentType::None) {
		FRotator Rotation_ = CurrOffsetActor->GetActorRotation();
		Rotation_.Yaw += 90;
		CurrOffsetActor->SetActorRotation(Rotation_);
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
			if (ClickedComponent->GetMaterials().Num() == 4)
			{
				CurrWallMaterial = ClickedComponent->GetMaterial(3);
				BuildingWidgetInstance->OpenDoorButton->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				CurrWallMaterial = ClickedComponent->GetMaterial(0);
				BuildingWidgetInstance->OpenDoorButton->SetVisibility(ESlateVisibility::Collapsed);
			}

			ClickedComponent->SetRenderCustomDepth(true);
			ClickedComponent->CustomDepthStencilValue = 2.0;
			BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AArchVizController::OpenDoorButtonClick()
{
	if (ClickedComponent)
	{
		TArray<UMaterialInterface*> MArray = ClickedComponent->GetMaterials();
		UStaticMesh* DoorMesh;
		if (bDoorOpen)
		{
			DoorMesh = LoadObject<UStaticMesh>(this, TEXT("/Script/Engine.StaticMesh'/Game/Doors/White.White'"));
			BuildingWidgetInstance->DoorToggle->SetText(FText::FromString("Open Door"));
			bDoorOpen = false;

		}
		else {
			DoorMesh = LoadObject<UStaticMesh>(this, TEXT("/Script/Engine.StaticMesh'/Game/MyOpenDoor.MyOpenDoor'"));
			BuildingWidgetInstance->DoorToggle->SetText(FText::FromString("Close Door"));
			bDoorOpen = true;
		}
		if (DoorMesh) {
			ClickedComponent->SetStaticMesh(DoorMesh);
			for (int i = 0; i < MArray.Num(); i++) {
				if (MArray[i])
					ClickedComponent->SetMaterial(i, MArray[i]);
			}
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
			BuildingWidgetInstance->DoorToggle->SetText(FText::FromString("Open Door"));
			UMaterialInterface* CurrFrameMaterial = DoorMesh->GetStaticMaterials()[0].MaterialInterface;
			UMaterialInterface* CurrDoorMaterial = DoorMesh->GetStaticMaterials()[1].MaterialInterface;
			ClickedComponent->SetStaticMesh(DoorMesh);
			if (CurrWallMaterial)
				ClickedComponent->SetMaterial(3, CurrWallMaterial);
			if (CurrFrameMaterial)
				ClickedComponent->SetMaterial(0, CurrFrameMaterial);
			if (CurrDoorMaterial)
				ClickedComponent->SetMaterial(1, CurrDoorMaterial);

			ClickedComponent->SetRenderCustomDepth(false);

		}
		ClickedComponent = nullptr;
	}
}

void AArchVizController::OnFloorButtonClicked()
{
	//bMove_ModifyMode = false;
	if ((TypeOfComponent != EBuildingComponentType::None) && WallActorInstance && bWallMove)
	{
		WallActorInstance->Destroy();
		WallActorInstance = nullptr;
		bWallMove = false;
	}

	//bMove_ModifyMode = false;
	if ((TypeOfComponent != EBuildingComponentType::None) && CurrFloorActor && bFloorMove)
	{
		CurrFloorActor->Destroy();
		CurrFloorActor = nullptr;
		bFloorMove = false;
	}

	if (IsValid(CurrFloorActor)) {
		if (CurrFloorActor->TypeOfActor == "Floor")
			CurrFloorActor->ProceduralMeshFloor->SetRenderCustomDepth(false);
		else
			CurrFloorActor->ProceduralMeshRoof->SetRenderCustomDepth(false);

		CurrFloorActor = nullptr;
	}

	TypeOfComponent = EBuildingComponentType::Floor;

	if (IsValid(WallActorInstance)) {
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
	


	}



	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrFloorActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (IsValid(CurrFloorActor))
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
		BuildingWidgetInstance->FLength->SetValue(CurrFloorActor->GetActorRelativeScale3D().X * 100);
		BuildingWidgetInstance->FWidth->SetValue(CurrFloorActor->GetActorRelativeScale3D().Y * 100);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {

			FVector Location = HitResult.Location;

			CurrFloorActor->GenerateFloor(FVector(100, 100, 10));
			CurrFloorActor->SetActorLocation(Location);

		}
		else {
			SetCustomText(FText::FromString("Floor Not Generated Click Again With Go Near To House"));
			if (IsValid(CurrFloorActor)) {
				CurrFloorActor->Destroy();
				CurrFloorActor = nullptr;
			}

		}
	}
}

void AArchVizController::OnWallButtonClicked()
{
	//bMove_ModifyMode = false;
	if ((TypeOfComponent != EBuildingComponentType::None) && CurrFloorActor && bFloorMove)
	{
		CurrFloorActor->Destroy();
		CurrFloorActor = nullptr;
		bFloorMove = false;
	}

	if ((TypeOfComponent != EBuildingComponentType::None) && WallActorInstance && bWallMove)
	{
		WallActorInstance->Destroy();
		WallActorInstance = nullptr;
		bWallMove = false;
	}

	TypeOfComponent = EBuildingComponentType::Wall;

	if (IsValid(CurrFloorActor)) {
		if (CurrFloorActor->TypeOfActor == "Floor")
			CurrFloorActor->ProceduralMeshFloor->SetRenderCustomDepth(false);
		else
			CurrFloorActor->ProceduralMeshRoof->SetRenderCustomDepth(false);

		CurrFloorActor = nullptr;
	}

	if (IsValid(WallActorInstance)) {
		for (auto MeshComponent : WallActorInstance->StaticMeshComponentArr) {
			if (MeshComponent) {
				MeshComponent->SetRenderCustomDepth(false);
			}
		}
		WallActorInstance = nullptr;
	}
	if (BuildingWidgetInstance) {
		
		BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Collapsed);
		
		BuildingWidgetInstance->VerticalBox_Modify->SetVisibility(ESlateVisibility::Hidden);
	}


	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	WallActorInstance = GetWorld()->SpawnActor<AWallActor>(WallActor, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (WallActorInstance && TypeOfComponent == EBuildingComponentType::Wall)
	{
		WallActorInstance->GenerateWall(1);
		BuildingWidgetInstance->NoOfSegments->SetValue(1);
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
		WallActorInstance = nullptr;
		bWallMove = false;
	}
	if ((TypeOfComponent != EBuildingComponentType::None) && CurrFloorActor && bFloorMove)
	{
		CurrFloorActor->Destroy();
		CurrFloorActor = nullptr;
		bFloorMove = false;
	}

	TypeOfComponent = EBuildingComponentType::Roof;

	if (IsValid(CurrFloorActor)) {
		if (CurrFloorActor->TypeOfActor == "Floor")
			CurrFloorActor->ProceduralMeshFloor->SetRenderCustomDepth(false);
		else
			CurrFloorActor->ProceduralMeshRoof->SetRenderCustomDepth(false);

		CurrFloorActor = nullptr;
	}
	if (IsValid(WallActorInstance)) {
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
		

	}



	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrFloorActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (IsValid(CurrFloorActor))
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
		BuildingWidgetInstance->FLength->SetValue(CurrFloorActor->GetActorRelativeScale3D().X * 100);
		BuildingWidgetInstance->FWidth->SetValue(CurrFloorActor->GetActorRelativeScale3D().Y * 100);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {

			FVector Location = HitResult.Location;
			CurrFloorActor->GenerateRoof(FVector(100, 100, 10));
			CurrFloorActor->SetActorLocation(Location);

		}
		else {
			SetCustomText(FText::FromString("Roof Not Generated Click Again With Go Near To House"));
			if (IsValid(CurrFloorActor)) {
				CurrFloorActor->Destroy();
				CurrFloorActor = nullptr;
			}

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
	


	}

	if ((TypeOfComponent != EBuildingComponentType::None) && WallActorInstance && bWallMove)
	{
		WallActorInstance->Destroy();
		WallActorInstance = nullptr;
		bWallMove = false;
	}
	if ((TypeOfComponent != EBuildingComponentType::None) && CurrFloorActor && bFloorMove)
	{
		CurrFloorActor->Destroy();
		CurrFloorActor = nullptr;
		bFloorMove = false;
	}
	TypeOfComponent = EBuildingComponentType::Door;
	if (IsValid(CurrFloorActor)) {
		if (CurrFloorActor->TypeOfActor == "Floor")
			CurrFloorActor->ProceduralMeshFloor->SetRenderCustomDepth(false);
		else
			CurrFloorActor->ProceduralMeshRoof->SetRenderCustomDepth(false);

		CurrFloorActor = nullptr;
	}
	if (IsValid(WallActorInstance)) {
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
	if (IsValid(CurrFloorActor)) {
		if (CurrFloorActor->TypeOfActor == "Floor")
			CurrFloorActor->ProceduralMeshFloor->SetRenderCustomDepth(false);
		else
			CurrFloorActor->ProceduralMeshRoof->SetRenderCustomDepth(false);

		CurrFloorActor = nullptr;
	}
	if (IsValid(WallActorInstance)) {
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
	if (IsValid(WallActorInstance)) {

	
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
			BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Visible);
			BuildingWidgetInstance->NoOfSegments->SetValue(float(WallActorInstance->NoOfSegments));
			BuildingWidgetInstance->XOffset->SetValue(0);
			BuildingWidgetInstance->YOffset->SetValue(0);
			BuildingWidgetInstance->ZOffset->SetValue(0);
		}
	}
	else if (IsValid(CurrFloorActor)) {

		CurrOffsetActor = CurrFloorActor;
		/*CurrFloorActor = CurrRoofFloorActor;
		TypeOfComponent = EBuildingComponentType::Floor;*/



		if (CurrFloorActor->TypeOfActor == "Floor")
		{
			CurrFloorActor->ProceduralMeshFloor->SetRenderCustomDepth(true);
			CurrFloorActor->ProceduralMeshFloor->CustomDepthStencilValue = 2.0;
		}
		else
		{
			CurrFloorActor->ProceduralMeshRoof->SetRenderCustomDepth(true);
			CurrFloorActor->ProceduralMeshRoof->CustomDepthStencilValue = 2.0;
		}






		if (BuildingWidgetInstance) {
			BuildingWidgetInstance->VerticalBox_Modify->SetVisibility(ESlateVisibility::Visible);
			BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Visible);
			BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Visible);
			BuildingWidgetInstance->XOffset->SetValue(0);
			BuildingWidgetInstance->YOffset->SetValue(0);
			BuildingWidgetInstance->ZOffset->SetValue(0);
			BuildingWidgetInstance->FLength->SetValue(CurrFloorActor->FDimentions.X);
			BuildingWidgetInstance->FWidth->SetValue(CurrFloorActor->FDimentions.Y);

		}

	}
}

void AArchVizController::OnModifyComponentButtonClicked()
{
	SubSystem->ClearAllMappings();
	SubSystem->AddMappingContext(ModifyComponentMappingContext, 0);

	if (WallActorInstance && bWallMove)
	{
		WallActorInstance->Destroy();
		WallActorInstance = nullptr;
	}
	if (CurrFloorActor && bFloorMove)
	{
		CurrFloorActor->Destroy();
		CurrFloorActor = nullptr;
	}

	TypeOfComponent = EBuildingComponentType::None;
	if (BuildingWidgetInstance)
	{
		BuildingWidgetInstance->SetPlayerController(this);
		BuildingWidgetInstance->AddToViewport();
		BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorL->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->Border_FloorW->SetVisibility(ESlateVisibility::Collapsed);
		BuildingWidgetInstance->DoorMeshBox->SetVisibility(ESlateVisibility::Collapsed);
	
		BuildingWidgetInstance->Border_Seg->SetVisibility(ESlateVisibility::Hidden);


	}
}

void AArchVizController::InteriorLeftClick()
{
	if (bInteriorMove) {
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

			bool bCon = (((InteriorType == "Floor" && HitResult.ImpactNormal.Z > 0)) || (InteriorType == "Roof" && HitResult.ImpactNormal.Z < 0));

			if ((bCon && FActor) || (WActor && InteriorType == "Wall")) {
				bInteriorMove = false;
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Interior Left Click");
			}

		}

	}
	else {
		FCollisionQueryParams TraceParams;
		TraceParams.bTraceComplex = true;
		TraceParams.AddIgnoredActor(this);


		FHitResult HitResult;
		FVector CursorWorldLocation;
		FVector CursorWorldDirection;
		DeprojectMousePositionToWorld(CursorWorldLocation, CursorWorldDirection);

		if (GetWorld()->LineTraceSingleByChannel(HitResult, CursorWorldLocation, CursorWorldLocation + CursorWorldDirection * 10000, ECC_Visibility, TraceParams)) {
			AInteriorActor* IActor = Cast<AInteriorActor>(HitResult.GetActor());

			if (IActor) {
				InteriorType = IActor->TypeOfInterior;
				CurrInteriorActor = IActor;
				bInteriorMove = true;
			}
		}
	}




}

void AArchVizController::InteriorRClick()
{
	if (IsValid(CurrInteriorActor)) {
		FRotator Rotation = CurrInteriorActor->GetActorRotation();
		Rotation.Yaw += 90;
		CurrInteriorActor->SetActorRotation(Rotation);
	}
}

void AArchVizController::InteriorDeleteClick()
{
	if (CurrInteriorActor) {
		CurrInteriorActor->Destroy();
		CurrInteriorActor = nullptr;
	}
	bInteriorMove = false;
	SetCustomText(FText::FromString("Deleted Sucessfully" ));
}

void AArchVizController::InteriorFloorGenerator(const FInteriorFloorMeshData& InteriorFloorMeshData)
{
	if (CurrInteriorActor && bInteriorMove)
	{
		CurrInteriorActor->Destroy();
		CurrInteriorActor = nullptr;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (IsValid(CurrInteriorActor)) {
		CurrInteriorActor->TypeOfInterior = "Floor";
		InteriorType = CurrInteriorActor->TypeOfInterior;
		bInteriorMove = true;
		CurrInteriorActor->GenerateInterior(InteriorFloorMeshData.InteriorFloorMesh);
	}
}
void AArchVizController::InteriorRoofGenerator(const FInteriorRoofMeshData& InteriorRoofMeshData)
{
	if (CurrInteriorActor && bInteriorMove)
	{
		CurrInteriorActor->Destroy();
		CurrInteriorActor = nullptr;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (IsValid(CurrInteriorActor)) {
		CurrInteriorActor->TypeOfInterior = "Roof";
		InteriorType = CurrInteriorActor->TypeOfInterior;

		bInteriorMove = true;
		CurrInteriorActor->GenerateInterior(InteriorRoofMeshData.InteriorRoofMesh);
	}
}
void AArchVizController::InteriorWallGenerator(const FInteriorWallMeshData& InteriorWallMeshData)
{
	if (CurrInteriorActor && bInteriorMove)
	{
		CurrInteriorActor->Destroy();
		CurrInteriorActor = nullptr;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	CurrInteriorActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (IsValid(CurrInteriorActor)) {
		CurrInteriorActor->TypeOfInterior = "Wall";
		InteriorType = CurrInteriorActor->TypeOfInterior;

		bInteriorMove = true;
		CurrInteriorActor->GenerateInterior(InteriorWallMeshData.InteriorWallMesh);
	}
}

void AArchVizController::MaterialLeftClick()
{
	//bMove_ModifyMode = false;
	bWallMove = false;
	bFloorMove = false;

	if (MaterialWidgetInstance) {
		MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Collapsed);
		MaterialWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (IsValid(CurrRoadActor)) {
		CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(false);
		CurrRoadActor = nullptr;

	}
	if (IsValid(CurrFloorActor)) {
		if (CurrFloorActor->TypeOfActor == "Floor")
			CurrFloorActor->ProceduralMeshFloor->SetRenderCustomDepth(false);
		else
			CurrFloorActor->ProceduralMeshRoof->SetRenderCustomDepth(false);

		CurrFloorActor = nullptr;
	}
	if (IsValid(WallActorInstance)) {
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

	if (IsValid(WallActorInstance)) {

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
	else if (IsValid(CurrFloorActor)) {


		if (CurrFloorActor->TypeOfActor == "Floor")
		{
			CurrFloorActor->ProceduralMeshFloor->SetRenderCustomDepth(true);
			CurrFloorActor->ProceduralMeshFloor->CustomDepthStencilValue = 2.0;
		}
		else
		{
			CurrFloorActor->ProceduralMeshRoof->SetRenderCustomDepth(true);
			CurrFloorActor->ProceduralMeshRoof->CustomDepthStencilValue = 2.0;
		}

		if (MaterialWidgetInstance) {
			MaterialWidgetInstance->BuildingMaterialBox->SetVisibility(ESlateVisibility::Visible);
		}

	}
	else if (IsValid(CurrRoadActor)) {

		CurrRoadActor->ProceduralMeshRoot->SetRenderCustomDepth(true);
		CurrRoadActor->ProceduralMeshRoot->CustomDepthStencilValue = 2.0;

		if (MaterialWidgetInstance) {
			MaterialWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Visible);
		}

	}
}


void AArchVizController::SaveGame(FString Name)
{
	UArchVizSaveGame* SaveGameInstance = Cast<UArchVizSaveGame>(UGameplayStatics::CreateSaveGameObject(UArchVizSaveGame::StaticClass()));

	if (SaveGameInstance)
	{
		TArray<AActor*> AllRoadActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoadActor::StaticClass(), AllRoadActors);
		for (AActor* Actor : AllRoadActors)
		{
			ARoadActor* CurRoadActor = Cast<ARoadActor>(Actor);
			if (CurRoadActor)
			{
				FRoadActorData RoadData;
				RoadData.RoadTransform = CurRoadActor->GetActorTransform();
				RoadData.Dimenstion = CurRoadActor->RoadDimensions;
				RoadData.Material = CurRoadActor->ProceduralMeshRoot->GetMaterial(0);
				SaveGameInstance->RoadActorArray.Add(RoadData);
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Road Saved");
			}
		}

		// Save Wall Actors
		TArray<AActor*> AllWallActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWallActor::StaticClass(), AllWallActors);
		for (AActor* Actor : AllWallActors)
		{
			AWallActor* CurWallActor = Cast<AWallActor>(Actor);
			if (CurWallActor)
			{
				FWallActorData WallData;
				WallData.WallTransform = CurWallActor->GetActorTransform();
				for (UStaticMeshComponent* MeshComponent : CurWallActor->StaticMeshComponentArr)
				{
					WallData.WallWarray.Add(MeshComponent->GetStaticMesh());
				}

				if (CurWallActor->StaticMeshComponentArr[0]->GetMaterials().Num() == 4) {
					WallData.Material = CurWallActor->StaticMeshComponentArr[0]->GetMaterial(3);
				}
				else {
					WallData.Material = CurWallActor->StaticMeshComponentArr[0]->GetMaterial(0);
				}

				SaveGameInstance->WallActorArray.Add(WallData);
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Saved Wall");
			}
		}

		TArray<AActor*> AllFloorActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFloorActor::StaticClass(), AllFloorActors);
		for (AActor* Actor : AllFloorActors)
		{
			AFloorActor* CurFloorActor = Cast<AFloorActor>(Actor);
			if (CurFloorActor)
			{
				FFloorActorData FloorData;
				FloorData.FloorTransform = CurFloorActor->GetActorTransform();
				FloorData.Dimenstion = CurFloorActor->FDimentions;
				FloorData.ActorType = CurFloorActor->TypeOfActor;
				if (CurFloorActor->TypeOfActor == "Floor")
					FloorData.Material = CurFloorActor->ProceduralMeshFloor->GetMaterial(0);
				else
					FloorData.Material = CurFloorActor->ProceduralMeshRoof->GetMaterial(0);

				SaveGameInstance->FloorActorArray.Add(FloorData);
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Floor Saved");
			}
		}

		TArray<AActor*> AllInteriorActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteriorActor::StaticClass(), AllInteriorActors);
		for (AActor* Actor : AllInteriorActors)
		{
			AInteriorActor* CurInteriorActor = Cast<AInteriorActor>(Actor);
			if (CurInteriorActor)
			{
				FInteriorActorData InteriorData;
				InteriorData.InteriorTransform = CurInteriorActor->GetActorTransform();
				InteriorData.StaticMesh = CurInteriorActor->InteriorStaticMesh;
				InteriorData.InteriorType = CurInteriorActor->TypeOfInterior;
				SaveGameInstance->InteriorActorArray.Add(InteriorData);
				//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Interior Saved");
			}
		}



		UGameplayStatics::SaveGameToSlot(SaveGameInstance, Name, 0);
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Game Saved Successfully");
	}
}

void AArchVizController::LoadGame(FString Name)
{
	UArchVizSaveGame* LoadGameInstance = Cast<UArchVizSaveGame>(UGameplayStatics::LoadGameFromSlot(Name, 0));
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	IgonoreActors.Empty();
    CurrTemplateActor =  GetWorld()->SpawnActor<ATemplateActor>(ATemplateActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	bTemplateMove = true;
	CurrTemplateActor->SetActorLocation(LoadGameInstance->WallActorArray[0].WallTransform.GetLocation());
	if (LoadGameInstance)
	{
		// Clear existing Road Actors
		TArray<AActor*> AllRoadActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoadActor::StaticClass(), AllRoadActors);
		for (AActor* Actor : AllRoadActors)
		{
			Actor->Destroy();
		}


		TArray<AActor*> AllWallActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWallActor::StaticClass(), AllWallActors);
		for (AActor* Actor : AllWallActors)
		{
			Actor->Destroy();
		}

		TArray<AActor*> AllInteriorActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AInteriorActor::StaticClass(), AllInteriorActors);
		for (AActor* Actor : AllInteriorActors)
		{
			Actor->Destroy();
		}

		TArray<AActor*> AllFloorActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFloorActor::StaticClass(), AllFloorActors);
		for (AActor* Actor : AllFloorActors)
		{
			Actor->Destroy();
		}


		// Load Road Actors
		for (const FRoadActorData& RoadData : LoadGameInstance->RoadActorArray)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Load Roads");
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			ARoadActor* SpawnActor = GetWorld()->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

			if (SpawnActor)
			{
				SpawnActor->SetActorTransform(RoadData.RoadTransform);
				SpawnActor->GenerateRoad(RoadData.Dimenstion);
				SpawnActor->ProceduralMeshRoot->SetMaterial(0, RoadData.Material);
			}
			SpawnActor->AttachToActor(CurrTemplateActor, FAttachmentTransformRules::KeepWorldTransform);
			IgonoreActors.Add(SpawnActor);
		}

		// Load Wall Actors
		for (const FWallActorData& WallData : LoadGameInstance->WallActorArray)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Load Walls");
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AWallActor* SpawnActor = GetWorld()->SpawnActor<AWallActor>(AWallActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

			if (SpawnActor)
			{
				SpawnActor->SetActorTransform(WallData.WallTransform);
				SpawnActor->DestroyWalls();

				for (int32 i = 0; i < WallData.WallWarray.Num(); i++)
				{
					UStaticMesh* Mesh = WallData.WallWarray[i];
					if (Mesh)
					{
						UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(SpawnActor);
						float length = Mesh->GetBounds().GetBox().GetSize().X;
						MeshComponent->SetStaticMesh(Mesh);
						MeshComponent->RegisterComponent();
						MeshComponent->AttachToComponent(SpawnActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

						MeshComponent->SetRelativeLocation(FVector(i * length, 0, 0));


						SpawnActor->StaticMeshComponentArr.Add(MeshComponent);
						if (SpawnActor->StaticMeshComponentArr[i]->GetMaterials().Num() == 4) {
							SpawnActor->StaticMeshComponentArr[i]->SetMaterial(3, WallData.Material);
						}
						else {
							SpawnActor->StaticMeshComponentArr[i]->SetMaterial(0, WallData.Material);
						}

					}
				}
				SpawnActor->AttachToActor(CurrTemplateActor, FAttachmentTransformRules::KeepWorldTransform);
				IgonoreActors.Add(SpawnActor);


			}
		}

		for (const FFloorActorData& FloorData : LoadGameInstance->FloorActorArray)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Load Floors");
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AFloorActor* SpawnActor = GetWorld()->SpawnActor<AFloorActor>(AFloorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

			if (SpawnActor)
			{
				if (FloorData.ActorType == "Floor") {
					SpawnActor->GenerateFloor(FloorData.Dimenstion);
					SpawnActor->ProceduralMeshFloor->SetMaterial(0, FloorData.Material);

				}
				else {
					SpawnActor->GenerateRoof(FloorData.Dimenstion);
					SpawnActor->ProceduralMeshRoof->SetMaterial(0, FloorData.Material);

				}
				SpawnActor->SetActorTransform(FloorData.FloorTransform);
				SpawnActor->AttachToActor(CurrTemplateActor, FAttachmentTransformRules::KeepWorldTransform);
				IgonoreActors.Add(SpawnActor);

				//SpawnActor->ProceduralMeshRoot->SetMaterial(0, FloorData.Material);
			}
		}

		for (const FInteriorActorData& InteriorData : LoadGameInstance->InteriorActorArray)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "Load Interiors");
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AInteriorActor* SpawnActor = GetWorld()->SpawnActor<AInteriorActor>(AInteriorActor::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, Params);

			if (SpawnActor)
			{
				SpawnActor->TypeOfInterior = InteriorData.InteriorType;
				SpawnActor->GenerateInterior(InteriorData.StaticMesh);
				SpawnActor->SetActorTransform(InteriorData.InteriorTransform);
				SpawnActor->AttachToActor(CurrTemplateActor, FAttachmentTransformRules::KeepWorldTransform);
				IgonoreActors.Add(SpawnActor);

			}
		}
	}
}

void AArchVizController::TemplateLeftClick()
{
	if (bTemplateMove) {
		bTemplateMove = false;
	}
	else {
		bTemplateMove = true;

	}
}

void AArchVizController::TemplateRClick()
{
	if (CurrTemplateActor) {
		FRotator Rot = CurrTemplateActor->GetActorRotation();
		Rot.Yaw += 90;
		CurrTemplateActor->SetActorRotation(Rot);
	}
}

void AArchVizController::OnInstructionButtonClicked()
{
	FTimerHandle TimeHandle;
	ArchVizWidgetInstance->InstructionBox->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AArchVizController::HideInstructionVisibility, 5, false);
}

void AArchVizController::HideInstructionVisibility()
{
	ArchVizWidgetInstance->InstructionBox->SetVisibility(ESlateVisibility::Hidden);
	ArchVizWidgetInstance->CustomTextBorder->SetVisibility(ESlateVisibility::Hidden);
}

void AArchVizController::SetCustomText(FText Custom)
{
	ArchVizWidgetInstance->CustomText->SetText(Custom);
	ArchVizWidgetInstance->CustomTextBorder->SetVisibility(ESlateVisibility::Visible);
	FTimerHandle TimeHandle;
	GetWorld()->GetTimerManager().SetTimer(TimeHandle, this, &AArchVizController::HideInstructionVisibility, 2, false);
}


TArray<FString>  AArchVizController::FindFiles(FString Path, FString Extension)
{
		TArray<FString> FileList;
		IFileManager& FileManager = IFileManager::Get();

		if (!Path.EndsWith("/"))
		{
			Path += "/";
		}

		FString FullPath = Path + TEXT("*") + Extension;

		FileManager.FindFiles(FileList, *FullPath, true, false);

		return FileList;
}


