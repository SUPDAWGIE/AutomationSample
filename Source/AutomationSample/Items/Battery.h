#pragma once

#include "CoreMinimal.h"

namespace TPS
{
class Battery
{
public:
    Battery() = default;
    Battery(float InPercent);

    void Charge();
    void Discharge();

    float GetPercent() const;
    FColor GetColor() const;
    FString ToString() const;

private:
    float Percent{1.0f};
    void SetPercent(float InPercent);
};
}  // namespace TPS
