// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SupGameSetting.generated.h"

struct FSettingOption
{
    FText Name;
    int32 Value;
};

UCLASS()
class USupGameSetting : public UObject
{
    GENERATED_BODY()

public:
    void SetName(const FText& InName);
    void SetOptions(const TArray<FSettingOption>& InOptions);

    FSettingOption GetCurrentOption() const;
    FText GetName() const;
    void AddGetter(TFunction<int32()> Func);
    void AddSetter(TFunction<void(int32)> Func);

    void ApplyNextOption();
    void ApplyPreviousOption();

private:
    FText Name;
    TArray<FSettingOption> Options;
    TFunction<int32()> Getter;
    TFunction<void(int32)> Setter;

    int32 GetCurrentValue() const;
    void SetCurrentValue(int32 InValue);
    int32 GetCurrentIndex() const;
};
