// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SupPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePauseDelegate, bool, bIsPaused);

class UInputMappingContext;
class UInputAction;

UCLASS()
class AUTOMATIONSAMPLE_API ASupPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ASupPlayerController();

    void ToggleGamePause();

    UPROPERTY(BlueprintAssignable, Category = "GameMode")
    FOnGamePauseDelegate OnGamePause;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
    UInputAction* PauseAction;

    virtual void SetupInputComponent() override;

private:
    bool bInPause{false};
};
