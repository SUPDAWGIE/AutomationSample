// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/TPSInventoryItemTests.h"
#include "AutomationSampleCharacter.h"
#include "Components/TPSInventoryActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/OutputDeviceNull.h"
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryDataShouldBeSetupCorrectly,
    "AutomationSample.Items.Inventory.InventoryDataShouldBeSetupCorrectly",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryCanBeTaken, "AutomationSample.Items.Inventory.InventoryCanBeTaken",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEveryInventoryItemExists, "AutomationSample.Items.Inventory.EveryInventoryItemExists",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
constexpr const char* InventoryItemBlueprintPath =
    "/Script/Engine.Blueprint'/Game/AutomationSample/BP_TPSInventoryItem.BP_TPSInventoryItem'";
constexpr const char* InventoryItemBlueprintTestPath =
    "/Script/Engine.Blueprint'/Game/AutomationSample/Tests/BP_Test_TPSInventoryItem.BP_Test_TPSInventoryItem'";
}  // namespace

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

    // Same but lambda
    // ForEach<ECollisionChannel>([&, SphereComponent](const ECollisionChannel& EElement)
    //    {
    //        if (EElement != ECollisionChannel::ECC_OverlapAll_Deprecated)
    //        {
    //            TestTrueExpr(SphereComponent->GetCollisionResponseToChannel(EElement) == ECollisionResponse::ECR_Overlap);
    //        }
    //    });

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

bool FInventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
    LevelScope(TEXT("Game/AutomationSample/Tests/EmptyTestLevel"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector(1000.f)};
    ATPSInventoryItem* InventoryItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBlueprintTestPath, InitialTransform);
    if (!TestNotNull("Inventory Item could be created", InventoryItem))
    {
        return false;
    }

    // const FVector TestVector{1.f, 2.f, 3.f};
    // CallFuncByNameWithParams(InventoryItem, TEXT("SetTestData"), {FString::FromInt(123), FString::SanitizeFloat(456.789f), FString("\"My
    // cool string\""), FString("Yes"), FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), TestVector.X, TestVector.Y, TestVector.Z)});

    const FLinearColor Color = FLinearColor::Yellow;
    const FInventoryData InventoryData{EInventoryItemType::CYLINDER, 13};
    CallFuncByNameWithParams(InventoryItem, TEXT("SetInventoryData"), {InventoryData.ToString(), Color.ToString()});

    const UTextRenderComponent* TextRenderComponent = InventoryItem->FindComponentByClass<UTextRenderComponent>();
    if (!TestNotNull("Text Render Component could be found", TextRenderComponent))
    {
        return false;
    }

    TestTrueExpr(TextRenderComponent->Text.ToString().Equals(FString::FromInt(InventoryData.Score)));
    TestTrueExpr(TextRenderComponent->TextRenderColor == Color.ToFColor(true));

    const UStaticMeshComponent* StaticMeshComponent = InventoryItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh Component could be found", StaticMeshComponent))
    {
        return false;
    }

    const auto Material = StaticMeshComponent->GetMaterial(0);
    if (!TestNotNull("Material could be found", Material))
    {
        return false;
    }

    FLinearColor MaterialColor;
    Material->GetVectorParameterValue(FName("Color"), MaterialColor);
    TestTrueExpr(MaterialColor == Color);

    return true;
}

bool FInventoryCanBeTaken::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(TEXT("/Game/AutomationSample/Tests/EmptyTestLevel"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector(1000.f)};
    ATPSInventoryItem* InventoryItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBlueprintTestPath, InitialTransform);
    if (!TestNotNull("Inventory Item could be created", InventoryItem))
    {
        return false;
    }

    // const FVector TestVector{1.f, 2.f, 3.f};
    // CallFuncByNameWithParams(InventoryItem, TEXT("SetTestData"), {FString::FromInt(123), FString::SanitizeFloat(456.789f), FString("\"My
    // cool string\""), FString("Yes"), FString::Printf(TEXT("(X=%f,Y=%f,Z=%f)"), TestVector.X, TestVector.Y, TestVector.Z)});

    const FLinearColor Color = FLinearColor::Yellow;
    const EInventoryItemType ItemType = EInventoryItemType::CYLINDER;
    const int32 ItemScore = 13;
    const FInventoryData InventoryData{ItemType, ItemScore};
    CallFuncByNameWithParams(InventoryItem, TEXT("SetInventoryData"), {InventoryData.ToString(), Color.ToString()});

    TArray<AActor*> Pawns;
    UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), Pawns);
    if (!TestTrue("There is one pawn in the world", Pawns.Num() == 1))
    {
        return false;
    }

    AAutomationSampleCharacter* Character = Cast<AAutomationSampleCharacter>(Pawns[0]);
    if (!TestNotNull("Character could be found", Character))
    {
        return false;
    }

    UTPSInventoryActorComponent* InventoryComponent = Character->FindComponentByClass<UTPSInventoryActorComponent>();
    if (!TestNotNull("Inventory Component could be found", InventoryComponent))
    {
        return false;
    }

    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(ItemType) == 0);

    Character->SetActorLocation(InitialTransform.GetLocation());

    TestTrueExpr(InventoryComponent->GetInventoryAmountByType(ItemType) == ItemScore);

    TestTrueExpr(!IsValid(InventoryItem));

    TArray<AActor*> InventoryItems;

    UGameplayStatics::GetAllActorsOfClass(Character->GetWorld(), ATPSInventoryItem::StaticClass(), InventoryItems);
    TestTrueExpr(InventoryItems.Num() == 0);

    return true;
}

bool FEveryInventoryItemExists::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(TEXT("/Game/AutomationSample/Tests/EmptyTestLevel"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    ENUM_LOOP_START(EInventoryItemType, EnumElem)
    const FTransform InitialTransform{FVector(100.f * (index + 1))};
    ATPSInventoryItem* InventoryItem = CreateBlueprint<ATPSInventoryItem>(World, InventoryItemBlueprintTestPath, InitialTransform);
    if (!TestNotNull("Inventory Item could be created", InventoryItem))
    {
        return false;
    }

    const FLinearColor Color = FLinearColor::Red;
    const EInventoryItemType ItemType = EnumElem;
    const int32 ItemScore = 13;
    const FInventoryData InventoryData{ItemType, ItemScore};
    CallFuncByNameWithParams(InventoryItem, TEXT("SetInventoryData"), {InventoryData.ToString(), Color.ToString()});

    const UStaticMeshComponent* StaticMeshComponent = InventoryItem->FindComponentByClass<UStaticMeshComponent>();
    if (!TestNotNull("Static Mesh Component could be found", StaticMeshComponent))
    {
        return false;
    }

    const FString MeshMsg = FString::Printf(TEXT("Mesh for %s is not set"), *InventoryItem->GetName());
    TestNotNull(MeshMsg, StaticMeshComponent->GetStaticMesh().Get());

    ENUM_LOOP_END
    return true;
}

#endif
