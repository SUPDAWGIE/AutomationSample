#pragma once
#include "Blueprint/WidgetBlueprintLibrary.h"

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Tests/AutomationCommon.h"

class UWidget;
class UInputModifier;

namespace TPS
{
namespace Test
{
template <typename Type1, typename Type2>
struct TestPayload
{
    Type1 TestValue;
    Type2 ExpectedValue;
    float Tolerance = KINDA_SMALL_NUMBER;
};

#define ENUM_LOOP_START(TYPE, EnumElem)                                        \
    for (int32 index = 0; index < StaticEnum<TYPE>()->NumEnums() - 1; ++index) \
    {                                                                          \
        const auto EnumElem = static_cast<TYPE>(StaticEnum<TYPE>()->GetValueByIndex(index));
#define ENUM_LOOP_END }

template <typename EnumType, typename FunctionType>
void ForEach(FunctionType&& Function)
{
    const UEnum* Enum = StaticEnum<EnumType>();
    for (int32 i = 0; i < Enum->NumEnums(); ++i)
    {
        Function(static_cast<EnumType>(Enum->GetValueByIndex(i)));
    }
}

template <typename T>
T* CreateBlueprint(UWorld* World, const FString& Name, const FTransform Transform = FTransform::Identity)
{
    const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

template <typename T>
T* CreateBlueprintDeferred(UWorld* World, const FString& Name, const FTransform Transform = FTransform::Identity)
{
    const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && Blueprint) ? World->SpawnActorDeferred<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

class LevelScope
{
public:
    LevelScope(const FString& LevelName) { AutomationOpenMap(LevelName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

class FTPSUntilLatentCommand : public IAutomationLatentCommand
{
public:
    FTPSUntilLatentCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout = 5.0f);

    virtual bool Update() override;

private:
    TFunction<void()> Callback;
    TFunction<void()> TimeoutCallback;
    float Timeout;
};

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);

FString GetTestDataDir();

UWidget* FindWidgetByName(const UUserWidget* ParentWidget, const FName& Name);

template <class T>
T* FindWidgetByClass()
{
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(AutomationCommon::GetAnyGameWorld(), Widgets, T::StaticClass(), false);
    return Widgets.Num() != 0 ? Cast<T>(Widgets[0]) : nullptr;
}

void ExecuteInputAction(
    const APlayerController* PC, const FString& Command, const float& Value, const TArray<UInputModifier*>& Modifiers = {});

class FTakeScreenshotLatentCommand : public IAutomationLatentCommand
{
public:
    FTakeScreenshotLatentCommand(const FString& InScreenshotName);

    virtual ~FTakeScreenshotLatentCommand() override;

protected:
    const FString ScreenshotName;
    bool bScreenshotRequested{false};
    bool bScreenshotTaken{false};

    virtual void OnScreenshotTakenAndCompared();
};

class FTakeGameScreenshotLatentCommand : public FTakeScreenshotLatentCommand
{
public:
    FTakeGameScreenshotLatentCommand(const FString& InScreenshotName);

    virtual bool Update() override;
};

class FTakeUIScreenshotLatentCommand : public FTakeScreenshotLatentCommand
{
public:
    FTakeUIScreenshotLatentCommand(const FString& InScreenshotName);

    virtual bool Update() override;

private:
    bool bScreenshotSetupDone{false};

    virtual void OnScreenshotTakenAndCompared() override;

    void SetBufferVisualization(const FName& VisualizeBuffer);
};

}  // namespace Test
}  // namespace TPS
#endif