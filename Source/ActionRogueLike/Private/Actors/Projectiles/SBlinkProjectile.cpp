// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectiles/SBlinkProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ASBlinkProjectile::ASBlinkProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ExplosionEffectComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEffectComponent"));
	ExplosionEffectComponent->SetupAttachment(GetRootComponent());
	
}

// Called when the game starts or when spawned
void ASBlinkProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(ProjectileTimer, this, &ASBlinkProjectile::InitialExplosion, 0.2, false);
	
}

void ASBlinkProjectile::ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetActorEnableCollision(false);
	InitialExplosion();
}

void ASBlinkProjectile::InitialExplosion()
{
	// Stop the projectile's movement
	FHitResult Hit;
	ProjectileMovementComponent->StopSimulating(Hit);
	// VFX 
	EffectComponent->SetHiddenInGame(true);
	ExplosionEffectComponent->Activate();
	// Timers
	ProjectileTimer.Invalidate();
	GetWorldTimerManager().SetTimer(ProjectileTimer, this, &ASBlinkProjectile::SecondExplosion, 0.2, false);
}

void ASBlinkProjectile::SecondExplosion()
{
	GetInstigator()->SetActorLocation(GetActorLocation());
	// Destroys the actor and clears it from World
	Destroy();
}

// Called every frame
void ASBlinkProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

