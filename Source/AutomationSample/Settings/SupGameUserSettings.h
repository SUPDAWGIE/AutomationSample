// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "SupGameUserSettings.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnSettingsUpdatedDelegate);

class USupGameSetting;

UCLASS()
class AUTOMATIONSAMPLE_API USupGameUserSettings : public UGameUserSettings
{
    GENERATED_BODY()

public:
    USupGameUserSettings();

    static USupGameUserSettings* Get();

    const TArray<USupGameSetting*>& GetVideoSettings() const;

    void RunBenchmark();

    FOnSettingsUpdatedDelegate OnSettingsUpdated;

private:
    UPROPERTY()
    TArray<USupGameSetting*> VideoSettings;
};
