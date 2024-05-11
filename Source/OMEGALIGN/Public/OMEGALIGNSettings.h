// Copyright 2023 Sascha Wagentrotz All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "OMEGALIGNSettings.generated.h"

UENUM()
enum AlignTarget
{
	UnderCursor		UMETA(DisplayName = "Actor under the Mouse Cursor"),
	FirstSelected	UMETA(DisplayName = "First Selected Actor"),
	LastSelected	UMETA(DisplayName = "Last Selected Áctor"),
};

UENUM()
enum  AfterAlignOperation
{
	SelectChanged	UMETA(DisplayName = "Only Select Changed Actors"),
	SelectFirst		UMETA(DisplayName = "Only Select First Actor"),
	SelectAll		UMETA(DisplayName = "Select All Actors"),
	DeselectAll		UMETA(DisplayName = "Deselect All Actors"),
};

UENUM()
enum  AlignTransformSettings
{
	LocationRotationScale	UMETA(DisplayName = "Location + Rotation + Scale"),
	LocationRotation		UMETA(DisplayName = "Only Location + Rotation"),
	LocationScale			UMETA(DisplayName = "Only Location + Scale"),
	RotationScale			UMETA(DisplayName = "Only Rotation + Scale"),
};

UENUM()
enum  IncrementBasedOn
{
	FirstSelectedActor	UMETA(DisplayName = "First Selected Actor"),
	LastSelectedActor	UMETA(DisplayName = "Last Selected Actor"),
};

UENUM()
enum  AfterLookAtOperation
{
	KeepAllSelected	UMETA(DisplayName = "Keep all Actors selected"),
	DeselectTarget		UMETA(DisplayName = "Deselect LookAtTargetActor (first or last selected actor)"),
};

UENUM()
enum DuplicateToViewMode
{
	CameraTrace UMETA(DisplayName = "Camera Trace"),
	MouseTrace UMETA(DisplayName = "Mouse Trace"),
};

UCLASS(config = MySetting)

class OMEGALIGN_API UOMEGALIGNSettings : public UObject
{
	GENERATED_BODY()

public:
	UOMEGALIGNSettings(const FObjectInitializer& obj);

	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	TEnumAsByte<AlignTarget> AlignTarget;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	TEnumAsByte<AfterAlignOperation> AfterAlignOperation;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	TEnumAsByte<AlignTransformSettings> AlignTransformTo;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	bool AlignRotationRoll;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	bool AlignRotationPitch;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	bool AlignRotationYaw;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	bool AlignLocationX;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	bool AlignLocationY;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	bool AlignLocationZ;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	bool AlignScaleX;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	bool AlignScaleY;
	UPROPERTY(Config, EditAnywhere, Category = "Alignment Rules")
	bool AlignScaleZ;
	
	UPROPERTY(Config, EditAnywhere, Category = "Rotation Randomization Rules")
	bool RandomRotationPerActor;
	UPROPERTY(Config, EditAnywhere, Category = "Rotation Randomization Rules")
	FVector2D RandomRollRange;
	UPROPERTY(Config, EditAnywhere, Category = "Rotation Randomization Rules")
	FVector2D RandomPitchRange;
	UPROPERTY(Config, EditAnywhere, Category = "Rotation Randomization Rules")
	FVector2D RandomYawRange;

	UPROPERTY(Config, EditAnywhere, Category = "Increment Transform Rules")
	TEnumAsByte<IncrementBasedOn> IncrementBasedOn;
	UPROPERTY(Config, EditAnywhere, Category = "Increment Transform Rules")
	FRotator RotationIncrement;
	UPROPERTY(Config, EditAnywhere, Category = "Increment Transform Rules")
	FVector3d LocationIncrement;
	UPROPERTY(Config, EditAnywhere, Category = "Increment Transform Rules")
	FVector3d ScaleIncrement;
	
	UPROPERTY(Config, EditAnywhere, Category = "Look At Rules")
	TEnumAsByte<AfterLookAtOperation> AfterLookAtOperation;
	UPROPERTY(Config, EditAnywhere, Category = "Look At Rules")
	double FixedLookAtRotationOffset;
	UPROPERTY(Config, EditAnywhere, Category = "Look At Rules")
	FVector2D AdditionalRandomRotationOffsetRange;

	UPROPERTY(Config, EditAnywhere, Category = "Random Distribution Rules")
	double RandomDistributionRadius;
	UPROPERTY(Config, EditAnywhere, Category = "Random Distribution Rules")
	double RandomDistributionMaxHeightDifference;
	UPROPERTY(Config, EditAnywhere, Category = "Random Distribution Rules")
	int MaxPlacementAttempts;
	UPROPERTY(Config, EditAnywhere, Category = "Random Distribution Rules")
	FVector2D RandomDistributionOffsetRangeZ;

	UPROPERTY(Config, EditAnywhere, Category = "Dupliacte To View Rules")
	TEnumAsByte<DuplicateToViewMode> DuplicateToViewMode;
	UPROPERTY(Config, EditAnywhere, Category = "Dupliacte To View Rules")
	bool AlignDuplicatedActorsToSurface;
	UPROPERTY(Config, EditAnywhere, Category = "Dupliacte To View Rules")
	bool AutomaticallyDeleteOriginallySelectedActors;
	UPROPERTY(Config, EditAnywhere, Category = "Dupliacte To View Rules")
	FVector3d DuplicatedActorSurfaceOffset;
	UPROPERTY(Config, EditAnywhere, Category = "Dupliacte To View Rules")
	bool ApplyRandomRotation;
	UPROPERTY(Config, EditAnywhere, Category = "Dupliacte To View Rules")
	bool ShowSurfaceTraces;
	UPROPERTY(Config, EditAnywhere, Category = "Dupliacte To View Rules")
	double DuplicatedActorSurfaceTraceLength;
	UPROPERTY(Config, EditAnywhere, Category = "Dupliacte To View Rules")
	TEnumAsByte<ECollisionChannel> SurfaceTraceCollisionChannel;
	
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	bool SelectArrayActorsAfterOperation;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	bool KeepOriginalActorsAfterOperation;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	int ArrayAmountX;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	int ArrayAmountY;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	int ArrayAmountZ;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	FVector2D ArrayRelativeOffsetRangeX;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	FVector2D ArrayRelativeOffsetRangeY;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	FVector2D ArrayRelativeOffsetRangeZ;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	FVector2D ArrayConstantOffsetRange;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	FRotator ArrayRotationIncrementX;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	FRotator ArrayRotationIncrementY;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	FRotator ArrayRotationIncrementZ;
	UPROPERTY(Config, EditAnywhere, Category = "Array Duplication Rules")
	bool ArrayApplyRandomRotation;
		
	UPROPERTY(Config, EditAnywhere, Category = "Select Actors Near Cursor Rules")
	double SelectActorsNearCursorDistance;

	UPROPERTY(Config, EditAnywhere, Category = "Select Similar in Range Rules")
	double SelectSimilarInRangeDistance;
	
};