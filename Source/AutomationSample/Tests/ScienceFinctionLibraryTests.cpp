// Fill out your copyright notice in the Description page of Project Settings.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/ScienceFinctionLibraryTests.h"
#include "TestUtils.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Science/ScienceFunctionLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciSimple, "AutomationSample.Science.Fibonacci.Simple",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciStress, "AutomationSample.Science.Fibonacci.Stress",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::LowPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FFibonacciLogHasErrors, "AutomationSample.Science.Fibonacci.LogHasErrors",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

bool FFibonacciSimple::RunTest(const FString& Parameters)
{
    AddInfo("This test checks that UScienceFunctionLibrary::Fibonacci returns the correct value for a given input");

    // 0 1 1 2 3 5 8 13 21 34 55 89 144 233 377

    const TArray<TPS::Test::TestPayload<int32, int32>> TestData{{0, 0}, {1, 1}, {2, 1}, {3, 2}, {4, 3}, {5, 5}, {6, 8}, {7, 13}, {8, 21},
        {9, 34}, {10, 55}, {11, 89}, {12, 144}, {13, 233}, {14, 377}};

    for (const auto& Test : TestData)
    {
        TestEqual(FString::Printf(TEXT("test value: %i, expected value: %i"), Test.TestValue, Test.ExpectedValue),
            UScienceFunctionLibrary::Fibonacci(Test.TestValue), Test.ExpectedValue);
    }

    return true;
}

bool FFibonacciStress::RunTest(const FString& Parameters)
{
    AddInfo("Fibonacci Stress Test");
    int32 PrevPrevValue = 0;
    int32 PrevValue = 1;
    for (int32 i = 2; i < 40; ++i)
    {
        const int32 CurrentValue = UScienceFunctionLibrary::Fibonacci(i);
        TestTrueExpr(CurrentValue == PrevPrevValue + PrevValue);

        PrevPrevValue = PrevValue;
        PrevValue = CurrentValue;
    }
    return true;
}

bool FFibonacciLogHasErrors::RunTest(const FString& Parameters)
{
    AddInfo("This test checks that UScienceFunctionLibrary::Fibonacci logs an error for invalid input");
    AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::Contains);
    UScienceFunctionLibrary::Fibonacci(-1);
    return true;
}

#endif  // WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS
