#pragma once

#include "CoreMinimal.h"

namespace TPS
{
class Battery
{
public:
    Battery() = default;
    Battery(float InPercent);

    //! \todo Add parameter for charge
    void Charge();

    //! \todo Add parameter for discharge
    void Discharge();

    float GetPercent() const;
    FColor GetColor() const;
    FString ToString() const;

    bool operator>=(const Battery& rhs) const { return GetPercent() >= rhs.GetPercent(); }

    bool operator==(const Battery& Battery) const { return FMath::IsNearlyEqual(GetPercent(), Battery.GetPercent()); }

    bool operator<(const Battery& Battery) const { return !(*this >= Battery); }

private:
    float Percent{1.0f};
    void SetPercent(float InPercent);
};
}  // namespace TPS
