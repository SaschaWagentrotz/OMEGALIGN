// Copyright 2023 Sascha Wagentrotz All Rights Reserved.

#include "OMEGALIGNCommands.h"

#include "AssetSelection.h"
#include "OMEGALIGNSettings.h"
#include "ISettingsModule.h"
#include "LevelEditorViewport.h"
#include "Framework/Commands/UIAction.h"
#include "Framework/Commands/UICommandList.h"
#include "Interfaces/IMainFrameModule.h"
#include "Selection.h"
#include "Engine/StaticMeshActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/TransformCalculus3D.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "EngineUtils.h"
#include "Landscape.h"
#include "ScopedTransaction.h"
#include "Framework/Application/SlateApplication.h"
#include "LandscapeProxy.h"
#include "OMEGALIGNSubsystem.h"

#define LOCTEXT_NAMESPACE "FOMEGALIGNModule"

TSharedRef<FUICommandList> FOMEGALIGNCommands::ActionList(new FUICommandList());

void FOMEGALIGNCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "Open OMEGALIGN Plugin Settings", "Open the OMEGALIGN Plugin Settings screen to adjust OMEGALIGN to your needs.", EUserInterfaceActionType::Button, FInputChord());

	UI_COMMAND(AlignTransform, "OMEGALIGN Transform", "Copies the specified transform values of the actor specified in the PluginSettings and applies these transform values to all other selected actors", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AlignRotation, "OMEGALIGN Rotation", "Copies the rotation value of the actor specified in the PluginSettings and applies that rotation value to all other selected actors", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AlignLocation, "OMEGALIGN Location", "Copies the location value of the actor specified in the PluginSettings and applies that location value to all other selected actors", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(AlignScale, "OMEGALIGN Scale", "Copies the scale value of the actor specified in the PluginSettings and applies that scale value to all other selected actors", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ApplyRandomRotation, "OMEGALIGN Random Rotation", "Randomly rotate all selected objects between the two values defined here", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(IncrementTransform, "OMEGALIGN Increment Transform", "Increment tranform values for every selected actor by the values specified in the OMEGALIGN Settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(LookAtFirst, "OMEGALIGN Look At First",  "Rotate all selected actors on the specified axis to look at the first selected actor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(LookAtLast, "OMEGALIGN Look At Last",  "Rotate all selected actors on the specified axis to look at the last selected actor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(DistributeRandomly, "OMEGALIGN Distribute Randomly",  "Uniformly distribute all selected actors randomly in a a circle with a specified radius around the first selected actor", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(DuplicateToView, "OMEGALIGN Duplicate to View",  "Duplicate all selected actors to the current editor camera view", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(ArrayDuplication, "OMEGALIGN Array Duplication",  "Duplicate the selected actors to an array, specified in the settings", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(SelectActorsNearCursor, "OMEGALIGN Select Actors near cursor",  "Select all actors within a specified range of the mouse to world projection", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(MultiSelectActorsNearCursor, "OMEGALIGN Multi-Select Actors near cursor",  "Multi-Select all actors within a specified range of the mouse to world projection. Saves actors over multiple commands", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(DeselectActorsNearCursor, "OMEGALIGN Deselect Actors near cursor",  "Deselect all actors within a specified range of the mouse to world projection. Also removes them from saved Multi-elect near cursor commands.", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(SelectSimilarInRange, "OMEGALIGN Select similar Actors in range",  "Select all actors of the same classes in specified range", EUserInterfaceActionType::Button, FInputChord());

	ActionList->MapAction(PluginAction, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::PluginAction), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanPluginAction));
	ActionList->MapAction(AlignTransform, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::AlignTransform), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanAlignTransform));
	ActionList->MapAction(AlignRotation, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::AlignRotation), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanAlignRotation));
	ActionList->MapAction(AlignLocation, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::AlignLocation), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanAlignLocation));
	ActionList->MapAction(AlignScale, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::AlignScale), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanAlignScale));
	ActionList->MapAction(ApplyRandomRotation, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::ApplyRandomRotation), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanApplyRandomRotation));
	ActionList->MapAction(IncrementTransform, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::IncrementTransform), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanIncrementTransform));
	ActionList->MapAction(LookAtFirst, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::LookAtFirst), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanLookAtFirst));
	ActionList->MapAction(LookAtLast, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::LookAtLast), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanLookAtLast));
	ActionList->MapAction(DistributeRandomly, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::DistributeRandomly), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanDistributeRandomly));
	ActionList->MapAction(DuplicateToView, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::DuplicateToView), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanDuplicateToView));
	ActionList->MapAction(ArrayDuplication, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::ArrayDuplication), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanArrayDuplication));
	ActionList->MapAction(SelectActorsNearCursor, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::SelectActorsNearCursor), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanSelectActorsNearCursor));
	ActionList->MapAction(MultiSelectActorsNearCursor, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::MultiSelectActorsNearCursor), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanMultiSelectActorsNearCursor));
	ActionList->MapAction(DeselectActorsNearCursor, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::DeselectActorsNearCursor), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanDeselectActorsNearCursor));
	ActionList->MapAction(SelectSimilarInRange, FExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::SelectSimilarInRange), FCanExecuteAction::CreateStatic(&FOMEGALIGNCommands::Callbacks::CanSelectSimilarInRange));
	
	IMainFrameModule& mainFrame = FModuleManager::Get().LoadModuleChecked<IMainFrameModule>("MainFrame");
	mainFrame.GetMainFrameCommandBindings()->Append(ActionList);
}

