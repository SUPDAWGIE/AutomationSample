// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSProjectile.h"

#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATPSProjectile::ATPSProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    check(CollisionComponent);
    CollisionComponent->InitSphereRadius(10.0f);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SetRootComponent(CollisionComponent);

    MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    check(MovementComponent);
    MovementComponent->InitialSpeed = 2000.0f;
    MovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void ATPSProjectile::BeginPlay()
{
    Super::BeginPlay();

    MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
    CollisionComponent->OnComponentHit.AddDynamic(this, &ATPSProjectile::OnHit);
    SetLifeSpan(LifeSeconds);
}

// Called every frame
void ATPSProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATPSProjectile::OnHit(
    UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (GetWorld() && OtherActor)
    {
        MovementComponent->StopMovementImmediately();
        OtherActor->TakeDamage(DamageAmount, FDamageEvent(), nullptr, this);
        Destroy();
    }
}
