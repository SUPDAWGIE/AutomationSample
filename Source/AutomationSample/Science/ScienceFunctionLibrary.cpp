// Fill out your copyright notice in the Description page of Project Settings.

#include "Science/ScienceFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogScience, Log, All);

int32 UScienceFunctionLibrary::Fibonacci(int32 N)
{
    // check(N >= 0);
    if (N < 0)
    {
        UE_LOG(LogScience, Error, TEXT("Invalid input for Fibonacci: %i"), N);
        return -1;
    }
    return N <= 1 ? N : Fibonacci(N - 1) + Fibonacci(N - 2);
}

int32 UScienceFunctionLibrary::Factorial(int32 N)
{
    return N <= 1 ? 1 : N * Factorial(N - 1);
}
