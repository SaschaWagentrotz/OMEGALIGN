// Copyright 2023 Sascha Wagentrotz All Rights Reserved.

#include "OMEGALIGN.h"

#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "OMEGALIGNStyle.h"
#include "OMEGALIGNCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "ISettingsModule.h"
#include "OMEGALIGNSettings.h"

static const FName OMEGALIGNTabName("OMEGALIGN");

#define LOCTEXT_NAMESPACE "FOMEGALIGNModule"

void FOMEGALIGNModule::StartupModule()
{
	FOMEGALIGNStyle::Initialize();
	FOMEGALIGNStyle::ReloadTextures();

	FOMEGALIGNCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FOMEGALIGNCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FOMEGALIGNModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FOMEGALIGNModule::RegisterMenus));

	if(ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "OMEGALIGN",
			LOCTEXT("RuntimeSettingsName", "OMEGALIGN"), LOCTEXT("RuntimeSettingsDescription", "Configure OMEGALIGN Settings"), 
			GetMutableDefault<UOMEGALIGNSettings>());
	}
}

void FOMEGALIGNModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FOMEGALIGNStyle::Shutdown();

	FOMEGALIGNCommands::Unregister();
}

void FOMEGALIGNModule::PluginButtonClicked()
{
	const FSoftObjectPath widgetAssetPath("/OMEGALIGN/OMEGALIGN/OMEGALIGN.OMEGALIGN");

	UObject* widgetAssetLoaded = widgetAssetPath.TryLoad();
	if (widgetAssetLoaded == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Missing Expected widget class at : /OMEGALIGN/OMEGALIGN/OMEGALIGN.OMEGALIGN"));
		return;
	}

	UEditorUtilityWidgetBlueprint* widget = Cast<UEditorUtilityWidgetBlueprint>(widgetAssetLoaded);
	if (widget == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Couldnt cast /OMEGALIGN/OMEGALIGN/OMEGALIGN.OMEGALIGN to UEditorUtilityWidgetBlueprint"));
		return;
	}

	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	EditorUtilitySubsystem->SpawnAndRegisterTab(widget);
}

void FOMEGALIGNModule::StartWidget()
{
}

void FOMEGALIGNModule::RegisterMenus()
{
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FOMEGALIGNCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("OMEGALIGN");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FOMEGALIGNCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FOMEGALIGNModule, OMEGALIGN)