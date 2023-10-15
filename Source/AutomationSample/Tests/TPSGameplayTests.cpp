// My game copyright

#include "EnhancedInputSubsystems.h"
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
#include "Utils/SupInputRecordingUtils.h"
#include "Utils/SupJsonUtils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCanBeTakenOnJump, "AutomationSample.Gameplay.InventoryItemCanBeTakenOnJump",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FInventoryItemCantBeTakenOnJumpIfTooHigh,
    "AutomationSample.Gameplay.InventoryItemCantBeTakenOnJumpIfTooHigh",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsCanBeTakenOnMovement, "AutomationSample.Gameplay.AllItemsCanBeTakenOnMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAllItemsCanBeTakenOnRecordedMovement, "AutomationSample.Gameplay.AllItemsCanBeTakenOnRecordedMovement",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
void ExecuteInputAction(
    const ACharacter* Character, const FString& Command, const float& Value, const TArray<UInputModifier*>& Modifiers = {})
{
    const UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Character->InputComponent);
    UEnhancedPlayerInput* EnhancedInput = Cast<UEnhancedPlayerInput>(Character->GetLocalViewingPlayerController()->PlayerInput);

    for (auto& ActionEventBinding : EnhancedInputComponent->GetActionEventBindings())
    {
        if (ActionEventBinding->GetAction()->GetName() == Command)
        {
            EnhancedInput->InjectInputForAction(ActionEventBinding->GetAction(), Value, Modifiers);
            break;
        }
    }
}

void ExecuteVectorInputAction(const ACharacter* Character, const FString& Command, const FVector& Value,
    const TArray<UInputModifier*>& Modifiers = {}, const TArray<UInputTrigger*>& Triggers = {})
{
    const UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Character->InputComponent);
    UEnhancedPlayerInput* EnhancedInput = Cast<UEnhancedPlayerInput>(Character->GetLocalViewingPlayerController()->PlayerInput);

    for (auto& ActionEventBinding : EnhancedInputComponent->GetActionEventBindings())
    {
        if (ActionEventBinding->GetAction()->GetName() == Command)
        {
            UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                Character->GetLocalViewingPlayerController()->GetLocalPlayer());
            Subsystem->InjectInputVectorForAction(ActionEventBinding->GetAction(), Value, Modifiers, Triggers);
            break;
        }
    }
}
}  // namespace

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FJumpLatentCommand, ACharacter*, Character);

bool FJumpLatentCommand::Update()
{
    if (!Character)
    {
        return true;
    }
    ExecuteInputAction(Character, "IA_Jump", 1.0f);
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

bool FAllItemsCanBeTakenOnMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(TEXT("/Game/AutomationSample/Tests/InventoryTestLevel3"));

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
    if (!TestEqual("Inventory items count", InventoryItems.Num(), 7))
    {
        return false;
    }
    UInputModifierSwizzleAxis* SwizzleAxisModifier = NewObject<UInputModifierSwizzleAxis>();
    SwizzleAxisModifier->Order = EInputAxisSwizzle::YXZ;

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSUntilLatentCommand([this, Character, SwizzleAxisModifier]()
        { ExecuteInputAction(Character, "IA_Move", 1.0f, {SwizzleAxisModifier}); },
        []() {}, 2.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FJumpLatentCommand(Character));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(
        FTPSUntilLatentCommand([this, Character]() { ExecuteInputAction(Character, "IA_Move", 1.0f); }, []() {}, 2.5f));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [this, Character]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(Character, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        },
        1.0f));

    return true;
}

class FSimulateMovementLatentCommand : public IAutomationLatentCommand
{
public:
    FSimulateMovementLatentCommand(UWorld* InWorld, UInputComponent* InInputComponent, const TArray<FBindingsData>& InBindings)
        : World(InWorld), InputComponent(InInputComponent), Bindings(InBindings)
    {
    }

    virtual bool Update() override
    {
        if (!World || !InputComponent)
        {
            return true;
        }

        if (WorldStartTime == 0.0f)
        {
            WorldStartTime = World->GetTimeSeconds();
        }

        while (World->TimeSeconds - WorldStartTime >= Bindings[Index].WorldTime)
        {
            const auto& AxisValues = Bindings[Index].AxisValues;
            for (const auto& AxisValue : AxisValues)
            {
                ExecuteVectorInputAction(Cast<ACharacter>(InputComponent->GetOwner()), AxisValue.Name.ToString(), AxisValue.Value);
            }
            if (++Index >= Bindings.Num())
            {
                return true;
            }
        }

        return false;
    }

private:
    const UWorld* World;
    const UInputComponent* InputComponent;
    const TArray<FBindingsData> Bindings;
    int32 Index{0};
    float WorldStartTime{0.0f};
};
bool FAllItemsCanBeTakenOnRecordedMovement::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope(TEXT("/Game/AutomationSample/Tests/InventoryTestLevel4"));

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
    if (!TestEqual("Inventory items count", InventoryItems.Num(), 7))
    {
        return false;
    }

    const FString InputDataFileName = GetTestDataDir().Append("CharacterTestInput.json");
    FInputData InputData;
    if (!JsonUtils::ReadInputData(InputDataFileName, InputData))
    {
        return false;
    }

    if (!TestTrue("Input data is not empty", InputData.Bindings.Num() > 0))
    {
        return false;
    }

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
    if (!TestNotNull("Player controller could be created", PlayerController))
    {
        return false;
    }

    Character->SetActorTransform(InputData.InitialTransform);
    PlayerController->SetControlRotation(InputData.InitialTransform.GetRotation().Rotator());

    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FSimulateMovementLatentCommand(World, Character->InputComponent, InputData.Bindings));
    ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [this, Character]()
        {
            TArray<AActor*> InventoryItems;
            UGameplayStatics::GetAllActorsOfClass(Character, ATPSInventoryItem::StaticClass(), InventoryItems);
            TestTrueExpr(InventoryItems.Num() == 0);
            return true;
        }));

    return true;
}
#endif
