// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/SBlackHoleProjectile.h"

#include "Components/SphereComponent.h"


// Sets default values
ASBlackHoleProjectile::ASBlackHoleProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASBlackHoleProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(BlackHoleTimerHandle, this, &ASBlackHoleProjectile::EndBlackHole, 5.0, false);
}

void ASBlackHoleProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ASBlackHoleProjectile::OnSphereOverlap);
}

void ASBlackHoleProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor != GetInstigator())
	{
		OtherActor->Destroy();
	}
}

void ASBlackHoleProjectile::EndBlackHole()
{
	Destroy();	
}


// Called every frame
void ASBlackHoleProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



