// Fill out your copyright notice in the Description page of Project Settings.

#include "SupGameSetting.h"

DEFINE_LOG_CATEGORY_STATIC(LogSupGameSetting, All, All);

void USupGameSetting::SetName(const FText& InName)
{
    Name = InName;
}

void USupGameSetting::SetOptions(const TArray<FSettingOption>& InOptions)
{
    Options = InOptions;
}

FSettingOption USupGameSetting::GetCurrentOption() const
{
    const int32 CurrentValue = GetCurrentValue();
    const auto Option = Options.FindByPredicate([&](const auto& InOption) { return InOption.Value == CurrentValue; });
    if (!Option)
    {
        UE_LOG(LogSupGameSetting, Error, TEXT("Failed to find option with value %d"), CurrentValue);
        return FSettingOption{};
    }
    return *Option;
}

FText USupGameSetting::GetName() const
{
    return Name;
}

void USupGameSetting::AddGetter(TFunction<int32()> Func)
{
    Getter = Func;
}

void USupGameSetting::AddSetter(TFunction<void(int32)> Func)
{
    Setter = Func;
}

void USupGameSetting::ApplyNextOption()
{
    const int32 CurrentIndex = GetCurrentIndex();
    if (CurrentIndex == INDEX_NONE)
    {
        UE_LOG(LogSupGameSetting, Error, TEXT("Failed to get current index"));
        return;
    }
    const int32 NextIndex = (CurrentIndex + 1) % Options.Num();
    SetCurrentValue(NextIndex);
}

void USupGameSetting::ApplyPreviousOption()
{
    const int32 CurrentIndex = GetCurrentValue();
    if (CurrentIndex == INDEX_NONE)
    {
        UE_LOG(LogSupGameSetting, Error, TEXT("Failed to get current index"));
        return;
    }
    const int32 PreviousIndex = CurrentIndex == 0 ? Options.Num() - 1 : CurrentIndex - 1;
    SetCurrentValue(PreviousIndex);
}

int32 USupGameSetting::GetCurrentValue() const
{
    if (!Getter)
    {
        UE_LOG(LogSupGameSetting, Error, TEXT("Getter is not set"));
        return INDEX_NONE;
    }

    return Getter();
}

void USupGameSetting::SetCurrentValue(int32 InValue)
{
    if (!Setter)
    {
        UE_LOG(LogSupGameSetting, Error, TEXT("Setter is not set"));
        return;
    }
    Setter(InValue);
}

int32 USupGameSetting::GetCurrentIndex() const
{
    const int32 CurrentValue = GetCurrentValue();
    return Options.IndexOfByPredicate([&](const auto& InOption) { return InOption.Value == CurrentValue; });
}
