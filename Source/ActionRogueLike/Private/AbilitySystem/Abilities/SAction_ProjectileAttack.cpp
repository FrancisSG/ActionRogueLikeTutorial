// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/SAction_ProjectileAttack.h"

#include "Actors/Projectiles/SMagicProjectile.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


USAction_ProjectileAttack::USAction_ProjectileAttack()
{

	HandSocketName = "Muzzle_01";
	AttackAnimDelay = 0.2f;
	
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(ACharacter* Character = Cast<ACharacter>(Instigator))
	{
		// Play Attack Montage
		Character->PlayAnimMontage(AttackAnim);
		
		UGameplayStatics::SpawnEmitterAttached(EmitterComponent, Character->GetMesh(), HandSocketName,
			FVector::ZeroVector, FRotator::ZeroRotator,EAttachLocation::SnapToTarget);

		// Delay logic
		FTimerHandle TimerHandle_AttackDelay;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, Delegate, AttackAnimDelay, false);
		// This difference between the below old implementation is that the referenced function below has no params, and in this
		// case we need params so it's better to create a FTimerDelegate which takes the function name and whatever params you need to pass
		// GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);

	}
	
	
	
	
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatingCharacter)
{
	if(ensureAlways(ProjectileClass))
	{
		// The transform combines the location of the socket with the look rotation of the controller
		// Line tracing for potential targets
		FHitResult Hit;
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic); 
		const FVector HandLocation = InstigatingCharacter->GetMesh()->GetSocketLocation(FName("Muzzle_01"));

		// Line tracing vectors
		const FVector TraceStart = 	InstigatingCharacter->GetPawnViewLocation();
		const FVector TraceEnd = TraceStart + (InstigatingCharacter->GetControlRotation().Vector() * 3000.0f);
	
		bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectQueryParams);
	
		/*FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;%
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 3.0f, 0, 2.0f);*/

		// If the trace has found a target, use the impact point as destination otherwise use the trace end which his less accurate
		const FRotator SpawnRotation = bBlockingHit ? (Hit.ImpactPoint - HandLocation).Rotation() : (Hit.TraceEnd - HandLocation).Rotation();

		// Instantiating a transform for the projectile's spawn
		const FTransform SpawnTM = FTransform(SpawnRotation,HandLocation);

		// Spawning the actor
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatingCharacter;
		SpawnParams.Owner = InstigatingCharacter;
		// No need to store in array or store it can just use GetWorld()
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}

	StopAction_Implementation(InstigatingCharacter);
}