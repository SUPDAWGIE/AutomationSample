#pragma once

#include "CoreMinimal.h"
#include "TPSTypes.generated.h"

UENUM(BlueprintType)
enum class EInventoryItemType : uint8
{
    SPHERE = 0 UMETA(DisplayName = "MY COOL SPHERE"),
    CUBE UMETA(DisplayName = "Cube"),
    CYLINDER,
    CONE,
};

USTRUCT(BlueprintType)
struct FInventoryData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInventoryItemType Type = EInventoryItemType::SPHERE;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
    int32 Score = 0;

    FString ToString() const { return FString::Printf(TEXT("(Type=%i,Score=%i)"), Type, Score); }
};
