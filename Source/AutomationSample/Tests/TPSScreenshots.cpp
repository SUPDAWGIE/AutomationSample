// My game copyright

#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/TPSScreenshots.h"
#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Camera/CameraActor.h"
#include "Engine/DamageEvents.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRenderingShouldBeCorrect, "AutomationSample.Screenshots.RenderingShouldBeCorrect",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority |
        EAutomationTestFlags::NonNullRHI);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMainPlayerHudShouldBeRendered, "AutomationSample.Screenshots.MainPlayerHudShouldBeRendered",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority |
        EAutomationTestFlags::NonNullRHI);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHealthWidgetShouldBeRenderedCorrectrlAfterDamage,
    "AutomationSample.Screenshots.HealthWidgetShouldBeRenderedCorrectrlAfterDamage",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority |
        EAutomationTestFlags::NonNullRHI);

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FSettingsWidgetShouldBeRendedOnPause, "AutomationSample.Screenshots.SettingsWidgetShouldBeRendedOnPause",
    EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority |
        EAutomationTestFlags::NonNullRHI);

using namespace TPS::Test;

bool FRenderingShouldBeCorrect::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/ScreenshotTestLevel");

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World", World))
    {
        return false;
    }

    const FTransform Transform{FVector(-20.0f, 0.0f, 250.0f)};
    ACameraActor* Camera = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Transform);
    if (!TestNotNull("Camera", Camera))
    {
        return false;
    }

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC", PC))
    {
        return false;
    }

    PC->SetViewTarget(Camera);

    SWindow* Window = PC->GetLocalPlayer()->ViewportClient->GetWindow().Get();
    if (!TestNotNull("Window", Window))
    {
        return false;
    }
    Window->Resize(FVector2D(1280.0f, 720.0f));

    ADD_LATENT_AUTOMATION_COMMAND(FTakeGameScreenshotLatentCommand("rendering_check_screenshot"));
    return true;
}

bool FMainPlayerHudShouldBeRendered::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/EmptyTestLevel");

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World", World))
    {
        return false;
    }

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC", PC))
    {
        return false;
    }

    SWindow* Window = PC->GetLocalPlayer()->ViewportClient->GetWindow().Get();
    if (!TestNotNull("Window", Window))
    {
        return false;
    }
    Window->Resize(FVector2D(1280.0f, 720.0f));

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("ui_check_screenshot"));
    return true;
}

bool FHealthWidgetShouldBeRenderedCorrectrlAfterDamage::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/EmptyTestLevel");

    UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World", World))
    {
        return false;
    }

    APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC", PC))
    {
        return false;
    }

    SWindow* Window = PC->GetLocalPlayer()->ViewportClient->GetWindow().Get();
    if (!TestNotNull("Window", Window))
    {
        return false;
    }
    Window->Resize(FVector2D(1280.0f, 720.0f));

    APawn* Pawn = PC->GetPawn();
    if (!TestNotNull("Pawn", Pawn))
    {
        return false;
    }

    const float DamageAmount = 40.0f;
    Pawn->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("health_widget_screenshot"));
    return true;
}

bool FSettingsWidgetShouldBeRendedOnPause::RunTest(const FString& Parameters)
{
    const auto Level = LevelScope("/Game/AutomationSample/Tests/EmptyTestLevel");

    const UWorld* World = AutomationCommon::GetAnyGameWorld();
    if (!TestNotNull("World", World))
    {
        return false;
    }

    const APlayerController* PC = World->GetFirstPlayerController();
    if (!TestNotNull("PC", PC))
    {
        return false;
    }

    SWindow* Window = PC->GetLocalPlayer()->ViewportClient->GetWindow().Get();
    if (!TestNotNull("Window", Window))
    {
        return false;
    }
    Window->Resize(FVector2D(1280.0f, 720.0f));

    ExecuteInputAction(PC, "IA_Pause", 1.0f);

    ADD_LATENT_AUTOMATION_COMMAND(FTakeUIScreenshotLatentCommand("settings_widget_screenshot"));
    return true;
}

#endif
