// Copyright 2023 Sascha Wagentrotz All Rights Reserved.

#include "OMEGALIGNSettings.h"

#include "Engine/StaticMeshActor.h"

UOMEGALIGNSettings::UOMEGALIGNSettings(const FObjectInitializer& obj)
{
	// Alignment Rules
	AlignTarget = LastSelected;
	AfterAlignOperation = SelectChanged;
	AlignTransformTo = LocationRotationScale;
	AlignRotationRoll = true;
	AlignRotationPitch = true;
	AlignRotationYaw = true;
	AlignLocationX = true;
	AlignLocationY = true;
	AlignLocationZ = true;
	AlignScaleX = true;
	AlignScaleY = true;
	AlignScaleZ = true;

	// Rotation Randomization Rules
	RandomRotationPerActor = true;
	RandomRollRange = FVector2D(0.0f, 0.0f);
	RandomPitchRange = FVector2D(0.0f, 0.0f);
	RandomYawRange = FVector2D(-180.0f, 180.0f);

	// Increment Transform Rules
	IncrementBasedOn = FirstSelectedActor;
	RotationIncrement = FRotator(0.0f, 0.0f, 0.0f);
	LocationIncrement = FVector3d(100.0f, 0.0f, 100.0f);
	ScaleIncrement = FVector3d(0.0f, 0.0f, 0.0f);

	// Look At Rules
	AfterLookAtOperation = KeepAllSelected;
	FixedLookAtRotationOffset = 0.0f;
	AdditionalRandomRotationOffsetRange = FVector2D(0.0f, 0.0f);

	// Random Distribution Rules
	RandomDistributionRadius = 600.0f;
	RandomDistributionMaxHeightDifference = 1000.0f;
	MaxPlacementAttempts = 50;
	RandomDistributionOffsetRangeZ = FVector2D(0.0f, 0.0f);

	// Distribute to View Rules
	DuplicateToViewMode = MouseTrace;
	AlignDuplicatedActorsToSurface = true;
	AutomaticallyDeleteOriginallySelectedActors = false;
	DuplicatedActorSurfaceOffset = FVector3d(0.0f, 0.0f, 0.0f);
	ApplyRandomRotation = false;
	ShowSurfaceTraces = false;
	DuplicatedActorSurfaceTraceLength = 500.0f;
	SurfaceTraceCollisionChannel = ECC_Visibility;

	// Array Duplication Rules
	SelectArrayActorsAfterOperation = true;
	KeepOriginalActorsAfterOperation = false;
	ArrayAmountX = 4;
	ArrayAmountY = 4;
	ArrayAmountZ = 4;
	ArrayConstantOffsetRange = FVector2D(125.0f, 125.0f);
	ArrayRelativeOffsetRangeX = FVector2D(0.0f, 0.0f);
	ArrayRelativeOffsetRangeY = FVector2D(0.0f, 0.0f);
	ArrayRelativeOffsetRangeZ = FVector2D(0.0f, 0.0f);
	ArrayRotationIncrementX = FRotator(0.0f, 0.0f, 0.0f);
	ArrayRotationIncrementY = FRotator(0.0f, 0.0f, 0.0f);
	ArrayRotationIncrementZ = FRotator(0.0f, 0.0f, 0.0f);
	ArrayApplyRandomRotation = false;

	// Select Actors Near Cursor Rules
	SelectActorsNearCursorDistance = 500.0f;
	SelectSimilarInRangeDistance = 250.0f;
}
