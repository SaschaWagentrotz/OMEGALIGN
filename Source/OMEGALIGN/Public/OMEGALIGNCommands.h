// Copyright 2023 Sascha Wagentrotz All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "OMEGALIGNStyle.h"
#include "OMEGALIGNSettings.h"

class OMEGALIGN_API FOMEGALIGNCommands : public TCommands<FOMEGALIGNCommands>
{
public:

	FOMEGALIGNCommands()
		: TCommands<FOMEGALIGNCommands>(TEXT("OMEGALIGN"), NSLOCTEXT("Contexts", "OMEGALIGN", "OMEGALIGN Plugin"), NAME_None, FOMEGALIGNStyle::GetStyleSetName())
	{
	}
	
	virtual void RegisterCommands() override;

	static TSharedRef<FUICommandList> ActionList;
	
	TSharedPtr<FUICommandInfo> PluginAction;
	TSharedPtr<FUICommandInfo> AlignTransform;
	TSharedPtr<FUICommandInfo> AlignRotation;
	TSharedPtr<FUICommandInfo> AlignLocation;
	TSharedPtr<FUICommandInfo> AlignScale;
	TSharedPtr<FUICommandInfo> ApplyRandomRotation;
	TSharedPtr<FUICommandInfo> IncrementTransform;
	TSharedPtr<FUICommandInfo> LookAtFirst;
	TSharedPtr<FUICommandInfo> LookAtLast;
	TSharedPtr<FUICommandInfo> DistributeRandomly;
	TSharedPtr<FUICommandInfo> DuplicateToView;
	TSharedPtr<FUICommandInfo> ArrayDuplication;
	TSharedPtr<FUICommandInfo> SelectActorsNearCursor;
	TSharedPtr<FUICommandInfo> MultiSelectActorsNearCursor;
	TSharedPtr<FUICommandInfo> DeselectActorsNearCursor;
	TSharedPtr<FUICommandInfo> SelectSimilarInRange;

	static void AlignTransformFunction(bool AlignRotation, bool AlignLocation, bool AlginScale);
	static FRotator GetRandomRotator();
	static TArray<AActor*> GetSelectedActors();
	static FVector GetRandomPointInCircle(double Radius, FVector Center);
	static bool TryDistributeRandomly(AActor* CenterActor, AActor* ActorToPlace, TArray<AActor*> SelectedActors, TArray<AActor*> PlacedActors);
	static UOMEGALIGNSettings* GetPluginSettings();
	static void DuplicateToMouseOrView(bool ForceCameraMode);
	static void ActorNearCursorSelection(bool IsMultiSelect);
	
	class Callbacks
	{
	public:

		static void PluginAction();
		static bool CanPluginAction();
		
		static void AlignTransform();
		static bool CanAlignTransform();

		static void AlignRotation();
		static bool CanAlignRotation();

		static void AlignLocation();
		static bool CanAlignLocation();

		static void AlignScale();
		static bool CanAlignScale();

		static void ApplyRandomRotation();
		static bool CanApplyRandomRotation();
		
		static void IncrementTransform();
		static bool CanIncrementTransform();

		static void LookAtFirst();
		static bool CanLookAtFirst();
		static void LookAtLast();
		static bool CanLookAtLast();

		static void DistributeRandomly();
		static bool CanDistributeRandomly();

		static void DuplicateToView();
		static bool CanDuplicateToView();

		static void ArrayDuplication();
		static bool CanArrayDuplication();

		static void SelectActorsNearCursor();
		static bool CanSelectActorsNearCursor();

		static void MultiSelectActorsNearCursor();
		static bool CanMultiSelectActorsNearCursor();

		static void DeselectActorsNearCursor();
		static bool CanDeselectActorsNearCursor();
		
		static void SelectSimilarInRange();
		static bool CanSelectSimilarInRange();
	};
};