void FOMEGALIGNCommands::Callbacks::AlignTransform()
{
	switch (GetPluginSettings()->AlignTransformTo)
	{
		case LocationRotationScale:
			AlignTransformFunction(true, true, true);
			break;

		case LocationRotation:
			AlignTransformFunction(true, true, false);
			break;

		case LocationScale:
			AlignTransformFunction(false, true, true);
			break;

		case RotationScale:
			AlignTransformFunction(true, false, true);
			break;

		default:
			break;
	}
}

bool FOMEGALIGNCommands::Callbacks::CanAlignTransform()
{
	return true;
}


void FOMEGALIGNCommands::Callbacks::AlignRotation()
{
	AlignTransformFunction(true, false, false);
}

bool FOMEGALIGNCommands::Callbacks::CanAlignRotation()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::AlignLocation()
{
	AlignTransformFunction(false, true, false);
}

bool FOMEGALIGNCommands::Callbacks::CanAlignLocation()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::AlignScale()
{
	AlignTransformFunction(false, false, true);
}

bool FOMEGALIGNCommands::Callbacks::CanAlignScale()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::ApplyRandomRotation()
{
	TArray<AActor*> SelectedActors = GetSelectedActors();
	FRotator Rotator = GetRandomRotator();
	
	FScopedTransaction Transaction(LOCTEXT("OMEGALIGN Random Rotation", "OMEGALIGN Random Rotation"));

	for (AActor* Actor : SelectedActors)
	{
		Actor->Modify();

		if (GetPluginSettings()->RandomRotationPerActor)
		{
			Actor->SetActorRotation(GetRandomRotator());
		}
		else
		{
			Actor->SetActorRotation(Rotator);
		}

	}
}

bool FOMEGALIGNCommands::Callbacks::CanApplyRandomRotation()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::IncrementTransform()
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	TArray<AActor*> SelectedActors = GetSelectedActors();
	
	FScopedTransaction Transaction(LOCTEXT("OMEGALIGN Increment Transform", "OMEGALIGN Increment Transform"));

	for (int i = 0; i < SelectedActors.Num(); i++)
	{
		SelectedActors[i]->Modify();
		
		int ActorIndex = (PluginSettings->IncrementBasedOn.GetValue() == FirstSelectedActor) ? 0 : SelectedActors.Num()-1;
		
		FRotator CurrentRotation = UKismetMathLibrary::ComposeRotators(SelectedActors[ActorIndex]->GetActorRotation(), PluginSettings->RotationIncrement * i);
		SelectedActors[i]->SetActorRotation(CurrentRotation);

		FVector3d CurrentLocation = SelectedActors[ActorIndex]->GetActorLocation() + (PluginSettings->LocationIncrement * i);
		SelectedActors[i]->SetActorLocation(CurrentLocation);

		FVector3d CurrentScale = SelectedActors[ActorIndex]->GetActorScale() + (PluginSettings->ScaleIncrement * i);
		SelectedActors[i]->SetActorScale3D(CurrentScale);
	}
}

bool FOMEGALIGNCommands::Callbacks::CanIncrementTransform()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::LookAtFirst()
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	TArray<AActor*> SelectedActors = GetSelectedActors();
	TArray<AActor*> NewSelectedActors;

	if (GetSelectedActors().Num() > 0)
	{
		FScopedTransaction Transaction(LOCTEXT("OMEGALIGN LookAt First", "OMEGALIGN LookAt First"));
			
		for (int i = 0; i < SelectedActors.Num(); i++)
		{
			if (i != 0)
			{
				SelectedActors[i]->Modify();
				NewSelectedActors.Add(SelectedActors[i]);
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SelectedActors[i]->GetActorLocation(), SelectedActors[0]->GetActorLocation());
				FRotator OffsetLookAtRotation = UKismetMathLibrary::ComposeRotators(LookAtRotation, FRotator(0.0f, PluginSettings->FixedLookAtRotationOffset, 0.0f));
				SelectedActors[i]->SetActorRotation(OffsetLookAtRotation);
			}
		}
		if (PluginSettings->AfterLookAtOperation == DeselectTarget)
		{
			GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(NewSelectedActors);
		}
	}
}

