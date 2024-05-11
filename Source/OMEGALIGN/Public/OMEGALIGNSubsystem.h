// Copyright 2022 Unknown Worlds Entertainment.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "OMEGALIGNSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class OMEGALIGN_API UOMEGALIGNSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
		
	UPROPERTY()
	TArray<AActor*> PersistentSelectedActors;
};
