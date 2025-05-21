// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/UITests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "UI/PauseMenu/PauseMenuWidget.h"
#include "UI/PauseMenu/VideoSettingsWidget.h"
#include "Components/VerticalBox.h"
#include "Settings/SupGameSetting.h"
#include "Settings/SupGameUserSettings.h"
#include "UI/PauseMenu/SettingOptionWidget.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPauseMenuShouldBeVisibleOnGamePaused, "AutomationSample.UITests.PauseMenuShouldBeVisibleOnGamePaused",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllVideoSettingsExist, "AutomationSample.UITests.AllVideoSettingsExist",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingCanBeApplied, "AutomationSample.UITests.SettingCanBeApplied",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoBenchmarkShouldWork, "AutomationSample.UITests.AutoBenchmarkShouldWork",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::StressFilter | EAutomationTestFlags::MediumPriority);

using namespace TPS::Test;

namespace
{
void NextSettingClick(int32 SettingIndex)
{
    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const UVerticalBox* VideoSettingsBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    const USettingOptionWidget* SettingOption = Cast<USettingOptionWidget>(VideoSettingsBox->GetChildAt(SettingIndex));
    const UButton* NextSettingButton = Cast<UButton>(FindWidgetByName(SettingOption, "NextOptionButton"));
    NextSettingButton->OnClicked.Broadcast();
}
void DoBenchmarkClick()
{
    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    BenchmarkButton->OnClicked.Broadcast();
}
}  // namespace

bool FPauseMenuShouldBeVisibleOnGamePaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/EmptyTestLevel");

    const APlayerController* PC = AutomationCommon::GetAnyGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    UPauseMenuWidget* PauseMenuWidget = FindWidgetByClass<UPauseMenuWidget>();
    TestTrueExpr(PauseMenuWidget != nullptr);
    TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&, PC]()
        {
            ExecuteInputAction(PC, "IA_Pause", 1.0f);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&, PauseMenuWidget]()
        {
            TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&, PC]()
        {
            ExecuteInputAction(PC, "IA_Pause", 1.0f);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&, PauseMenuWidget]()
        {
            TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
            return true;
        }));
    return true;
}

bool FAllVideoSettingsExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/EmptyTestLevel");

    const APlayerController* PC = AutomationCommon::GetAnyGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    const UVideoSettingsWidget* VideoSettingsWidget = FindWidgetByClass<UVideoSettingsWidget>();
    TestTrueExpr(VideoSettingsWidget != nullptr);

    const UButton* BenchmarkButton = Cast<UButton>(FindWidgetByName(VideoSettingsWidget, "RunBenchmarkButton"));
    TestTrueExpr(BenchmarkButton != nullptr);

    const UVerticalBox* VideoSettingsBox = Cast<UVerticalBox>(FindWidgetByName(VideoSettingsWidget, "VideoSettingsContainer"));
    TestTrueExpr(VideoSettingsBox != nullptr);

    const USupGameUserSettings* GameUserSettings = USupGameUserSettings::Get();
    TestTrueExpr(GameUserSettings != nullptr);

    const TArray<USupGameSetting*>& VideoSettings = GameUserSettings->GetVideoSettings();
    TestTrueExpr(VideoSettings.Num() == VideoSettingsBox->GetChildrenCount());
    TestTrueExpr(VideoSettingsBox->GetChildrenCount() > 0);

    return true;
}

bool FSettingCanBeApplied::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/EmptyTestLevel");

    const APlayerController* PC = AutomationCommon::GetAnyGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    ExecuteInputAction(PC, "IA_Pause", 1.0f);

    USupGameUserSettings* GameUserSettings = USupGameUserSettings::Get();
    const TArray<USupGameSetting*>& VideoSettings = GameUserSettings->GetVideoSettings();
    const int32 GlobalIlluminationQualityBefore = GameUserSettings->GetGlobalIlluminationQuality();
    TestTrueExpr(GlobalIlluminationQualityBefore == VideoSettings[2]->GetCurrentOption().Value);
    NextSettingClick(2);
    const int32 GlobalIlluminationQualityAfter = GameUserSettings->GetGlobalIlluminationQuality();
    TestTrueExpr(GlobalIlluminationQualityAfter != GlobalIlluminationQualityBefore);
    TestTrueExpr(GlobalIlluminationQualityAfter == VideoSettings[2]->GetCurrentOption().Value);

    GameUserSettings->SetGlobalIlluminationQuality(GlobalIlluminationQualityBefore);
    GameUserSettings->ApplySettings(false);

    return true;
}

bool FAutoBenchmarkShouldWork::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/EmptyTestLevel");

    const APlayerController* PC = AutomationCommon::GetAnyGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);

    ExecuteInputAction(PC, "IA_Pause", 1.0f);

    DoBenchmarkClick();

    TArray<int32> SavedSettingValues;
    const auto& VideoSettings = USupGameUserSettings::Get()->GetVideoSettings();
    for (const auto& Setting : VideoSettings)
    {
        SavedSettingValues.Add(Setting->GetCurrentOption().Value);
    }

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        NextSettingClick(i);
    }

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(VideoSettings[i]->GetCurrentOption().Value != SavedSettingValues[i]);
    }

    DoBenchmarkClick();

    for (int32 i = 0; i < VideoSettings.Num(); ++i)
    {
        TestTrueExpr(VideoSettings[i]->GetCurrentOption().Value == SavedSettingValues[i]);
    }

    return true;
}

#endif