bool FOMEGALIGNCommands::Callbacks::CanLookAtFirst()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::LookAtLast()
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	TArray<AActor*> SelectedActors = GetSelectedActors();
	TArray<AActor*> NewSelectedActors;

	if (GetSelectedActors().Num() > 0)
	{
		FScopedTransaction Transaction(LOCTEXT("OMEGALIGN LookAt Last", "OMEGALIGN LookAt Last"));
		
		for (int i = 0; i < SelectedActors.Num(); i++)
		{
			if (i != SelectedActors.Num()-1)
			{
				SelectedActors[i]->Modify();
				NewSelectedActors.Add(SelectedActors[i]);
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(SelectedActors[i]->GetActorLocation(), SelectedActors[SelectedActors.Num()-1]->GetActorLocation());
				FRotator OffsetLookAtRotation = UKismetMathLibrary::ComposeRotators(LookAtRotation, FRotator(0.0f, PluginSettings->FixedLookAtRotationOffset + UKismetMathLibrary::RandomFloatInRange(PluginSettings->AdditionalRandomRotationOffsetRange.X, PluginSettings->AdditionalRandomRotationOffsetRange.Y), 0.0f));
				SelectedActors[i]->SetActorRotation(OffsetLookAtRotation);
			}
		}
		if (PluginSettings->AfterLookAtOperation == DeselectTarget)
		{
			GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(NewSelectedActors);
		}
	}
}

bool FOMEGALIGNCommands::Callbacks::CanLookAtLast()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::DistributeRandomly()
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	TArray<AActor*> SelectedActors = GetSelectedActors();
	TArray<AActor*> PlacedActors;
	PlacedActors.Add(SelectedActors[0]);
	
	FScopedTransaction Transaction(LOCTEXT("OMEGALIGN LookAt First", "OMEGALIGN LookAt First"));
	
	if (GetSelectedActors().Num() > 0)
	{
		DrawDebugCylinder(GEditor->GetEditorWorldContext().World(), SelectedActors[0]->GetActorLocation() - FVector(0.0f, 0.0f, PluginSettings->RandomDistributionMaxHeightDifference), SelectedActors[0]->GetActorLocation() + FVector(0.0f, 0.0f, PluginSettings->RandomDistributionMaxHeightDifference), PluginSettings->RandomDistributionRadius, 32, FColor(147.0f, 140.0f, 228.0f), false, 2.0f, 0, 2.0f );
		
		for (int i = 0; i < SelectedActors.Num(); i++)
		{
			if (i != 0)
			{
				if (TryDistributeRandomly(SelectedActors[0],SelectedActors[i], SelectedActors, PlacedActors))
				{
					PlacedActors.Add(SelectedActors[i]);
				}
			}
		}
	}
}

bool FOMEGALIGNCommands::Callbacks::CanDistributeRandomly()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::DuplicateToView()
{
	DuplicateToMouseOrView(false);
}

bool FOMEGALIGNCommands::Callbacks::CanDuplicateToView()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::ArrayDuplication()
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	TArray<AActor*> SelectedActors = GetSelectedActors();
	TArray<AActor*> NewSelectedActors;

	FScopedTransaction Transaction(LOCTEXT("OMEGALIGN Array Duplication", "OMEGALIGN Array Duplication"));

	for (int z = 0; z < PluginSettings->ArrayAmountZ; z++)
	{
		for (int y = 0; y < PluginSettings->ArrayAmountY; y++)
		{
			for (int x = 0; x < PluginSettings->ArrayAmountX; x++)
			{
				if (SelectedActors.Num() > 0)
				{
					for (AActor* Actor : SelectedActors)
					{
						FActorSpawnParameters SpawnParams = FActorSpawnParameters();
						SpawnParams.Template = Actor;

						FVector OutCentre, OutBoxExtent;
						UGameplayStatics::GetActorArrayBounds(SelectedActors, false, OutCentre, OutBoxExtent);
	
			
						if (Actor->IsA(AActor::StaticClass()))
						{
							FRotator BaseRot = FRotator(0.0f, 0.0f, 0.0f);							
							FVector RelativeOffsets = FVector(UKismetMathLibrary::RandomFloatInRange(PluginSettings->ArrayRelativeOffsetRangeX.X, PluginSettings->ArrayRelativeOffsetRangeX.Y), UKismetMathLibrary::RandomFloatInRange(PluginSettings->ArrayRelativeOffsetRangeY.X, PluginSettings->ArrayRelativeOffsetRangeY.Y), UKismetMathLibrary::RandomFloatInRange(PluginSettings->ArrayRelativeOffsetRangeZ.X, PluginSettings->ArrayRelativeOffsetRangeZ.Y));
							FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(FVector(UKismetMathLibrary::RandomFloatInRange(PluginSettings->ArrayConstantOffsetRange.X, PluginSettings->ArrayConstantOffsetRange.Y) * x, UKismetMathLibrary::RandomFloatInRange(PluginSettings->ArrayConstantOffsetRange.X, PluginSettings->ArrayConstantOffsetRange.Y) * y, UKismetMathLibrary::RandomFloatInRange(PluginSettings->ArrayConstantOffsetRange.X, PluginSettings->ArrayConstantOffsetRange.Y) * z ) + RelativeOffsets, BaseRot, FVector(1.0f, 1.0f, 1.0f));
							AActor* DuplicateActor = GEditor->GetEditorWorldContext().World()->SpawnActor(Actor->GetClass(), &SpawnTransform, SpawnParams );
							NewSelectedActors.Add(DuplicateActor);

							if (PluginSettings->ArrayApplyRandomRotation)
							{
								DuplicateActor->SetActorRotation(GetRandomRotator());
							}
							else
							{
								FRotator RotX = PluginSettings->ArrayRotationIncrementX * x;
								FRotator RotY = PluginSettings->ArrayRotationIncrementY * y;
								FRotator RotZ = PluginSettings->ArrayRotationIncrementZ * z;

								FRotator ArrayRot = UKismetMathLibrary::ComposeRotators(UKismetMathLibrary::ComposeRotators(RotX, RotY), RotZ);
								FRotator NewRot = UKismetMathLibrary::ComposeRotators(Actor->GetActorRotation(), ArrayRot);

								DuplicateActor->SetActorRotation(NewRot);
							}
							DuplicateActor->Rename();
							DuplicateActor->SetActorLabel(Actor->GetActorLabel());
						}
					}
				}
			}
		}
	}
	if (!PluginSettings->KeepOriginalActorsAfterOperation)
	{
		for (AActor* Actor : SelectedActors)
		{
			Actor->Destroy();
		}
	}

	if (PluginSettings->SelectArrayActorsAfterOperation)
	{
		GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(NewSelectedActors);
	}
}

