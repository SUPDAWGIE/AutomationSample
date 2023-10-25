// Fill out your copyright notice in the Description page of Project Settings.

#include "SupPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedPlayerInput.h"

ASupPlayerController::ASupPlayerController()
{
    bShouldPerformFullTickWhenPaused = true;
}

void ASupPlayerController::ToggleGamePause()
{
    bInPause=!bInPause;
    
        SetPause(bInPause);
                bShowMouseCursor = bInPause;
    bInPause                                                                     //
        ? SetInputMode(FInputModeGameAndUI().SetHideCursorDuringCapture(false))  //
        : SetInputMode(FInputModeGameOnly());
    OnGamePause.Broadcast(bInPause);
}

void ASupPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }

    check(InputComponent);
    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    check(EnhancedInputComponent);

    EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &ASupPlayerController::ToggleGamePause);
}
