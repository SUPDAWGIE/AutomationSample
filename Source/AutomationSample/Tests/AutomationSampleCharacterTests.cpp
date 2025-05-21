// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/AutomationSampleCharacterTests.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/DamageEvents.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/TestUtils.h"
#include "AutomationSampleCharacter.h"
#include "CoreMinimal.h"

DEFINE_LOG_CATEGORY_STATIC(LogTPSCharacterTests, All, All);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthMightBeChangedWithDamage, "AutomationSample.Character.HealthMightBeChangedWithDamage",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleWait, "AutomationSample.Character.LatentCommandSimpleWait",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandSimpleLog, "AutomationSample.Character.LatentCommandSimpleLog",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLatentCommandOpenCloseMap, "AutomationSample.Character.LatentCommandOpenCloseMap",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FCharacterCanBeKilled, "AutomationSample.Character.CharacterCanBeKilled",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FAutoHealShouldRestoreHealth, "AutomationSample.Character.AutoHealShouldRestoreHealth",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

namespace
{
constexpr const char* CharacterBlueprintTestPath =
    "/Script/Engine.Blueprint'/Game/AutomationSample/Tests/BP_Test_ThirdPersonCharacter.BP_Test_ThirdPersonCharacter'";
}

using namespace TPS::Test;

bool FHealthMightBeChangedWithDamage::RunTest(const FString& Parameters)
{
    LevelScope Level(TEXT("/Game/AutomationSample/Tests/EmptyTestLevel"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector(0.0f, -240.0f, 110.0f)};
    AAutomationSampleCharacter* Character =
        CreateBlueprintDeferred<AAutomationSampleCharacter>(World, CharacterBlueprintTestPath, InitialTransform);
    if (!TestNotNull("Character could be created", Character))
    {
        return false;
    }

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;

    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});
    Character->FinishSpawning(InitialTransform);

    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    const float DamageAmount = 10.f;
    Character->TakeDamage(DamageAmount, FDamageEvent(), nullptr, nullptr);
    TestEqual("Health is damaged", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    return true;
}

bool FLatentCommandSimpleWait::RunTest(const FString& Parameters)
{
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));

    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FTPSLatentLogCommand, FString, LogMessage);

bool FTPSLatentLogCommand::Update()
{
    UE_LOG(LogTPSCharacterTests, Log, TEXT("%s"), *LogMessage);
    return true;
}

bool FLatentCommandSimpleLog::RunTest(const FString& Parameters)
{
    UE_LOG(LogTPSCharacterTests, Log, TEXT("Log 1"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSLatentLogCommand(TEXT("Latent Log 1")));
    UE_LOG(LogTPSCharacterTests, Log, TEXT("Log 2"));
    ADD_LATENT_AUTOMATION_COMMAND(FTPSLatentLogCommand(TEXT("Latent Log 2")));
    UE_LOG(LogTPSCharacterTests, Log, TEXT("Log 3"));

    return true;
}

bool FLatentCommandOpenCloseMap::RunTest(const FString& Parameters)
{
    const LevelScope Level = LevelScope(TEXT("/Game/AutomationSample/Tests/EmptyTestLevel"));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(3.0f));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterDestroyedLatentCommand, AAutomationSampleCharacter*, Character, float, LifeSpan);
bool FCharacterDestroyedLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime > LifeSpan)
    {
        if (IsValid(Character))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Character is not destroyed"));
        }
        return true;
    }
    return false;
}

