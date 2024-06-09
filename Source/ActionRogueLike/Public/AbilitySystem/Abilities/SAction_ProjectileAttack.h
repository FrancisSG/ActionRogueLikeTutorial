// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class AProjectileBase;
class UAnimMontage;
class UParticleSystemComponent;
/**
 * This ability spawns a basic projectile, and requires different variables which will then be passed
 * on the actual projectile to be spawned. Hence the different protected member variables
 */
UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "References|Projectiles")
	TSubclassOf<AProjectileBase> ProjectileClass;
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackAnim;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName HandSocketName = FName();

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TObjectPtr<UParticleSystem> EmitterComponent;
	
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatingCharacter);
	
public:
	// Class constructor
	USAction_ProjectileAttack();

	// Overriding the base class' implementation of start action as each ability will have its unique start possibly
	virtual void StartAction_Implementation(AActor* Instigator) override;
	
};
