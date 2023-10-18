// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSTurret.h"

#include "TPSProjectile.h"

// Sets default values
ATPSTurret::ATPSTurret()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
    check(TurretMesh);
    SetRootComponent(TurretMesh);
}

// Called when the game starts or when spawned
void ATPSTurret::BeginPlay()
{
    Super::BeginPlay();

    check(AmmoCount > 0);
    check(FireFrequency > 0.0f);

    const float FirstDelay = FireFrequency;
    GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATPSTurret::OnFire, FireFrequency, true, FirstDelay);
}

// Called every frame
void ATPSTurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATPSTurret::OnFire()
{
    if (--AmmoCount == 0)
    {
        GetWorldTimerManager().ClearTimer(FireTimerHandle);
    }

    if (!GetWorld())
    {
        return;
    }

    const FTransform SocketTransform = TurretMesh->GetSocketTransform(TEXT("MuzzleSocket"));
    if (ATPSProjectile* ProjectileObj = GetWorld()->SpawnActorDeferred<ATPSProjectile>(ProjectileClass, SocketTransform))
    {
        ProjectileObj->SetShotDirection(SocketTransform.GetRotation().GetForwardVector());
        ProjectileObj->FinishSpawning(SocketTransform);
    }
}
