// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSTypes.h"
#include "Components/ActorComponent.h"
#include "TPSInventoryActorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class AUTOMATIONSAMPLE_API UTPSInventoryActorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTPSInventoryActorComponent();

    /**
     * Try to add an item to the inventory
     * Score of the data can't be < 0
     * Final score might be less than the corresponding limit
     * @param Data
     * @return true if the item was added
     */
    bool TryAddItem(const FInventoryData& Data);

    UFUNCTION(BlueprintCallable)
    int32 GetInventoryAmountByType(EInventoryItemType Type) const;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TMap<EInventoryItemType, int32> InventoryLimits;

private:
    TMap<EInventoryItemType, int32> Inventory;
};
