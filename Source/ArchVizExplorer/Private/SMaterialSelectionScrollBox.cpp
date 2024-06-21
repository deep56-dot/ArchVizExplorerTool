// Fill out your copyright notice in the Description page of Project Settings.


#include "SMaterialSelectionScrollBox.h"
#include "SlateOptMacros.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SMaterialSelectionScrollBox::Construct(const FArguments& InArgs)
{
	ParentVBox = SNew(SVerticalBox);
	ScrollBox = SNew(SScrollBox).Orientation(EOrientation::Orient_Horizontal);
	MaterialAsset = InArgs._InMaterialAsset;
	MeshType = InArgs._InMeshType;
	FSlateColorBrush* ParentBrush = new FSlateColorBrush(FLinearColor(0.955974f, 0.871367f, 0.590619f, 1.0f));
	TSharedPtr<SBorder> Border = SNew(SBorder).BorderImage(ParentBrush)[
		ParentVBox.ToSharedRef()
	];
	ChildSlot
		[
			Border.ToSharedRef()
		];
	if (MeshType == EAssetType::RoadMaterialData)
	{
		RefreshAssetRoadMaterialThubnail();
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "jlhasfdklisjhgfilvjsdgvdf");
	}
	else if(MeshType == EAssetType::DoorMeshData) {
		RefreshAssetDoorMeshThubnail();
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "jlhasfdkvdf");
	}
		
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMaterialSelectionScrollBox::RefreshAssetRoadMaterialThubnail()
{
	ScrollBox->ClearChildren();
	if (MaterialAsset.IsValid()) {
		ParentVBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(10)
			[
				SNew(STextBlock)
					.Text(FText::FromString("Road Materials"))
					.ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 25))
			];


		for (const auto& MeshData : MaterialAsset->RoadMaterialDataArray) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.RoadThumbnail);
			ThumbnailBrush->SetImageSize(FVector2D(200));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));
			TextBlock->SetText(FText::FromString(MeshData.RoadName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnRoadMaterialThubnailSelected.ExecuteIfBound(MeshData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});
			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.904661f, 0.651406f, 0.111932f, 1.0f));

			TSharedPtr<SBorder> ImageBorder = SNew(SBorder).BorderImage(BorderBrush).Padding(10)[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot()[
				ImageBorder.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight().Padding(10)
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().Padding(10).VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
		ParentVBox->AddSlot()[
			ScrollBox.ToSharedRef()
		];
	}
}

void SMaterialSelectionScrollBox::RefreshAssetDoorMeshThubnail()
{
	ScrollBox->ClearChildren();
	if (MaterialAsset.IsValid()) {

		ParentVBox->AddSlot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			.Padding(10)
			[
				SNew(STextBlock)
					.Text(FText::FromString("Door Data"))
					.ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 25))
			];


		for (const auto& MeshData : MaterialAsset->DoorMeshDataArray) {
			TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox);
			FSlateBrush* ThumbnailBrush = new FSlateBrush();
			ThumbnailBrush->SetResourceObject(MeshData.DoorThumbnail);
			ThumbnailBrush->SetImageSize(FVector2D(200));
			TSharedPtr<STextBlock> TextBlock = SNew(STextBlock).ColorAndOpacity(FColor::FromHex("#0d3b66")).Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20));
			TextBlock->SetText(FText::FromString(MeshData.DoorName));
			TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(ThumbnailBrush).Cursor(EMouseCursor::Hand).OnMouseButtonDown_Lambda([this, MeshData](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
				if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
					OnDoorMeshThubnailSelected.ExecuteIfBound(MeshData);
					return FReply::Handled();
				}
				return FReply::Unhandled();
				});
			FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.904661f, 0.651406f, 0.111932f, 1.0f));

			TSharedPtr<SBorder> ImageBorder = SNew(SBorder).BorderImage(BorderBrush).Padding(10)[
				ThumbnailImage.ToSharedRef()
			];
			VerticalBox->AddSlot()[
				ImageBorder.ToSharedRef()
			];
			VerticalBox->AddSlot().HAlign(EHorizontalAlignment::HAlign_Center).AutoHeight().Padding(10)
				[
					TextBlock.ToSharedRef()
				];

			ScrollBox->AddSlot().Padding(10).VAlign(EVerticalAlignment::VAlign_Center)
				[
					VerticalBox.ToSharedRef()
				];
		}
		ParentVBox->AddSlot()[
			ScrollBox.ToSharedRef()
		];
	}
}
