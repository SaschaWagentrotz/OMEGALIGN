// Copyright 2022 Unknown Worlds Entertainment.


#include "OMEGALIGNSubsystem.h"

void UOMEGALIGNSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Verbose, TEXT("OMEGALIGN Subsystem initialized!"));
}
