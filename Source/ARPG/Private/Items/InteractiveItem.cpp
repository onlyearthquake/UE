// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/InteractiveItem.h"

#include "Components/BoxComponent.h"

AInteractiveItem::AInteractiveItem()
{
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	RootComponent = BoxComponent;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetupAttachment(RootComponent);
}

void AInteractiveItem::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this,&AInteractiveItem::OnActorOverlap);
}

void AInteractiveItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractiveItem::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	OnActorIn(OtherActor,SweepResult);
}