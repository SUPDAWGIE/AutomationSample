#include "Battery.h"

using namespace TPS;
constexpr float ChargeAmount = 0.1f;

TPS::Battery::Battery(float InPercent)
{
    SetPercent(InPercent);
}

void TPS::Battery::Charge()
{
    SetPercent(Percent + ChargeAmount);
}

void TPS::Battery::Discharge()
{
    SetPercent(Percent - ChargeAmount);
}

float TPS::Battery::GetPercent() const
{
    return Percent;
}

FColor TPS::Battery::GetColor() const
{
    if (Percent > 0.8f)
    {
        return FColor::Green;
    }
    else if (Percent > 0.3f)
    {
        return FColor::Yellow;
    }
    else
    {
        return FColor::Red;
    }
}

FString TPS::Battery::ToString() const
{
    return FString::Printf(TEXT("%u%%"), FMath::RoundToInt(Percent * 100.0f));
}

void TPS::Battery::SetPercent(float InPercent)
{
    Percent = FMath::Clamp(InPercent, 0.0f, 1.0f);
}
