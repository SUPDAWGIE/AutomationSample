#include "AutomationBlueprintFunctionLibrary.h"
#include "AutomationScreenshotOptions.h"
#include "BufferVisualizationData.h"
#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/TestUtils.h"
#include "Misc/OutputDeviceNull.h"
#include "EnhancedInputComponent.h"
#include "EnhancedPlayerInput.h"
#include "InputModifiers.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"

namespace TPS
{
namespace Test
{

FTPSUntilLatentCommand::FTPSUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout)
    : Callback(MoveTemp(InCallback)), TimeoutCallback(MoveTemp(InTimeoutCallback)), Timeout(InTimeout)
{
}

bool FTPSUntilLatentCommand::Update()
{
    const double NewTime = FPlatformTime::Seconds();
    if (NewTime - StartTime >= Timeout)
    {
        TimeoutCallback();
        return true;
    }

    Callback();
    return false;
}

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params)
{
    if (!Object)
    {
        return;
    }
    FString Command = FString::Printf(TEXT("%s"), *FuncName);
    for (const auto& Param : Params)
    {
        Command += FString::Printf(TEXT(" %s"), *Param);
    }
    FOutputDeviceNull NullOutputDevice;
    Object->CallFunctionByNameWithArguments(*Command, NullOutputDevice, nullptr, true);
}

FString GetTestDataDir()
{
    return FPaths::GameSourceDir().Append("AutomationSample/Tests/Data/");
}

UWidget* FindWidgetByName(const UUserWidget* ParentWidget, const FName& Name)
{
    if (!ParentWidget || !ParentWidget->WidgetTree)
    {
        return nullptr;
    }
    TArray<UWidget*> Widgets;
    ParentWidget->WidgetTree->GetAllWidgets(Widgets);
    for (const auto& WidgetItem : Widgets)
    {
        if (WidgetItem->GetFName() == Name)
        {
            return WidgetItem;
        }
    }
    return nullptr;
}

void ExecuteInputAction(const APlayerController* PC, const FString& Command, const float& Value, const TArray<UInputModifier*>& Modifiers)
{
    if (!PC)
    {
        return;
    }
    const UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent);
    if (!EnhancedInputComponent)
    {
        return;
    }

    UEnhancedPlayerInput* EnhancedInput = Cast<UEnhancedPlayerInput>(PC->PlayerInput);
    if (!EnhancedInput)
    {
        return;
    }

    for (auto& ActionEventBinding : EnhancedInputComponent->GetActionEventBindings())
    {
        if (ActionEventBinding->GetAction()->GetName() == Command)
        {
            EnhancedInput->InjectInputForAction(ActionEventBinding->GetAction(), Value, Modifiers);
            break;
        }
    }
}

FTakeScreenshotLatentCommand::FTakeScreenshotLatentCommand(const FString& InScreenshotName) : ScreenshotName(InScreenshotName)
{
    FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.AddRaw(this, &FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared);
}

FTakeScreenshotLatentCommand::~FTakeScreenshotLatentCommand()
{
    FAutomationTestFramework::Get().OnScreenshotTakenAndCompared.RemoveAll(this);
}

void FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared()
{
    bScreenshotTaken = true;
}

FTakeGameScreenshotLatentCommand::FTakeGameScreenshotLatentCommand(const FString& InScreenshotName)
    : FTakeScreenshotLatentCommand(InScreenshotName)
{
}

bool FTakeGameScreenshotLatentCommand::Update()
{
    if (!bScreenshotRequested)
    {
        const FAutomationScreenshotOptions Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
        UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotInternal(
            AutomationCommon::GetAnyGameWorld(), ScreenshotName, FString{}, Options);
        bScreenshotRequested = true;
    }
    return bScreenshotTaken;
}

FTakeUIScreenshotLatentCommand::FTakeUIScreenshotLatentCommand(const FString& InScreenshotName)
    : FTakeScreenshotLatentCommand(InScreenshotName)
{
}

bool FTakeUIScreenshotLatentCommand::Update()
{
    if (!bScreenshotSetupDone)
    {
        bScreenshotSetupDone = true;
        SetBufferVisualization("Opacity");
        return false;
    }
    if (!bScreenshotRequested)
    {
        const FAutomationScreenshotOptions Options = UAutomationBlueprintFunctionLibrary::GetDefaultScreenshotOptionsForRendering();
        UAutomationBlueprintFunctionLibrary::TakeAutomationScreenshotOfUI_Immediate(
            AutomationCommon::GetAnyGameWorld(), ScreenshotName, Options);
        bScreenshotRequested = true;
    }
    return bScreenshotTaken;
}

void FTakeUIScreenshotLatentCommand::OnScreenshotTakenAndCompared()
{
    FTakeScreenshotLatentCommand::OnScreenshotTakenAndCompared();
    SetBufferVisualization(NAME_None);
}

void FTakeUIScreenshotLatentCommand::SetBufferVisualization(const FName& VisualizeBuffer)
{
    if (UGameViewportClient* ViewportClient = AutomationCommon::GetAnyGameViewportClient())
    {
        static IConsoleVariable* ICVar =
            IConsoleManager::Get().FindConsoleVariable(FBufferVisualizationData::GetVisualizationTargetConsoleCommandName());
        if (ICVar)
        {
            if (ViewportClient->GetEngineShowFlags())
            {
                ViewportClient->GetEngineShowFlags()->SetVisualizeBuffer(!VisualizeBuffer.IsNone());
                ViewportClient->GetEngineShowFlags()->SetTonemapper(VisualizeBuffer.IsNone());
                ICVar->Set(VisualizeBuffer.IsNone() ? TEXT("") : *VisualizeBuffer.ToString());
            }
        }
    }
}

}  // namespace Test
}  // namespace TPS

#endif