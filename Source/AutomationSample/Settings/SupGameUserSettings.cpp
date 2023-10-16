// Fill out your copyright notice in the Description page of Project Settings.

#include "Settings/SupGameUserSettings.h"

#include "SupGameSetting.h"

#define BIND_SETTINGS_FUNC(FUNC) \
    [&](int32 Level)             \
    {                            \
        FUNC(Level);             \
        ApplySettings(false);    \
    }

#define LOCTEXT_NAMESPACE "SupGameUserSettings"

USupGameUserSettings::USupGameUserSettings()
{
    const TArray<FSettingOption> VFXOptions = {{LOCTEXT("Low_Loc", "Low"), 0}, {LOCTEXT("Medium_Loc", "Medium"), 1},
        {LOCTEXT("High_Loc", "High"), 2}, {LOCTEXT("Epic_Loc", "Epic"), 3}};

    {
        auto* Setting = NewObject<USupGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("AntiAliasing_Loc", "Anti-Aliasing"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetAntiAliasingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetAntiAliasingQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<USupGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Textures_Loc", "Textures"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetTextureQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetTextureQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<USupGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("GlobalIllumination_Loc", "Global Illumination"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetGlobalIlluminationQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetGlobalIlluminationQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<USupGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Shadows_Loc", "Shadows"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetShadowQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetShadowQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<USupGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("PostProcessing_Loc", "Post Processing"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetPostProcessingQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetPostProcessingQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<USupGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Reflections_Loc", "Reflections"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetReflectionQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetReflectionQuality));
        VideoSettings.Add(Setting);
    }

    {
        auto* Setting = NewObject<USupGameSetting>();
        check(Setting);
        Setting->SetName(LOCTEXT("Effects_Loc", "Effects"));
        Setting->SetOptions(VFXOptions);
        Setting->AddGetter([&]() { return GetVisualEffectQuality(); });
        Setting->AddSetter(BIND_SETTINGS_FUNC(SetVisualEffectQuality));
        VideoSettings.Add(Setting);
    }
}

USupGameUserSettings* USupGameUserSettings::Get()
{
    return GEngine ? Cast<USupGameUserSettings>(GEngine->GetGameUserSettings()) : nullptr;
}

const TArray<USupGameSetting*>& USupGameUserSettings::GetVideoSettings() const
{
    return VideoSettings;
}

void USupGameUserSettings::RunBenchmark()
{
    RunHardwareBenchmark();
    ApplySettings(false);
    OnSettingsUpdated.Broadcast();
}

#undef LOCTEXT_NAMESPACE