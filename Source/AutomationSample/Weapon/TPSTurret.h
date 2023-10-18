// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSTurret.generated.h"

class ATPSProjectile;

UCLASS(Abstract)
class AUTOMATIONSAMPLE_API ATPSTurret : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATPSTurret();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* TurretMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    int32 AmmoCount{10};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Units = s))
    float FireFrequency{1.5f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TSubclassOf<ATPSProjectile> ProjectileClass;

private:
    UPROPERTY()
    FTimerHandle FireTimerHandle;

    void OnFire();
};
