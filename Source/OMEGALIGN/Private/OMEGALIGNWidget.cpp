// Copyright 2023 Sascha Wagentrotz All Rights Reserved.

#include "OMEGALIGNWidget.h"
#include "OMEGALIGNCommands.h"
#include "ISettingsModule.h"


void UOMEGALIGNWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UOMEGALIGNWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UOMEGALIGNWidget::OpenOMEGALIGNSettings()
{
	FModuleManager::GetModulePtr<ISettingsModule>("Settings")->ShowViewer("Project", "Plugins", "OMEGALIGN");
}

void UOMEGALIGNWidget::OpenOMEGALIGNHotkeys()
{
	FModuleManager::GetModulePtr<ISettingsModule>("Settings")->ShowViewer("Editor", "General", "InputBindings");
}

void UOMEGALIGNWidget::OMEGALIGN_AlignRotation()
{
	FOMEGALIGNCommands::Callbacks::AlignRotation();
}

void UOMEGALIGNWidget::OMEGALIGN_AlignLocation()
{
	FOMEGALIGNCommands::Callbacks::AlignLocation();
}

void UOMEGALIGNWidget::OMEGALIGN_AlignScale()
{
	FOMEGALIGNCommands::Callbacks::AlignScale();
}

void UOMEGALIGNWidget::OMEGALIGN_AlignTransform()
{
	FOMEGALIGNCommands::Callbacks::AlignTransform();
}

void UOMEGALIGNWidget::OMEGALIGN_ApplyRandomRotation()
{
	FOMEGALIGNCommands::Callbacks::ApplyRandomRotation();
}

void UOMEGALIGNWidget::OMEGALIGN_IncrementTransform()
{
	FOMEGALIGNCommands::Callbacks::IncrementTransform();
}

void UOMEGALIGNWidget::OMEGALIGN_LookAtFirst()
{
	FOMEGALIGNCommands::Callbacks::LookAtFirst();
}

void UOMEGALIGNWidget::OMEGALIGN_LookAtLast()
{
	FOMEGALIGNCommands::Callbacks::LookAtLast();
}

void UOMEGALIGNWidget::OMEGALIGN_DuplicateToView()
{
	FOMEGALIGNCommands::DuplicateToMouseOrView(true);
}

void UOMEGALIGNWidget::OMEGALIGN_DistributeRandomly()
{
	FOMEGALIGNCommands::Callbacks::DistributeRandomly();
}

void UOMEGALIGNWidget::OMEGALIGN_ArrayDuplication()
{
	FOMEGALIGNCommands::Callbacks::ArrayDuplication();
}

void UOMEGALIGNWidget::OMEGALIGN_SelectSimilarInRage()
{
	FOMEGALIGNCommands::Callbacks::SelectSimilarInRange();
}

bool UOMEGALIGNWidget::IsAlignTargetMouseCursor()
{
	UOMEGALIGNSettings* PluginSettings = GetMutableDefault<UOMEGALIGNSettings>();
	return PluginSettings->AlignTarget == UnderCursor;
}