bool FOMEGALIGNCommands::Callbacks::CanArrayDuplication()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::SelectActorsNearCursor()
{
	ActorNearCursorSelection(false);
}

bool FOMEGALIGNCommands::Callbacks::CanSelectActorsNearCursor()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::MultiSelectActorsNearCursor()
{
	ActorNearCursorSelection(true);
}

bool FOMEGALIGNCommands::Callbacks::CanMultiSelectActorsNearCursor()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::DeselectActorsNearCursor()
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	auto Client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
	
	FVector StartLoc = Client->GetViewLocation();
	FVector TargetDir = Client->GetCursorWorldLocationFromMousePos().GetDirection() * 10000000;
	FVector TargetLoc = StartLoc + TargetDir;

	FHitResult TraceHit;
	FCollisionQueryParams TraceParams;
	
	TArray<AActor*> ActorsToDeselect;
	TArray<AActor*> NewSelectedActors = GetSelectedActors();
	
	GEditor->GetEditorWorldContext().World()->LineTraceSingleByChannel(TraceHit, StartLoc, TargetLoc, ECC_Visibility, TraceParams);
	
	for (TActorIterator<AActor> Actor(GEditor->GetEditorWorldContext().World()); Actor; ++Actor)
	{
		if (UKismetMathLibrary::Vector_Distance(Actor->GetActorLocation(), TraceHit.Location) <= PluginSettings->SelectActorsNearCursorDistance)
		{
			ActorsToDeselect.AddUnique(Actor.operator*());
		}
	}
	DrawDebugSphere(GEditor->GetEditorWorldContext().World(), TraceHit.Location, PluginSettings->SelectActorsNearCursorDistance, 36, FColor(228.0f, 140.0f, 147.0f), false, 1.0f, 0, 2.0f);

	for (AActor* Actor : ActorsToDeselect)
	{
		NewSelectedActors.Remove(Actor);
		GEditor->GetEngineSubsystem<UOMEGALIGNSubsystem>()->PersistentSelectedActors.Remove(Actor);
	}

	GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(NewSelectedActors);
}

bool FOMEGALIGNCommands::Callbacks::CanDeselectActorsNearCursor()
{
	return true;
}

void FOMEGALIGNCommands::Callbacks::SelectSimilarInRange()
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	
	TArray<AActor*> SelectedActors = GetSelectedActors();
	TArray<AActor*> NewSelectedActors;
	
	for (AActor* SelectedActor : SelectedActors)
	{
		DrawDebugSphere(GEditor->GetEditorWorldContext().World(), SelectedActor->GetActorLocation(), PluginSettings->SelectSimilarInRangeDistance, 36, FColor(147.0f, 140.0f, 228.0f), false, 1.0f, 0, 2.0f);
		for (TActorIterator<AActor> Actor(GEditor->GetEditorWorldContext().World()); Actor; ++Actor)
		{
			if (Actor->IsA(SelectedActor->GetClass()))
			{
				if (Actor->IsA(AStaticMeshActor::StaticClass()))
				{
					AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(Actor.operator*());
					if (StaticMeshActor != nullptr)
					{
						if (Cast<AStaticMeshActor>(SelectedActor)->GetStaticMeshComponent()->GetStaticMesh() == StaticMeshActor->GetStaticMeshComponent()->GetStaticMesh())
						{
							if (UKismetMathLibrary::Vector_Distance(Actor->GetActorLocation(), SelectedActor->GetActorLocation()) <= PluginSettings->SelectSimilarInRangeDistance)
							{
								NewSelectedActors.AddUnique(Actor.operator*());
							}
						}
					}
				}
				else
				{
					if (UKismetMathLibrary::Vector_Distance(Actor->GetActorLocation(), SelectedActor->GetActorLocation()) <= PluginSettings->SelectSimilarInRangeDistance)
					{
						NewSelectedActors.Add(Actor.operator*());
					}
				}

			}
		}
	}
	
	GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(NewSelectedActors);
}

