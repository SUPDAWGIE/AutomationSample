// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/TPSGameplayTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "GameFramework/Character.h"
#include "Items/TPSInventoryItem.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedPlayerInput.h"
#include "EnhancedInputComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "AutomationSample.Gameplay.InventoryItemCanBeTakenOnJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh,
    "AutomationSample.Gameplay.InventoryItemCantBeTakenOnJumpIfTooHigh",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
void ExecuteInputAction(const ACharacter* Character, const FString& Command, const float& Value)
{
    const UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Character->InputComponent);
    UEnhancedPlayerInput* EnhancedInput = Cast<UEnhancedPlayerInput>(Character->GetLocalViewingPlayerController()->PlayerInput);
    for (auto& ActionEventBinding : EnhancedInputComponent->GetActionEventBindings())
    {
        if (ActionEventBinding->GetAction()->GetName() == Command)
        {
            EnhancedInput->InjectInputForAction(ActionEventBinding->GetAction(), Value);
            break;
        }
    }
}
}  // namespace

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, ACharacter*, Character);

bool FJumpLatentCommand::Update()
{
    if (Character)
    {
        ExecuteInputAction(Character, "IA_Jump", 1.0f);
    }
    return true;
}
bool FInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(TEXT("/Game/AutomationSample/Tests/InventoryTestLevel1"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character could be created", Character))
    {
        return false;
    }

    TArray<AActor*> InventoryItems;

    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Inventory items count", InventoryItems.Num(), 1))
    {
        return false;
    }

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [this, Character]()
        {
            TArray<AActor*> InventoryItemsAfterJump;
            UGameplayStatics::GetAllActorsOfClass(Character, ATPSInventoryItem::StaticClass(), InventoryItemsAfterJump);
            TestTrueExpr(InventoryItemsAfterJump.Num() == 0);
        },
        2.0f));

    return true;
}

bool FInventoryItemCantBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(TEXT("/Game/AutomationSample/Tests/InventoryTestLevel2"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    ACharacter* Character = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!TestNotNull("Character could be created", Character))
    {
        return false;
    }

    TArray<AActor*> InventoryItems;

    UGameplayStatics::GetAllActorsOfClass(World, ATPSInventoryItem::StaticClass(), InventoryItems);
    if (!TestEqual("Inventory items count", InventoryItems.Num(), 1))
    {
        return false;
    }

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [this, Character]()
        {
            TArray<AActor*> InventoryItemsAfterJump;
            UGameplayStatics::GetAllActorsOfClass(Character, ATPSInventoryItem::StaticClass(), InventoryItemsAfterJump);
            TestTrueExpr(InventoryItemsAfterJump.Num() == 1);
        },
        2.0f));

    return true;
}

#endif
