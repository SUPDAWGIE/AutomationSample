// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "SettingOptionWidget.generated.h"

class UButton;
class UVideoSettingsWidget;
class UTextBlock;
class USupGameSetting;
/**
 *
 */
UCLASS()
class AUTOMATIONSAMPLE_API USettingOptionWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* SettingName;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SettingCurrentValue;

    UPROPERTY(meta = (BindWidget))
    UButton* NextOptionButton;

    UPROPERTY(meta = (BindWidget))
    UButton* PreviousOptionButton;

    virtual void NativeOnInitialized() override;

private:
    TWeakObjectPtr<USupGameSetting> Setting;

    void Init(USupGameSetting* InSetting);
    void UpdateTexts();

    UFUNCTION()
    void OnNextOption();

    UFUNCTION()
    void OnPreviousOption();

    friend UVideoSettingsWidget;
};