bool FOMEGALIGNCommands::Callbacks::CanSelectSimilarInRange()
{
	return true;
}

void FOMEGALIGNCommands::AlignTransformFunction(bool AlignRotation, bool AlignLocation, bool AlignScale)
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	TArray<AActor*> SelectedActors = GetSelectedActors();
	TArray<AActor*> NewSelectedActors;
	TArray<AActor*> Empty;

	FRotator RotBase = FRotator();
	FVector LocBase = FVector();
	FVector ScaleBase = FVector();

	auto Client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
	FVector TraceStartLoc = Client->GetViewLocation();
	FVector TraceTargetDir = Client->GetCursorWorldLocationFromMousePos().GetDirection() * 10000000;
	FVector TraceTargetLoc = TraceStartLoc + TraceTargetDir;
	FHitResult TraceHit;
	TArray<FHitResult> TraceHits;
	FCollisionQueryParams TraceParams;
	FCollisionObjectQueryParams TraceObjects(ECC_TO_BITFIELD(ECC_WorldStatic));
	TraceParams.ClearIgnoredActors();
	TraceParams.bTraceComplex = true;

	bool IsValidMouseTrace = false;
	
		
	if (GetSelectedActors().Num() > 0)
	{
		switch (PluginSettings->AlignTarget)
		{
		case UnderCursor:
			GEditor->GetEditorWorldContext().World()->LineTraceSingleByChannel(TraceHit, TraceStartLoc, TraceTargetLoc, ECC_Camera, TraceParams);
			// DrawDebugLine(GEditor->GetEditorWorldContext().World(), TraceStartLoc, TraceTargetLoc, FColor::Red, false, 2.0f, 0, 2.0f); 
			
			if (TraceHit.GetActor())
			{
				ALandscapeProxy* LandscapeProxy = Cast<ALandscapeProxy>(TraceHit.GetActor());
				if (!LandscapeProxy)
				{
					RotBase = TraceHit.GetActor()->GetActorRotation();
					LocBase = TraceHit.GetActor()->GetActorLocation();
					ScaleBase = TraceHit.GetActor()->GetActorScale();
					IsValidMouseTrace = true;
				}
			}
			break;
			
		case FirstSelected:
			RotBase = SelectedActors[0]->GetActorRotation();
			LocBase = SelectedActors[0]->GetActorLocation();
			ScaleBase = SelectedActors[0]->GetActorScale();
			break;
			
		case LastSelected:
			RotBase = SelectedActors[SelectedActors.Num()-1]->GetActorRotation();
			LocBase = SelectedActors[SelectedActors.Num()-1]->GetActorLocation();
			ScaleBase = SelectedActors[SelectedActors.Num()-1]->GetActorScale();
			break;
		}
		if (AlignRotation)
		{
			FScopedTransaction Transaction(LOCTEXT("OMEGALIGN Rotation", "OMEGALIGN Rotation"));

			if (PluginSettings->AlignTarget == UnderCursor)
			{
				if (IsValidMouseTrace)
				{
					for (AActor* Actor : SelectedActors)
					{
						Actor->Modify();

						FRotator NewRotation = FRotator(PluginSettings->AlignRotationPitch ? RotBase.Pitch : Actor->GetActorRotation().Pitch, PluginSettings->AlignRotationYaw ? RotBase.Yaw : Actor->GetActorRotation().Yaw, PluginSettings->AlignRotationRoll ? RotBase.Roll : Actor->GetActorRotation().Roll);
						Actor->SetActorRotation(NewRotation);
					}
				}
			}
			else
			{
				for (AActor* Actor : SelectedActors)
				{
					Actor->Modify();

					FRotator NewRotation = FRotator(PluginSettings->AlignRotationPitch ? RotBase.Pitch : Actor->GetActorRotation().Pitch, PluginSettings->AlignRotationYaw ? RotBase.Yaw : Actor->GetActorRotation().Yaw, PluginSettings->AlignRotationRoll ? RotBase.Roll : Actor->GetActorRotation().Roll);
					Actor->SetActorRotation(NewRotation);
				}
			}

		}

		if (AlignLocation)
		{
			FScopedTransaction Transaction(LOCTEXT("OMEGALIGN Location", "OMEGALIGN Location"));

			if (PluginSettings->AlignTarget == UnderCursor)
			{
				if (IsValidMouseTrace)
				{
					for (AActor* Actor : SelectedActors)
					{
						Actor->Modify();

						FVector NewLocation = FVector(PluginSettings->AlignLocationX ? LocBase.X : Actor->GetActorLocation().X, PluginSettings->AlignLocationY ? LocBase.Y : Actor->GetActorLocation().Y, PluginSettings->AlignLocationZ ? LocBase.Z : Actor->GetActorLocation().Z);
						Actor->SetActorLocation(NewLocation);
					}
				}
			}
			else
			{
				for (AActor* Actor : SelectedActors)
				{
					Actor->Modify();

					FVector NewLocation = FVector(PluginSettings->AlignLocationX ? LocBase.X : Actor->GetActorLocation().X, PluginSettings->AlignLocationY ? LocBase.Y : Actor->GetActorLocation().Y, PluginSettings->AlignLocationZ ? LocBase.Z : Actor->GetActorLocation().Z);
					Actor->SetActorLocation(NewLocation);
				}
			}


		}

		if (AlignScale)
		{
			FScopedTransaction Transaction(LOCTEXT("OMEGALIGN Scale", "OMEGALIGN Scale"));

			if (PluginSettings->AlignTarget == UnderCursor)
			{
				if (IsValidMouseTrace)
				{
					for (AActor* Actor : SelectedActors)
					{
						Actor->Modify();

						FVector NewScale = FVector(PluginSettings->AlignScaleX ? ScaleBase.X : Actor->GetActorScale().X, PluginSettings->AlignScaleY ? ScaleBase.Y : Actor->GetActorScale().Y, PluginSettings->AlignScaleZ ? ScaleBase.Z : Actor->GetActorScale().Z);
						Actor->SetActorScale3D(NewScale);
					}
				}
			}
			else
			{
				for (AActor* Actor : SelectedActors)
				{
					Actor->Modify();

					FVector NewScale = FVector(PluginSettings->AlignScaleX ? ScaleBase.X : Actor->GetActorScale().X, PluginSettings->AlignScaleY ? ScaleBase.Y : Actor->GetActorScale().Y, PluginSettings->AlignScaleZ ? ScaleBase.Z : Actor->GetActorScale().Z);
					Actor->SetActorScale3D(NewScale);
				}
			}
		}		
		
		switch (PluginSettings->AfterAlignOperation)
		{
			case SelectChanged:
				for (int i = 0; i < SelectedActors.Num(); i++)
				{
					if (i != 0)
					{
						NewSelectedActors.Add(SelectedActors[i]);
					}
				}
				GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(NewSelectedActors);
				break;
			
			case SelectFirst:
				NewSelectedActors.Add(SelectedActors[0]);
				GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(NewSelectedActors);
				break;
			
			case SelectAll:
				break;

			case DeselectAll:
				GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(Empty);
				break;
			
			default:
				break;
		}
	}
}

