// My game copyright

#if WITH_AUTOMATION_TESTS

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "TestUtils.h"
#include "Weapon/TPSTurret.h"

BEGIN_DEFINE_SPEC(FTurret, "AutomationSample.Turret",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

UWorld* World;
const FTransform InitialTransform{FVector(0.0f, 200.0f, 0.0f)};
ATPSTurret* Turret;

END_DEFINE_SPEC(FTurret)

using namespace TPS::Test;

namespace
{
constexpr const char* MapName = "Game/AutomationSample/Tests/EmptyTestLevel";
constexpr const char* TurretBPTestName = "/Script/Engine.Blueprint'/Game/AutomationSample/Tests/BP_Turret_Test.BP_Turret_Test'";

void SpecCloseLevel(const UWorld* World)
{
    if (APlayerController* PC = World->GetFirstPlayerController())
    {
        PC->ConsoleCommand("Quit", false);
    }
}

template <class ObjectClass, class PropertyClass>
PropertyClass GetPropertyValueByName(ObjectClass* Object, const FString& PropertyName)
{
    if (!Object)
    {
        return PropertyClass{};
    }
    for (TFieldIterator<FProperty> It(Object->GetClass()); It; ++It)
    {
        if (It->GetName() == PropertyName)
        {
            return *It->template ContainerPtrToValuePtr<PropertyClass>(Object);
        }
    }
    return PropertyClass{};
}

}  // namespace

void FTurret::Define()
{
    Describe("Creation",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = AutomationCommon::GetAnyGameWorld();
                    TestNotNull("World", World);
                });
            It("Cpp instance can't be created",
                [this]()
                {
                    const FString ExpectedError =
                        FString::Printf(TEXT("SpawnActor failed because class %s is abstract"), *ATPSTurret::StaticClass()->GetName());
                    AddExpectedError(ExpectedError, EAutomationExpectedErrorFlags::Exact);
                    const ATPSTurret* Turret = World->SpawnActor<ATPSTurret>(ATPSTurret::StaticClass(), InitialTransform);
                    TestNull("Turret doesn't exist", Turret);
                });
            It("Blueprint instance can be created",
                [this]()
                {
                    const ATPSTurret* Turret = CreateBlueprint<ATPSTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });
            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Defaults",
        [this]()
        {
            BeforeEach(
                [this]()
                {
                    AutomationOpenMap(MapName);
                    World = AutomationCommon::GetAnyGameWorld();
                    TestNotNull("World", World);
                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                });

            const TArray<TTuple<int32, float>> TestData{{45, 2.0f}, {100, 1.0f}, {200, 0.5f}};
            for (const auto& Data : TestData)
            {
                const auto TestName = FString::Printf(TEXT("AmmoCount should be %d, FireFrequency should be %f"), Data.Key, Data.Value);
                It(TestName,
                    [this, Data]()
                    {
                        const auto [AmmoCount, FireFrequency] = Data;
                        CallFuncByNameWithParams(
                            Turret, "SetTurretData", {FString::FromInt(AmmoCount), FString::SanitizeFloat(FireFrequency)});
                        const int32 AmmoCountFromBP = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                        TestTrueExpr(AmmoCountFromBP == AmmoCount);
                        const float FireFrequencyFromBP = GetPropertyValueByName<ATPSTurret, float>(Turret, "FireFrequency");
                        TestTrueExpr(FireFrequencyFromBP == FireFrequency);
                    });
            }

            AfterEach([this]() { SpecCloseLevel(World); });
        });

    Describe("Ammo",
        [this]()
        {
            const int32 InitialAmmoCount = 3;
            const float FireFrequency = 1.0f;

            BeforeEach(
                [this, InitialAmmoCount, FireFrequency]()
                {
                    AutomationOpenMap(MapName);
                    World = AutomationCommon::GetAnyGameWorld();
                    TestNotNull("World", World);

                    Turret = CreateBlueprint<ATPSTurret>(World, TurretBPTestName, InitialTransform);
                    TestNotNull("Turret exists", Turret);
                    CallFuncByNameWithParams(
                        Turret, "SetTurretData", {FString::FromInt(InitialAmmoCount), FString::SanitizeFloat(FireFrequency)});
                });

            const FString TestName =
                FString::Printf(TEXT("AmmoCount should be 0 after %i sec"), FMath::RoundToInt(InitialAmmoCount * FireFrequency));
            LatentIt(TestName, EAsyncExecution::ThreadPool,
                [this, InitialAmmoCount, FireFrequency](const FDoneDelegate& TestDone)
                {
                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCountFromBP = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCountFromBP == InitialAmmoCount);

                            const FTimerHandle FireTimerHandle =
                                GetPropertyValueByName<ATPSTurret, FTimerHandle>(Turret, "FireTimerHandle");
                            TestTrueExpr(World->GetTimerManager().IsTimerActive(FireTimerHandle));
                        });

                    const float WaitTime = InitialAmmoCount * FireFrequency + 3.0f;
                    FPlatformProcess::Sleep(WaitTime);

                    AsyncTask(ENamedThreads::GameThread,
                        [&]()
                        {
                            const int32 AmmoCountFromBP = GetPropertyValueByName<ATPSTurret, int32>(Turret, "AmmoCount");
                            TestTrueExpr(AmmoCountFromBP == 0);

                            const FTimerHandle FireTimerHandle =
                                GetPropertyValueByName<ATPSTurret, FTimerHandle>(Turret, "FireTimerHandle");
                            TestTrueExpr(!World->GetTimerManager().IsTimerActive(FireTimerHandle));
                        });

                    TestDone.Execute();
                });
            AfterEach([this]() { SpecCloseLevel(World); });
        });
}

#endif
