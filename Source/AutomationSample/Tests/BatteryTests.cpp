// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/BatteryTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Items/Battery.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBatteryTests, "AutomationSample.Items.Battery",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FBatteryTests::RunTest(const FString& Parameters)
{
    using namespace TPS;

    AddInfo("This test checks that the battery works as expected");

    const Battery BatteryDefault;
    TestTrueExpr(FMath::IsNearlyEqual(BatteryDefault.GetPercent(), 1.0f));
    TestTrueExpr(BatteryDefault.GetColor() == FColor::Green);
    TestTrueExpr(BatteryDefault.ToString().Equals(TEXT("100%")));

    const Battery BatteryObject2{0.5f};
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject2.GetPercent(), 0.5f));
    TestTrueExpr(BatteryObject2.GetColor() == FColor::Yellow);
    TestTrueExpr(BatteryObject2.ToString().Equals(TEXT("50%")));

    const auto BatteryTestFunc = [this](float Percent, const FColor& Color, const FString& PercentString)
    {
        const Battery BatteryObject{Percent};
        TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), FMath::Clamp(Percent, 0.0f, 1.0f)));
        TestTrueExpr(BatteryObject.GetColor() == Color);
        TestTrueExpr(BatteryObject.ToString().Equals(PercentString));
    };

    BatteryTestFunc(1.0f, FColor::Green, TEXT("100%"));
    BatteryTestFunc(0.9f, FColor::Green, TEXT("90%"));
    BatteryTestFunc(0.8f, FColor::Yellow, TEXT("80%"));
    BatteryTestFunc(0.7f, FColor::Yellow, TEXT("70%"));
    BatteryTestFunc(0.3f, FColor::Red, TEXT("30%"));
    BatteryTestFunc(0.0f, FColor::Red, TEXT("0%"));

    AddInfo("This test checks that the battery can be charged");

    Battery BatteryObject{0.6f};
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));
    BatteryObject.Discharge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.5f));
    BatteryObject.Charge();
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));

    AddInfo("This test checks that the battery can't be overcharged and discharged to negative values");

    for (int32 i = 0; i < 100; ++i)
    {
        BatteryObject.Charge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 1.0f));
    for (int32 i = 0; i < 100; ++i)
    {
        BatteryObject.Discharge();
    }
    TestTrueExpr(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.0f));

    AddInfo("Battery comparison test");

    const Battery BatteryLow{0.1f};
    const Battery BatteryHigh{0.9f};

    TestTrueExpr(BatteryHigh >= BatteryLow);
    TestTrueExpr(FMath::Max(BatteryHigh, BatteryLow) == BatteryHigh);

    AddInfo("Battery in memory comparison test");

    TestNotSame("BatteryLow != BatteryHigh", BatteryLow, BatteryHigh);

    const Battery BatteryHighCopy{BatteryHigh};
    TestNotSame("BatteryHigh != BatteryHighCopy", BatteryHigh, BatteryHighCopy);

    const Battery& BatteryHighRef = BatteryHigh;
    TestSame("BatteryHigh == BatteryHighRef", BatteryHigh, BatteryHighRef);

    return true;
}

#endif
