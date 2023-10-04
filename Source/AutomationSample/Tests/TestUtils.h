#pragma once
#include "Tests/AutomationCommon.h"

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"

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

template <typename EnumType, typename FunctionType>
void ForEach(FunctionType&& Function)
{
    const UEnum* Enum = StaticEnum<EnumType>();
    for (int32 i = 0; i < Enum->NumEnums(); ++i)
        Function(static_cast<EnumType>(Enum->GetValueByIndex(i)), Enum->GetNameByIndex(i));
}

#define ENUM_LOOP_START(TYPE, EnumElem)                                        \
    for (int32 index = 0; index < StaticEnum<TYPE>()->NumEnums() - 1; ++index) \
    {                                                                          \
        const auto EnumElem = static_cast<TYPE>(index);
#define ENUM_LOOP_END }

template <typename T>
T* CreateBlueprint(UWorld* World, const FString& Name, const FTransform Transform = FTransform::Identity)
{
    const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *Name);
    return (World && Blueprint) ? World->SpawnActor<T>(Blueprint->GeneratedClass, Transform) : nullptr;
}

class LevelScope
{
public:
    LevelScope(const FString& LevelName) { AutomationOpenMap(LevelName); }
    ~LevelScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

void CallFuncByNameWithParams(UObject* Object, const FString& FuncName, const TArray<FString>& Params);
}  // namespace Test
}  // namespace TPS
#endif