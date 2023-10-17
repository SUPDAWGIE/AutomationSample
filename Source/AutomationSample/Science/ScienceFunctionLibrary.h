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
    UFUNCTION(BlueprintPure, Category = "Science")
    static int32 Fibonacci(int32 N);

    /**
     *  Factorial function
     *
     * @param   N   Input value
     * @return  Factorial of N
     */
    UFUNCTION(BlueprintPure, Category = "Science")
    static int32 Factorial(int32 N);
};
