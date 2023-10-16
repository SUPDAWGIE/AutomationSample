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

UCLASS()
class USupLocalizationFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static FText GetInventoryItemText(EInventoryItemType Type)
    {
        switch (Type)
        {
            case EInventoryItemType::SPHERE: return NSLOCTEXT("Inventory", "Sphere_Loc", "SPHERE");
            case EInventoryItemType::CUBE: return NSLOCTEXT("Inventory", "Cube_Loc", "CUBE");
            case EInventoryItemType::CYLINDER: return NSLOCTEXT("Inventory", "Cylinder_Loc", "CYLINDER");
            case EInventoryItemType::CONE: return NSLOCTEXT("Inventory", "Cone_Loc", "CONE");
        }

        checkNoEntry();
        return FText::GetEmpty();
    }
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

USTRUCT(BlueprintType)
struct FHealthData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0"))
    float MaxHealth = {100.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1"))
    float HealModifier{10.f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "seconds"))
    float HealRate{0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.1", Units = "seconds"))
    float LifeSpan{5.0f};

    FString ToString() const
    {
        return FString::Printf(TEXT("(MaxHealth=%f,HealModifier=%f,HealRate=%f,LifeSpan=%f)"), MaxHealth, HealModifier, HealRate, LifeSpan);
    }
};
