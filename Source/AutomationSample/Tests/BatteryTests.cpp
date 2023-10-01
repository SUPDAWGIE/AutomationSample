// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/BatteryTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTestClassName, "Category.TestName",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

bool FTestClassName::RunTest(const FString& Parameters)
{
    return true;
}

#endif
