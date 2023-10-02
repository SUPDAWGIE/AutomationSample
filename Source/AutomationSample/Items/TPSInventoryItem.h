// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSTypes.h"
#include "GameFramework/Actor.h"
#include "TPSInventoryItem.generated.h"

class USphereComponent;

UCLASS()
class AUTOMATIONSAMPLE_API ATPSInventoryItem : public AActor
{
    GENERATED_BODY()

public:
    ATPSInventoryItem();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
    UPROPERTY(VisibleAnywhere)
    USphereComponent* SphereComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FInventoryData InventoryData;
};