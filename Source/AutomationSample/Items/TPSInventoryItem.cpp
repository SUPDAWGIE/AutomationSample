// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSInventoryItem.h"
#include "Components/SphereComponent.h"
#include "Components/TPSInventoryActorComponent.h"

ATPSInventoryItem::ATPSInventoryItem()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    check(SphereComponent);
    SphereComponent->InitSphereRadius(30.f);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    SphereComponent->SetGenerateOverlapEvents(true);
    SetRootComponent(SphereComponent);
}

void ATPSInventoryItem::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    if (const auto Pawn = Cast<APawn>(OtherActor))
    {
        if (const auto InventoryComponent = Pawn->FindComponentByClass<UTPSInventoryActorComponent>())
        {
            if (InventoryComponent->TryAddItem(InventoryData))
            {
                Destroy();
            }
        }
    }
}
