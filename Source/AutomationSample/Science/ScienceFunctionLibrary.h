// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScienceFunctionLibrary.generated.h"

UCLASS()
class AUTOMATIONSAMPLE_API UScienceFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * Fibonacci function
     *
     * \f$F_n = F_{n-1} + F_{n-2}\f$
     *
     * @param N   Fibonacci index
     * @return    Fibonacci value
     */
    UFUNCTION(BlueprintPure, Category = "Science")
    static int32 Fibonacci(int32 N);

    /**
     *  Factorial function
     *
     * \f$N! = N \cdot (N-1) \cdot (N-2) \cdot ... \cdot 2 \cdot 1\f$
     *
     * @param   N   Input value
     * @return  Factorial of N
     */
    UFUNCTION(BlueprintPure, Category = "Science")
    static int32 Factorial(int32 N);
};