FRotator FOMEGALIGNCommands::GetRandomRotator()
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	
	double RandomRoll = UKismetMathLibrary::RandomFloatInRange(PluginSettings->RandomRollRange.X, PluginSettings->RandomRollRange.Y);
	double RandomPitch = UKismetMathLibrary::RandomFloatInRange(PluginSettings->RandomPitchRange.X, PluginSettings->RandomPitchRange.Y);
	double RandomYaw = UKismetMathLibrary::RandomFloatInRange(PluginSettings->RandomYawRange.X, PluginSettings->RandomYawRange.Y);

	FRotator RandomRotation = FRotator(RandomPitch, RandomYaw, RandomRoll);

	return RandomRotation;
}

TArray<AActor*> FOMEGALIGNCommands::GetSelectedActors()
{
	USelection* Selection = GEditor->GetSelectedActors();
	TArray<AActor*> SelectedActors;
	TArray<ULevel*> UniqueLevels;

	// Get Selected Actors from Selection
	for (FSelectionIterator Iter(*Selection); Iter; ++Iter)
	{
		AActor* Actor = Cast<AActor>(*Iter);
		if (Actor)
		{
			SelectedActors.Add(Actor);
			UniqueLevels.AddUnique(Actor->GetLevel());
		}
	}
	return SelectedActors;
}

FVector FOMEGALIGNCommands::GetRandomPointInCircle(double Radius, FVector Center)
{
	double r = Radius * UKismetMathLibrary::Sqrt(UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f));
	double theta = 2 * UKismetMathLibrary::GetPI() * UKismetMathLibrary::RandomFloatInRange(0.0f, 1.0f);

	FVector Point = FVector(Center.X + r * UKismetMathLibrary::Cos(theta), Center.Y + r * UKismetMathLibrary::Sin(theta), Center.Z);
	return Point;
}

