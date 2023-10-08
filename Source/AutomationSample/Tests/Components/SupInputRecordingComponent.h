// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tests/Utils/SupInputRecordingUtils.h"
#include "SupInputRecordingComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AUTOMATIONSAMPLE_API USupInputRecordingComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USupInputRecordingComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere)
    FString FileName{"CharacterTestInput"};

private:
    FInputData InputData;

    FBindingsData MakeBindingsData() const;

    FString GenerateFileName() const;
};
