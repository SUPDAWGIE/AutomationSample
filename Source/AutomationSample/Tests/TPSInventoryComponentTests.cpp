// My game copyright

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "AutomationSample/Tests/TPSInventoryComponentTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "AutomationSample/Components/TPSInventoryActorComponent.h"
#include "AutomationSample/TPSTypes.h"
#include "Tests/TestUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FComponentCouldBeCreated, "AutomationSample.Components.Inventory.ComponentCouldBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FItemScoresShouldBeZerosByDefault,
    "AutomationSample.Components.Inventory.ItemScoresShouldBeZerosByDefault",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FNegativeItemShouldntBeAdded, "AutomationSample.Components.Inventory.NegativeItemShouldntBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPositiveScoreShouldBeAdded, "AutomationSample.Components.Inventory.PositiveScoreShouldBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FScoreMoreThanLimitCantBeAdded, "AutomationSample.Components.Inventory.ScoreMoreThanLimitCantBeAdded",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
class UTPSInventoryActorComponentTestable : public UTPSInventoryActorComponent
{
public:
    void SetInventoryLimits(const TMap<EInventoryItemType, int32>& NewInventoryLimits) { InventoryLimits = NewInventoryLimits; }
};

TMap<EInventoryItemType, int32> InitLimits(UTPSInventoryActorComponentTestable* InventoryComponent, const int32 InventoryLimit)
{
    TMap<EInventoryItemType, int32> InventoryLimits;
    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    InventoryLimits.Add(EnumElem, InventoryLimit);
    ENUM_LOOP_END

    InventoryComponent->SetInventoryLimits(InventoryLimits);
    return InventoryLimits;
}
}  // namespace

bool FComponentCouldBeCreated::RunTest(const FString& Parameters)
{
    const UTPSInventoryActorComponent* InventoryComponent = NewObject<UTPSInventoryActorComponent>();
    if (!TestNotNull("Inventory Component could be created", InventoryComponent))
    {
        return false;
    }
    return true;
}

bool FItemScoresShouldBeZerosByDefault::RunTest(const FString& Parameters)
{
    const UTPSInventoryActorComponent* InventoryComponent = NewObject<UTPSInventoryActorComponent>();
    if (!TestNotNull("Inventory Component could be created", InventoryComponent))
    {
        return false;
    }

    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == 0);
    ENUM_LOOP_END
    return true;
}

bool FNegativeItemShouldntBeAdded::RunTest(const FString& Parameters)
{
    UTPSInventoryActorComponent* InventoryComponent = NewObject<UTPSInventoryActorComponent>();
    if (!TestNotNull("Inventory Component could be created", InventoryComponent))
    {
        return false;
    }

    const int32 NegativeScore = -1;
    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    const FInventoryData NegativeData{EnumElem, NegativeScore};
    TestTrueExpr(!InventoryComponent->TryAddItem(NegativeData));
    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == 0);
    ENUM_LOOP_END
    return true;
}

bool FPositiveScoreShouldBeAdded::RunTest(const FString& Parameters)
{
    UTPSInventoryActorComponentTestable* InventoryComponent = NewObject<UTPSInventoryActorComponentTestable>();
    if (!TestNotNull("Inventory Component could be created", InventoryComponent))
    {
        return false;
    }

    InitLimits(InventoryComponent, 100);

    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    const FInventoryData PositiveData{EnumElem, 1};
    TestTrueExpr(InventoryComponent->TryAddItem(PositiveData));
    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == 1);
    ENUM_LOOP_END

    return true;
}

bool FScoreMoreThanLimitCantBeAdded::RunTest(const FString& Parameters)
{
    UTPSInventoryActorComponentTestable* InventoryComponent = NewObject<UTPSInventoryActorComponentTestable>();
    if (!TestNotNull("Inventory Component could be created", InventoryComponent))
    {
        return false;
    }

    const int InventoryLimit = 100;
    InitLimits(InventoryComponent, InventoryLimit);

    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    const FInventoryData PositiveData{EnumElem, InventoryLimit + 1};
    TestTrueExpr(!InventoryComponent->TryAddItem(PositiveData));
    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(EnumElem) == 0);
    ENUM_LOOP_END
    return true;
}

#endif
