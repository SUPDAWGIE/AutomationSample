// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/TPSPlayerControllerTests.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameCanBeUnpaused, "AutomationSample.PlayerController.GameCanBeUnpaused",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

using namespace TPS::Test;

namespace
{
}

bool FGameCanBeUnpaused::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/EmptyTestLevel");

    const APlayerController* PC = AutomationCommon::GetAnyGameWorld()->GetFirstPlayerController();
    TestTrueExpr(PC != nullptr);
    TestTrueExpr(!PC->IsPaused());
    TestTrueExpr(!PC->bShowMouseCursor);
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&, PC]()
        {
            ExecuteInputAction(PC, "IA_Pause", 1.0f);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&, PC]()
        {
            TestTrueExpr(PC->IsPaused());
            TestTrueExpr(PC->bShowMouseCursor);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&, PC]()
        {
            ExecuteInputAction(PC, "IA_Pause", 1.0f);
            return true;
        }));
    ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0f));
    ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(
        [&, PC]()
        {
            TestTrueExpr(!PC->IsPaused());
            TestTrueExpr(!PC->bShowMouseCursor);
            return true;
        }));
    return true;
}

#endif
