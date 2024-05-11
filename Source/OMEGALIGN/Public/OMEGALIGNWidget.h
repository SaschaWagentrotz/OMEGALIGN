// Copyright 2023 Sascha Wagentrotz All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OMEGALIGNSettings.h"
#include "EditorUtilityWidget.h"
#include "OMEGALIGNWidget.generated.h"

UCLASS(Abstract)
class OMEGALIGN_API UOMEGALIGNWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OpenOMEGALIGNSettings();
	
	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OpenOMEGALIGNHotkeys();

	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_AlignRotation();
	
	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_AlignLocation();

	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_AlignScale();
	
	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_AlignTransform();

	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_ApplyRandomRotation();

	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_IncrementTransform();

	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_LookAtFirst();

	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_LookAtLast();

	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_DuplicateToView();
	
	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_DistributeRandomly();

	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_ArrayDuplication();

	UFUNCTION(BlueprintCallable, Category="OMEGALIGN")
	void OMEGALIGN_SelectSimilarInRage();

	UFUNCTION(BlueprintPure, Category="OMEGALIGN")
	bool IsAlignTargetMouseCursor();











public:
	
};
