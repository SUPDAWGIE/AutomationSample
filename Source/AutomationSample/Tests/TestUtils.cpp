#if WITH_AUTOMATION_TESTS

#include "AutomationSample/Tests/TestUtils.h"
#include "Misc/OutputDeviceNull.h"

namespace TPS
{
namespace Test
{

FTPSUntilCommand::FTPSUntilCommand(TFunction<void()> InCallback, TFunction<void()> InTimeoutCallback, float InTimeout)
    : Callback(MoveTemp(InCallback)), TimeoutCallback(MoveTemp(InTimeoutCallback)), Timeout(InTimeout)
{
}

bool FTPSUntilCommand::Update()
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
}  // namespace Test
}  // namespace TPS

#endif