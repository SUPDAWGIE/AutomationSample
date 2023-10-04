// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_AUTOMATION_TESTS

#include "Tests/SandboxTests.h"
#include "TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathMaxInt, "AutomationSample.Math.MaxInt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSqrt, "AutomationSample.Math.Sqrt",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMathSin, "AutomationSample.Math.Sin",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

using namespace TPS::Test;

bool FMathMaxInt::RunTest(const FString& Parameters)
{
    AddInfo("This test checks that FMath::Max returns the maximum of two integers");

    typedef TArray<TestPayload<TInterval<int32>, int32>> MaxIntTestPayload;
    //clang-format off
    MaxIntTestPayload TestData{{{13, 25}, 25}, {TInterval<int32>{25, 25}, 25}, {TInterval<int32>{0, 123}, 123}, {TInterval<int32>{0, 0}, 0},
        {TInterval<int32>{-2345, 0}, 0}, {TInterval<int32>{-45, -67}, -45}, {TInterval<int32>{-9, -9}, -9}, {TInterval<int32>{-9, 9}, 9}};
    //clang-format on

    for (const auto& Test : TestData)
    {
        TestEqual(
            FString::Printf(TEXT("test values: %i, %i, expected value: %i"), Test.TestValue.Min, Test.TestValue.Max, Test.ExpectedValue),
            FMath::Max(Test.TestValue.Min, Test.TestValue.Max), Test.ExpectedValue);
    }

    // AddWarning("This is a warning test");
    // TestTrue("2 different positive numbers", FMath::Max(13, 25) == 25);
    // TestEqual("2 equal positive numbers", FMath::Max(25, 25), 25);
    // TestTrueExpr(FMath::Max(0, 123) == 123);
    // TestTrue("0,0", FMath::Max(0, 0) == 0);
    // TestTrue("negative, 0", FMath::Max(-2345, 0) == 0);
    // TestTrue("negative, negative", FMath::Max(-45, -67) == -45);
    // TestTrue("negative, negative equal", FMath::Max(-9, -9) == -9);
    // TestTrue("negative, positive", FMath::Max(-9, 9) == 9);

    return true;
}

bool FMathSqrt::RunTest(const FString& Parameters)
{
    AddInfo("This test checks that FMath::Sqrt returns the square root of a number");
    TestEqual("sqrt(4)", FMath::Sqrt(4.f), 2.0f, 0.1f);
    TestEqual("sqrt(3)", FMath::Sqrt(3.f), 1.73205f, 0.00001f);
    return true;
}

bool FMathSin::RunTest(const FString& Parameters)
{
    AddInfo("This test checks that FMath::Sin returns the sine of a number");

    typedef float Degrees;
    typedef TArray<TestPayload<Degrees, float>> SinTestPayload;

    const SinTestPayload TestData{{0.f, 0.f}, {30.f, 0.5f}, {45.f, 0.707107f, 0.000001f}, {60.f, 0.866025f, 0.000001f}, {90.f, 1.f},
        {180.f, 0.f}, {270.f, -1.f}, {360.f, 0.f}};

    for (const auto& Test : TestData)
    {
        TestEqual(FString::Printf(TEXT("test value: %f, expected value: %f"), Test.TestValue, Test.ExpectedValue),
            FMath::Sin(FMath::DegreesToRadians(Test.TestValue)), Test.ExpectedValue, Test.Tolerance);
    }

    return true;
}

#endif  // WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS
