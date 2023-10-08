// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SupInputRecordingUtils.generated.h"

USTRUCT()
struct FAxisData
{
    GENERATED_BODY()

    UPROPERTY()
    FName Name;

    UPROPERTY()
    float Value{0.0f};
};

USTRUCT()
struct FBindingsData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FAxisData> AxisValues;

    UPROPERTY()
    float WorldTime{0.0f};
};

USTRUCT()
struct FInputData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FBindingsData> Bindings;

    UPROPERTY()
    FTransform InitialTransform;
};