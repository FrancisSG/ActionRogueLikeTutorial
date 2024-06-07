// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Projectiles/ProjectileBase.h"
#include "SBlinkProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASBlinkProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASBlinkProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	FTimerHandle ProjectileTimer;

	virtual void InitialExplosion();
	virtual void SecondExplosion();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UParticleSystemComponent> ExplosionEffectComponent;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
