// My game copyright

#include "Settings/SupGameUserSettings.h"
#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/TPSGameUsersSettingsTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsShouldExist, "AutomationSample.GameUserSettings.SettingsShouldExist",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

bool FSettingsShouldExist::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/EmptyTestLevel");

    TestTrueExpr(USupGameUserSettings::Get() != nullptr);
    TestTrueExpr(USupGameUserSettings::Get()->GetVideoSettings().Num() == 7);
    return true;
}

#endif
