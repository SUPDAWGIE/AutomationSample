// Fill out your copyright notice in the Description page of Project Settings.

#include "VideoSettingsWidget.h"

#include "SettingOptionWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Settings/SupGameSetting.h"
#include "Settings/SupGameUserSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogVideoSettingsWidget, All, All);

void UVideoSettingsWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    auto* Settings = USupGameUserSettings::Get();
    if (!Settings)
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("Failed to get SupGameUserSettings"));
        return;
    }

    Settings->LoadSettings();

    auto VideoSettings = Settings->GetVideoSettings();

    check(VideoSettingsContainer);

    VideoSettingsContainer->ClearChildren();
    for (USupGameSetting* Setting : VideoSettings)
    {
        USettingOptionWidget* SettingWidget = CreateWidget<USettingOptionWidget>(GetWorld(), SettingOptionWidgetClass);
        check(SettingWidget);
        SettingWidget->Init(Setting);
        VideoSettingsContainer->AddChild(SettingWidget);
    }

    check(RunBenchmarkButton);
    RunBenchmarkButton->OnClicked.AddDynamic(this, &UVideoSettingsWidget::OnRunBenchmark);

    Settings->OnSettingsUpdated.AddUObject(this, &UVideoSettingsWidget::OnVideoSettingsUpdated);
}

void UVideoSettingsWidget::OnRunBenchmark()
{
    if (!USupGameUserSettings::Get())
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("Failed to get SupGameUserSettings"));
        return;
    }
    USupGameUserSettings::Get()->RunBenchmark();
}

void UVideoSettingsWidget::OnVideoSettingsUpdated()
{
    if (!USupGameUserSettings::Get())
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("Failed to get SupGameUserSettings"));
        return;
    }
    if (!VideoSettingsContainer)
    {
        UE_LOG(LogVideoSettingsWidget, Error, TEXT("VideoSettingsContainer is nullptr"));
        return;
    }
    for (auto* Widget : VideoSettingsContainer->GetAllChildren())
    {
        if (auto* SettingWidget = Cast<USettingOptionWidget>(Widget))
        {
            SettingWidget->UpdateTexts();
        }
    }
}
