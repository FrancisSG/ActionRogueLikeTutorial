// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "SBlackHoleProjectile.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASBlackHoleProjectile : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASBlackHoleProjectile();

protected:

	FTimerHandle BlackHoleTimerHandle;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	virtual void EndBlackHole();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
