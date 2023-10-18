// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AUTOMATIONSAMPLE_API ATPSProjectile : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATPSProjectile();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

public:
    void SetShotDirection(const FVector& Direction) { ShotDirection = Direction; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float DamageAmount{30.0f};

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    float LifeSeconds{5.0f};

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

private:
    FVector ShotDirection;
};
