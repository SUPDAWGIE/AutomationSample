// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SupInputRecordingUtils.h"

namespace TPS
{
namespace Test
{
class JsonUtils
{
public:
    static bool WriteInputData(const FString& FilePath, const FInputData& InputData);
    static bool ReadInputData(const FString& FilePath, FInputData& InputData);
};
}  // namespace Test
}  // namespace TPS
