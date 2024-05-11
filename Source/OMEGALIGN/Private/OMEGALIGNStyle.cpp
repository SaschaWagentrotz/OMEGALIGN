// Copyright 2023 Sascha Wagentrotz All Rights Reserved.

#include "OMEGALIGNStyle.h"
#include "OMEGALIGN.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FOMEGALIGNStyle::StyleInstance = nullptr;

void FOMEGALIGNStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FOMEGALIGNStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FOMEGALIGNStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("OMEGALIGNStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FOMEGALIGNStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("OMEGALIGNStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("OMEGALIGN")->GetBaseDir() / TEXT("Resources"));

	Style->Set("OMEGALIGN.PluginAction", new IMAGE_BRUSH_SVG(TEXT("OMEGALIGN"), Icon20x20));
	return Style;
}

void FOMEGALIGNStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FOMEGALIGNStyle::Get()
{
	return *StyleInstance;
}