bool FCharacterCanBeKilled::RunTest(const FString& Parameters)
{
    const LevelScope Level = LevelScope(TEXT("/Game/AutomationSample/Tests/EmptyTestLevel"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector(0.0f, -240.0f, 110.0f)};
    AAutomationSampleCharacter* Character =
        CreateBlueprintDeferred<AAutomationSampleCharacter>(World, CharacterBlueprintTestPath, InitialTransform);
    if (!TestNotNull("Character could be created", Character))
    {
        return false;
    }

    FHealthData HealthData;
    HealthData.MaxHealth = 1000.0f;
    HealthData.LifeSpan = 1.5f;

    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});
    Character->FinishSpawning(InitialTransform);

    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    const float DamageAmount = 10.f;
    Character->TakeDamage(DamageAmount, FDamageEvent(), nullptr, nullptr);
    TestEqual("Health is damaged", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    const float KillingDamageAmount = HealthData.MaxHealth;
    Character->TakeDamage(KillingDamageAmount, FDamageEvent(), nullptr, nullptr);
    TestEqual("Character is dead", Character->GetHealthPercent(), 0.0f);

    TestTrueExpr(Character->GetCharacterMovement()->MovementMode == EMovementMode::MOVE_None);
    ENUM_LOOP_START(ECollisionChannel, EElement)
    if (EElement != ECollisionChannel::ECC_OverlapAll_Deprecated)
    {
        TestTrueExpr(Character->GetCapsuleComponent()->GetCollisionResponseToChannel(EElement) == ECollisionResponse::ECR_Ignore);
    }
    ENUM_LOOP_END

    TestTrueExpr(Character->GetMesh()->GetCollisionEnabled() == ECollisionEnabled::QueryAndPhysics);
    TestTrueExpr(Character->GetMesh()->IsSimulatingPhysics());
    TestTrueExpr(FMath::IsNearlyEqual(Character->GetLifeSpan(), HealthData.LifeSpan));

    // ADD_LATENT_AUTOMATION_COMMAND(FCharacterDestroyedLatentCommand(Character, HealthData.LifeSpan));
    // ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([Character]() { return !IsValid(Character); }, HealthData.LifeSpan));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]()
        {
            if (IsValid(Character))
            {
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Character is not destroyed"));
            }
        },
        HealthData.LifeSpan));
    return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FCharacterHealedLatentCommand, AAutomationSampleCharacter*, Character, float, LifeSpan);
bool FCharacterHealedLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime > LifeSpan)
    {
        if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
        {
            UE_LOG(LogTPSCharacterTests, Error, TEXT("Character is not healed"));
        }
        return true;
    }
    return false;
}

bool FAutoHealShouldRestoreHealth::RunTest(const FString& Parameters)
{
    const LevelScope Level = LevelScope(TEXT("/Game/AutomationSample/Tests/EmptyTestLevel"));

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World could be created", World))
    {
        return false;
    }

    const FTransform InitialTransform{FVector(0.0f, -240.0f, 110.0f)};
    AAutomationSampleCharacter* Character =
        CreateBlueprintDeferred<AAutomationSampleCharacter>(World, CharacterBlueprintTestPath, InitialTransform);
    if (!TestNotNull("Character could be created", Character))
    {
        return false;
    }

    FHealthData HealthData;
    HealthData.MaxHealth = 200.0f;
    HealthData.HealModifier = 5.0f;
    HealthData.HealRate = .5f;

    CallFuncByNameWithParams(Character, "SetHealthData", {HealthData.ToString()});
    Character->FinishSpawning(InitialTransform);

    TestEqual("Health is full", Character->GetHealthPercent(), 1.0f);
    const float DamageAmount = 20.f;
    Character->TakeDamage(DamageAmount, FDamageEvent(), nullptr, nullptr);
    TestEqual("Health is damaged", Character->GetHealthPercent(), 1.0f - DamageAmount / HealthData.MaxHealth);

    const float HealthDiff = HealthData.MaxHealth * (1.0f - Character->GetHealthPercent());
    const float HealingDuration = HealthData.HealRate * HealthDiff / HealthData.HealModifier + 1.0f;
    // ADD_LATENT_AUTOMATION_COMMAND(FCharacterHealedLatentCommand(Character, HealingDuration));
    ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(
        [Character]()
        {
            if (!FMath::IsNearlyEqual(Character->GetHealthPercent(), 1.0f))
            {
                UE_LOG(LogTPSCharacterTests, Error, TEXT("Character is not healed"));
            }
        },
        HealingDuration));
    return true;
}

#endif
