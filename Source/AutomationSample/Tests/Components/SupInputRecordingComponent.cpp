// Fill out your copyright notice in the Description page of Project Settings.

#include "Tests/Components/SupInputRecordingComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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
        // UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
        // Subsystem->GetPlayerInput()->GetActionValue(ActionEventBinding->GetAction());
        // const FVector Value =
        // Subsystem->GetPlayerInput()->GetActionValue(ActionEventBinding->GetAction()).ConvertToType(EInputActionValueType::Axis3D).Get<FVector>();
        BindingsData.AxisValues.Add(FAxisData{
            ActionEventBinding->GetAction()->GetFName(), EnhancedInput->GetActionValue(ActionEventBinding->GetAction()).Get<FVector>()});
        // print value
        GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red,
            FString::Printf(TEXT("Action: %s, Value: %s"), *ActionEventBinding->GetAction()->GetFName().ToString(),
                *EnhancedInput->GetActionValue(ActionEventBinding->GetAction()).Get<FVector>().ToString()));
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