bool FOMEGALIGNCommands::TryDistributeRandomly(AActor* CenterActor, AActor* ActorToPlace, TArray<AActor*> SelectedActors, TArray<AActor*> PlacedActors)
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	int PlacementAttempts = 0;
	FHitResult TraceHit;
	FVector StartLocation;
	FVector TargetLocation;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActors(SelectedActors);
	bool IsPlaced = false;
	TArray<AActor*> ActorsToIgnore;
	
	while (PlacementAttempts <= PluginSettings->MaxPlacementAttempts)
	{
		TargetLocation = GetRandomPointInCircle(PluginSettings->RandomDistributionRadius, CenterActor->GetActorLocation() - FVector (0.0f, 0.0f, PluginSettings->RandomDistributionMaxHeightDifference));
		StartLocation = TargetLocation + 2 * FVector (0.0f, 0.0f, PluginSettings->RandomDistributionMaxHeightDifference);
		GEditor->GetEditorWorldContext().World()->LineTraceSingleByChannel(TraceHit, StartLocation, TargetLocation, ECC_Visibility, TraceParams);
		// DrawDebugLine(GEditor->GetEditorWorldContext().World(), StartLocation, TargetLocation, TraceHit.IsValidBlockingHit() ? FColor::Cyan : FColor::Red, false, 1.0f, 0.0f, 2.0f);
		
		if (TraceHit.IsValidBlockingHit())
		{
			for (AActor* Actor : PlacedActors)
			{
				UE_LOG(LogTemp, Log, TEXT("Distance: %f"), UKismetMathLibrary::Vector_Distance(Actor->GetActorLocation(), TraceHit.Location));
				ActorToPlace->Modify();
				ActorToPlace->SetActorLocation(TraceHit.Location + FVector(0.0f, 0.0f, UKismetMathLibrary::RandomFloatInRange(PluginSettings->RandomDistributionOffsetRangeZ.X, PluginSettings->RandomDistributionOffsetRangeZ.Y)));
				IsPlaced = true;
			}
		}
		
		PlacementAttempts++;

		if (IsPlaced)
		{
			break;
		}
		
		if (PlacementAttempts == PluginSettings->MaxPlacementAttempts)
		{
			UE_LOG(LogTemp, Log, TEXT("Tried to place %s %d times without finding a valid position, consider changing the RandomDistributionHeightDifference in the Plugin Settings"), *GetNameSafe(ActorToPlace), PluginSettings->MaxPlacementAttempts);
		}
	}
	return IsPlaced;
}

UOMEGALIGNSettings* FOMEGALIGNCommands::GetPluginSettings()
{
	UOMEGALIGNSettings* PluginSettings = GetMutableDefault<UOMEGALIGNSettings>();
	return PluginSettings;
}

void FOMEGALIGNCommands::DuplicateToMouseOrView(bool ForceCameraMode)
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	auto Client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();

	FVector CameraLoc = Client->GetViewLocation();
	FRotator CameraRot = Client->GetViewRotation();
		
	FVector StartLoc;
	FVector TargetDir;
	
	switch (PluginSettings->DuplicateToViewMode)
	{
		case CameraTrace:
			StartLoc = CameraLoc;
			TargetDir = UKismetMathLibrary::GetForwardVector(CameraRot) * 100000000;
			break;
		case MouseTrace:
			if (ForceCameraMode)
			{
				StartLoc = CameraLoc;
				TargetDir = UKismetMathLibrary::GetForwardVector(CameraRot) * 100000000;
				break;
			}
			else
			{
				StartLoc =  CameraLoc;
				TargetDir = Client->GetCursorWorldLocationFromMousePos().GetDirection() * 10000000;
				break;
			}
		default:
			StartLoc = CameraLoc;
			TargetDir = UKismetMathLibrary::GetForwardVector(CameraRot) * 100000000;
	}

	FVector TargetLoc = StartLoc + TargetDir;
	FHitResult TraceHit;
	FCollisionQueryParams TraceParams;
	TArray<AActor*> SelectedActors = GetSelectedActors();
	TArray<AActor*> OriginallySelectedActors = SelectedActors;
	TArray<AActor*> NewSelectedActors;
	
	GEditor->GetEditorWorldContext().World()->LineTraceSingleByChannel(TraceHit, StartLoc, TargetLoc, PluginSettings->SurfaceTraceCollisionChannel, TraceParams);
	
	// DrawDebugLine(GEditor->GetEditorWorldContext().World(), StartLoc, TargetLoc, TraceHit.bBlockingHit ? FColor::Cyan : FColor::Red, false, 10.0f, 0.0f, 2.0f);
	// UE_LOG(LogTemp, Warning, TEXT("Duplicate To View!"));

	if (SelectedActors.Num() > 0)
	{
		FScopedTransaction Transaction(LOCTEXT("OMEGALIGN Duplicate to View", "OMEGALIGN Duplicate to View"));
		
		for (AActor* Actor : SelectedActors)
		{
			FActorSpawnParameters SpawnParams = FActorSpawnParameters();
			SpawnParams.Template = Actor;

			FVector OutCentre, OutBoxExtent;
			UGameplayStatics::GetActorArrayBounds(SelectedActors, true, OutCentre, OutBoxExtent);
			
			if (Actor->IsA(AActor::StaticClass()))
			{
				FTransform SpawnTransform = UKismetMathLibrary::MakeTransform(TraceHit.Location - OutCentre, FRotator(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));
				AActor* DuplicateActor = GEditor->GetEditorWorldContext().World()->SpawnActor(Actor->GetClass(), &SpawnTransform, SpawnParams );
				
				NewSelectedActors.Add(DuplicateActor);
				FHitResult SecondTraceHit;
				FCollisionQueryParams SecondTraceParams;
				SecondTraceParams.AddIgnoredActors(NewSelectedActors);
				
				if (DuplicateActor)
				{
					if (PluginSettings->AlignDuplicatedActorsToSurface)
					{
						GEditor->GetEditorWorldContext().World()->LineTraceSingleByChannel(SecondTraceHit, DuplicateActor->GetActorLocation() + FVector(0,0, PluginSettings->DuplicatedActorSurfaceTraceLength/2), DuplicateActor->GetActorLocation() - FVector(0,0, PluginSettings->DuplicatedActorSurfaceTraceLength/2), PluginSettings->SurfaceTraceCollisionChannel, SecondTraceParams);
						if (PluginSettings->ShowSurfaceTraces)
						{
							DrawDebugLine(GEditor->GetEditorWorldContext().World(), DuplicateActor->GetActorLocation() + FVector(0,0, PluginSettings->DuplicatedActorSurfaceTraceLength/2), DuplicateActor->GetActorLocation() - FVector(0,0, PluginSettings->DuplicatedActorSurfaceTraceLength/2), TraceHit.IsValidBlockingHit() ? FColor::Cyan : FColor::Red, false, 5.0f, 0.0f, 2.0f);
						}
						
						if (SecondTraceHit.IsValidBlockingHit())
						{
							DuplicateActor->SetActorLocation(SecondTraceHit.Location + PluginSettings->DuplicatedActorSurfaceOffset);
						}
					}
					if (PluginSettings -> ApplyRandomRotation)
					{
						DuplicateActor->SetActorRotation(GetRandomRotator());
					}
					else
					{
						DuplicateActor->SetActorRotation(Actor->GetActorRotation());
					}
	
					DuplicateActor->Rename();
					DuplicateActor->SetActorLabel(Actor->GetActorLabel());

				}
			}
			GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(NewSelectedActors);
		}
		if (PluginSettings->AutomaticallyDeleteOriginallySelectedActors)
		{
			for (AActor* Actor : OriginallySelectedActors)
			{
				Actor->Destroy();
			}
		}
	}
}

