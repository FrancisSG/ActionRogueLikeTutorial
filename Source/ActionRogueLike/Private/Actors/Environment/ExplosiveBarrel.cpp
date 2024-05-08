// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Environment/ExplosiveBarrel.h"

#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	StaticMeshComponent->SetSimulatePhysics(true);
	
	RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	RadialForceComponent->SetupAttachment(GetRootComponent());
	RadialForceComponent->bImpulseVelChange = true;
	RadialForceComponent->ImpulseStrength = 2500.0f;
	RadialForceComponent->ForceStrength = 20.0f;
	RadialForceComponent->Radius = 750.0f;
	RadialForceComponent->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(GetRootComponent());
	ParticleSystemComponent->bAutoActivate = false;

	// Binding either in Constructor() or in PostInitializeComponents() below
	// StaticMeshComponent->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::Explode);
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

void AExplosiveBarrel::Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
	ParticleSystemComponent->Activate();

	DrawDebugString(GetWorld(), Hit.ImpactPoint,FString("Exploded!!") ,nullptr, FColor::Green, 12.0f);
	
	// Hides the barrel actor ("Destroying it")
	CleanUpBarrel();
}



void AExplosiveBarrel::CleanUpBarrel()
{
	StaticMeshComponent->SetHiddenInGame(true);
	SetActorEnableCollision(false);
}

// This runs before the Begin play and after the construction so it's in between
void AExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::Explode);

}


// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

