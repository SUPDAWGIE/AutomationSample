// Fill out your copyright notice in the Description page of Project Settings.

#include "PauseMenuWidget.h"

#include "SupPlayerController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(CloseMenuButton);
    CloseMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnCloseMenu);
}

void UPauseMenuWidget::OnCloseMenu()
{
    if (!GetWorld())
    {
        return;
    }
    if (const auto PC = Cast<ASupPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
    {
        PC->ToggleGamePause();
    }
}
