// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSInventoryActorComponent.h"

UTPSInventoryActorComponent::UTPSInventoryActorComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UTPSInventoryActorComponent::TryAddItem(const FInventoryData& Data)
{
    if (Data.Score < 0)
    {
        return false;
    }

    if (!Inventory.Contains(Data.Type))
    {
        Inventory.Add(Data.Type, 0);
    }

    const auto NextScore = Inventory[Data.Type] + Data.Score;
    if (NextScore > InventoryLimits[Data.Type])
    {
        return false;
    }

    Inventory[Data.Type] = NextScore;

    return true;
}

int32 UTPSInventoryActorComponent::GetInventoryAmountByType(EInventoryItemType Type) const
{
    return Inventory.Contains(Type) ? Inventory[Type] : 0;
}

void UTPSInventoryActorComponent::BeginPlay()
{
    Super::BeginPlay();

#if !UE_BUILD_SHIPPING
    const UEnum* InvEnum = StaticEnum<EInventoryItemType>();
    check(InvEnum);
    for (int32 i = 0; i < InvEnum->NumEnums() - 1; ++i)
    {
        const EInventoryItemType EnumElem = static_cast<EInventoryItemType>(i);
        const bool LimitCheckCondition = InventoryLimits.Contains(EnumElem) && InventoryLimits[EnumElem] >= 0;
        const FString EnumElemName = UEnum::GetValueAsString(EnumElem);
        checkf(LimitCheckCondition, TEXT("InventoryLimits does not contain %s or less zero"), *EnumElemName);
        Inventory.Add(EnumElem, 0);
    }
#endif
}
