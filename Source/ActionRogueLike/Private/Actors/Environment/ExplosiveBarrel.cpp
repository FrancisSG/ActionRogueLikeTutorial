// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Environment/ExplosiveBarrel.h"

#include "Libraries/DebugMacros.h"
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
	RadialForceComponent->ImpulseStrength = 100000.0f;
	RadialForceComponent->ForceStrength = 20.0f;
	RadialForceComponent->Radius = 1500.0f;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(GetRootComponent());
	ParticleSystemComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &AExplosiveBarrel::Explode);
	
}

void AExplosiveBarrel::Explode(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	RadialForceComponent->FireImpulse();
	ParticleSystemComponent->Activate();
	// Hides the barrel actor ("Destroying it")
	CleanUpBarrel();
}

void AExplosiveBarrel::CleanUpBarrel()
{
	StaticMeshComponent->SetHiddenInGame(true);
	SetActorEnableCollision(false);
}


// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

