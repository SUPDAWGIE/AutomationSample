// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "VideoSettingsWidget.generated.h"

class UVerticalBox;
class USettingOptionWidget;
class UButton;

/**
 *
 */
UCLASS()
class AUTOMATIONSAMPLE_API UVideoSettingsWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* VideoSettingsContainer;

    UPROPERTY(meta = (BindWidget))
    UButton* RunBenchmarkButton;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<USettingOptionWidget> SettingOptionWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnRunBenchmark();

    void OnVideoSettingsUpdated();
};
