// Copyright Epic Games, Inc. All Rights Reserved.

#include "AutomationSampleGameMode.h"
#include "AutomationSampleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAutomationSampleGameMode::AAutomationSampleGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}