void FOMEGALIGNCommands::ActorNearCursorSelection(bool IsMultiSelect)
{
	UOMEGALIGNSettings* PluginSettings = GetPluginSettings();
	auto Client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
	
	FVector StartLoc = Client->GetViewLocation();
	FVector TargetDir = Client->GetCursorWorldLocationFromMousePos().GetDirection() * 10000000;
	FVector TargetLoc = StartLoc + TargetDir;

	FHitResult TraceHit;
	FCollisionQueryParams TraceParams;
	
	TArray<AActor*> NewSelectedActors;
	
	GEditor->GetEditorWorldContext().World()->LineTraceSingleByChannel(TraceHit, StartLoc, TargetLoc, ECC_Visibility, TraceParams);

	if(!IsMultiSelect)
	{
		const TArray<AActor*> EmptyArray;
		GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(EmptyArray);
		GEditor->GetEngineSubsystem<UOMEGALIGNSubsystem>()->PersistentSelectedActors.Empty();
	}

	for (TActorIterator<AActor> Actor(GEditor->GetEditorWorldContext().World()); Actor; ++Actor)
	{
		if (UKismetMathLibrary::Vector_Distance(Actor->GetActorLocation(), TraceHit.Location) <= PluginSettings->SelectActorsNearCursorDistance)
		{
			NewSelectedActors.AddUnique(Actor.operator*());
		}
	}
	DrawDebugSphere(GEditor->GetEditorWorldContext().World(), TraceHit.Location, PluginSettings->SelectActorsNearCursorDistance, 36, FColor(147.0f, 140.0f, 228.0f), false, 1.0f, 0, 2.0f);

	if(IsMultiSelect)
	{
		TArray<AActor*> AllSelectedActors = GetSelectedActors();
		
		for (AActor* Actor : GEditor->GetEngineSubsystem<UOMEGALIGNSubsystem>()->PersistentSelectedActors)
		{
			AllSelectedActors.AddUnique(Actor);
		}

		for (AActor* Actor : NewSelectedActors)
		{
			AllSelectedActors.AddUnique(Actor);
		}
		
		GEditor->GetEngineSubsystem<UOMEGALIGNSubsystem>()->PersistentSelectedActors = AllSelectedActors;
		GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(GEditor->GetEngineSubsystem<UOMEGALIGNSubsystem>()->PersistentSelectedActors);
	}
	else
	{
		GEditor->GetEditorSubsystem<UEditorActorSubsystem>()->SetSelectedLevelActors(NewSelectedActors);
	}
}


void FOMEGALIGNCommands::Callbacks::PluginAction()
{
	FModuleManager::GetModulePtr<ISettingsModule>("Settings")->ShowViewer("Project", "Plugins", "OMEGALIGN");
}

bool FOMEGALIGNCommands::Callbacks::CanPluginAction()
{
	return true;
}


#undef LOCTEXT_NAMESPACE

