// Fill out your copyright notice in the Description page of Project Settings.

#include "Tests/SandboxTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "AutomationSample.Math.MaxInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "AutomationSample.Math.Sqrt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("This test checks that FMath::Max returns the maximum of two integers");
    // AddWarning("This is a warning test");
    TestTrue("2 different positive numbers", FMath::Max(13, 25) == 25);
    TestEqual("2 equal positive numbers", FMath::Max(25, 25), 25);
    TestTrueExpr(FMath::Max(0, 123) == 123);
    TestTrue("0,0", FMath::Max(0, 0) == 0);
    TestTrue("negative, 0", FMath::Max(-2345, 0) == 0);
    TestTrue("negative, negative", FMath::Max(-45, -67) == -45);
    TestTrue("negative, negative equal", FMath::Max(-9, -9) == -9);
    TestTrue("negative, positive", FMath::Max(-9, 9) == 9);

    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    AddInfo("This test checks that FMath::Sqrt returns the square root of a number");
    TestEqual("sqrt(4)", FMath::Sqrt(4.f), 2.0f, 0.1f);
    TestEqual("sqrt(3)", FMath::Sqrt(3.f), 1.73205f, 0.00001f);
    return true;
}
