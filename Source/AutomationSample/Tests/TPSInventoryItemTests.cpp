// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/TPSInventoryItemTests.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "Items/TPSInventoryItem.h"
#include "Tests/AutomationCommon.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCppActorCantBeCreated, "AutomationSample.Items.Inventory.CppActorCantBeCreated",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintShouldBeSetupCorrectly, "AutomationSample.Items.Inventory.BlueprintShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
constexpr const char* InventoryItemBlueprintPath =
    "/Script/Engine.Blueprint'/Game/AutomationSample/BP_TPSInventoryItem.BP_TPSInventoryItem'";
}

using namespace TPS::Test;

bool FCppActorCantBeCreated::RunTest(const FString& Parameters)
{
    const FString ExpectedError =
        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSInventoryItem::StaticClass()->GetName());
    AddExpectedError(ExpectedError, EAutomationExpectedErrorFlags::Exact);

    LevelScope(TEXT("Game/AutomationSample/Tests/EmptyTestLevel"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector(1000.f)};
    const ATPSInventoryItem* InventoryItem = World->SpawnActor<ATPSInventoryItem>(ATPSInventoryItem::StaticClass(), InitialTransform);
    if (!TestNull("Inventory Item could not be created", InventoryItem))
    {
        return false;
    }

    return true;
}

bool FBlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope(TEXT("Game/AutomationSample/Tests/EmptyTestLevel"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector(1000.f)};
    const ATPSInventoryItem* InventoryItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBlueprintPath, InitialTransform);
    if (!TestNotNull("Inventory Item could be created", InventoryItem))
    {
        return false;
    }

    const USphereComponent* SphereComponent = InventoryItem->FindComponentByClass<USphereComponent>();
    if (!TestNotNull("Sphere Component could be found", SphereComponent))
    {
        return false;
    }

    TestTrueExpr(SphereComponent->GetUnscaledSphereRadius() >= 30.f);
    TestTrueExpr(SphereComponent->GetCollisionEnabled() == ECollisionEnabled::QueryOnly);
    TestTrueExpr(SphereComponent->GetGenerateOverlapEvents());
    TestTrueExpr(InventoryItem->GetRootComponent() == SphereComponent);

    ENUM_LOOP_START(ECollisionChannel, EElement)
    if (EElement != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(SphereComponent->GetCollisionResponseToChannel(EElement) == ECollisionResponse::ECR_Overlap);
    }
    ENUM_LOOP_END

    const UTextRenderComponent* TextRenderComponent = InventoryItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text Render Component could be found", TextRenderComponent))
    {
        return false;
    }

    const UStaticMeshComponent* StaticMeshComponent = InventoryItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh Component could be found", StaticMeshComponent))
    {
        return false;
    }

    TestTrueExpr(StaticMeshComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision);

    return true;
}

#endif
