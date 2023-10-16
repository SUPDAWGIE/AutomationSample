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

}  // namespace Test
}  // namespace TPS

#endif