#include "Misc/OutputDeviceNull.h"
#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/TestUtils.h"

namespace TPS
{
namespace Test
{

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
}  // namespace Test
}  // namespace TPS

#endif