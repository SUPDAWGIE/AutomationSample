// Fill out your copyright notice in the Description page of Project Settings.

#include "Tests/Components/SupInputRecordingComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "Kismet/GameplayStatics.h"
#include "Tests/TestUtils.h"
#include "Tests/Utils/SupJsonUtils.h"

using namespace TPS::Test;

USupInputRecordingComponent::USupInputRecordingComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USupInputRecordingComponent::BeginPlay()
{
    Super::BeginPlay();

    check(GetOwner());
    check(GetWorld());
    check(GetOwner()->InputComponent);

    InputData.InitialTransform = GetOwner()->GetActorTransform();
    InputData.Bindings.Add(MakeBindingsData());
}

void USupInputRecordingComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    JsonUtils::WriteInputData(GenerateFileName(), InputData);
}

void USupInputRecordingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    InputData.Bindings.Add(MakeBindingsData());
}

FBindingsData USupInputRecordingComponent::MakeBindingsData() const
{
    FBindingsData BindingsData;
    BindingsData.WorldTime = GetWorld()->GetTimeSeconds();

    const UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetOwner()->InputComponent);
    const APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    UEnhancedPlayerInput* EnhancedInput = Cast<UEnhancedPlayerInput>(PlayerController->PlayerInput);

    for (auto& ActionEventBinding : EnhancedInputComponent->GetActionEventBindings())
    {
        BindingsData.AxisValues.Add(FAxisData{
            ActionEventBinding->GetAction()->GetFName(), EnhancedInput->GetActionValue(ActionEventBinding->GetAction()).GetMagnitude()});
    }

    return BindingsData;
}

FString USupInputRecordingComponent::GenerateFileName() const
{
    FString SaveDir = FPaths::ProjectSavedDir();
    FString Date = FDateTime::Now().ToString();
    FString FullFileName = FString::Printf(TEXT("%s_%s.json"), *Date, *this->FileName);
    return SaveDir.Append("/Tests/").Append(FullFileName);
}
