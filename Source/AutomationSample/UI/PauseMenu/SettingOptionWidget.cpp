// Fill out your copyright notice in the Description page of Project Settings.

#include "SettingOptionWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Settings/SupGameSetting.h"

void USettingOptionWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    check(SettingName);
    check(SettingCurrentValue);
    check(NextOptionButton);
    check(PreviousOptionButton);

    NextOptionButton->OnClicked.AddDynamic(this, &USettingOptionWidget::OnNextOption);
    PreviousOptionButton->OnClicked.AddDynamic(this, &USettingOptionWidget::OnPreviousOption);
}

void USettingOptionWidget::Init(USupGameSetting* InSetting)
{
    Setting = MakeWeakObjectPtr(InSetting);
    check(Setting.IsValid());

    UpdateTexts();
}

void USettingOptionWidget::UpdateTexts()
{
    if (!Setting.IsValid())
    {
        return;
    }

    SettingName->SetText(Setting->GetName());
    SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
}

void USettingOptionWidget::OnNextOption()
{
    if (!Setting.IsValid())
    {
        return;
    }
    Setting->ApplyNextOption();
    SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
}

void USettingOptionWidget::OnPreviousOption()
{
    if (!Setting.IsValid())
    {
        return;
    }
    Setting->ApplyPreviousOption();
    SettingCurrentValue->SetText(Setting->GetCurrentOption().Name);
}
