// Fill out your copyright notice in the Description page of Project Settings.

#include "Tests/Utils/SupJsonUtils.h"

#include "JsonObjectConverter.h"

DEFINE_LOG_CATEGORY_STATIC(LogsonUtils, All, All);

using namespace TPS::Test;

bool JsonUtils::WriteInputData(const FString& FilePath, const FInputData& InputData)
{
    // Serialize to JSON and Write to file
    const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(InputData);
    if (!JsonObject.IsValid())
    {
        UE_LOG(LogsonUtils, Error, TEXT("JsonObject is not valid"));
        return false;
    }
    FString OutputString;
    const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    Writer->Close();
    if (!FFileHelper::SaveStringToFile(OutputString, *FilePath))
    {
        UE_LOG(LogsonUtils, Error, TEXT("Could not save to file: %s"), *FilePath);
        return false;
    }
    return true;
}

bool JsonUtils::ReadInputData(const FString& FilePath, FInputData& InputData)
{
    // Read from file and Deserialize from JSON
    FString InputString;
    if (!FFileHelper::LoadFileToString(InputString, *FilePath))
    {
        UE_LOG(LogsonUtils, Error, TEXT("Could not load from file: %s"), *FilePath);
        return false;
    }
    const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(InputString);
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    if (!FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        UE_LOG(LogsonUtils, Error, TEXT("Could not deserialize from file: %s"), *FilePath);
        return false;
    }
    if (!FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &InputData))
    {
        UE_LOG(LogsonUtils, Error, TEXT("Could not convert to struct from file: %s"), *FilePath);
        return false;
    }
    return true;
}
