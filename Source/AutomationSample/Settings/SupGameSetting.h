// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SupGameSetting.generated.h"

struct FSettingOption
{
    FText Name;   ///< Display name of the option
    int32 Value;  ///< Value of the option
};

UCLASS()
class USupGameSetting : public UObject
{
    GENERATED_BODY()

public:
    void SetName(const FText& InName);

    //! Set the options for this setting
    void SetOptions(const TArray<FSettingOption>& InOptions);

    FSettingOption GetCurrentOption() const;
    FText GetName() const;

    //! \attention Function might be set before object using
    void AddGetter(TFunction<int32()> Func);

    //! \attention Function might be set before object using
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
    FORCENOINLINE int32 GetCurrentIndex() const;
};
