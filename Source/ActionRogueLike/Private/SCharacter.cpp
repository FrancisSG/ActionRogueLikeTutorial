// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionRogueLike/Public/SCharacter.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Actors/Projectiles/ProjectileBase.h"
#include "Components/InputComponent.h"
#include "Components/SInteractionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/KismetMathLibrary.h"





// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	// Looking and camera behavior
	SpringArmComponent->bUsePawnControlRotation = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Interaction
	InteractionComponent = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));
	
	// Movement Behavior
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentProjectile = ProjectileClass;
	// Adding core movement input mapping context
	if(const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error! - SCharacter - PlayerController is not valid!"));
	}
	// DebugUtilityFunctions::RunConsoleCommand(FString("ShowDebug EnhancedInput"), GetWorld());

}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASCharacter::Jump);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASCharacter::Look);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASCharacter::Attack);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ASCharacter::Interact);
		EnhancedInputComponent->BindAction(AbilityShortcut_01_Action, ETriggerEvent::Triggered, this, &ASCharacter::AbilityShortcut_01);
		EnhancedInputComponent->BindAction(AbilityShortcut_02_Action, ETriggerEvent::Triggered, this, &ASCharacter::AbilityShortcut_02);
		EnhancedInputComponent->BindAction(AbilityShortcut_03_Action, ETriggerEvent::Triggered, this, &ASCharacter::AbilityShortcut_03);

	}

}

void ASCharacter::PrimaryAttack_TimeElapsed() const
{ 
	// The transform combines the location of the socket with the look rotation of the controller

	// Line tracing for potential targets
	FHitResult Hit;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	const FVector HandLocation = GetMesh()->GetSocketLocation(FName("Muzzle_01"));

	// Line tracing vectors
	const FVector TraceStart = CameraComponent->GetComponentLocation();
	const FVector TraceEnd = TraceStart + (GetControlRotation().Vector() * 3000.0f);
	
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, TraceStart, TraceEnd, ObjectQueryParams);
	
	/*FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 3.0f, 0, 2.0f);*/

	// If the trace has found a target, use the impact point as destination otherwise use the trace end which his less accurate
	const FRotator SpawnRotation = bBlockingHit ? (Hit.ImpactPoint - HandLocation).Rotation() : (Hit.TraceEnd - HandLocation).Rotation();

	// Instantiating a transform for the projectile's spawn
	const FTransform SpawnTM = FTransform(SpawnRotation,HandLocation);

	// Spawning the actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	// Removing the const from "this" by using const_cast
	APawn* CharacterAsPawn = Cast<APawn>(const_cast<ASCharacter*>(this));
	SpawnParams.Instigator = CharacterAsPawn;
	SpawnParams.Owner = CharacterAsPawn;
	// No need to store in array or store it can just use GetWorld()
	GetWorld()->SpawnActor<AActor>(CurrentProjectile, SpawnTM, SpawnParams);
}

void ASCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementAxisValue = Value.Get<FVector2D>();

	// We want the control rotation because we want to orient the rotation to the character's movement
	// Instead of using the character's forward vector we use the rotation and convert it to a vector
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	
	// Simpler way
	// Moving Forward and Back
	if(GetController() && (MovementAxisValue.X != 0.0f))
	{
		AddMovementInput(ControlRotation.Vector(), MovementAxisValue.X);
	}
	// Moving Left and Right
	if(GetController() && (MovementAxisValue.Y != 0.0f))
	{
		const FVector RightVector = UKismetMathLibrary::GetRightVector(ControlRotation);
		// Another way of getting the right vector
		// const FVector RightVector = FRotationMatrix(ControlRotation).GetScaleAxis(EAxis::Y);
		AddMovementInput(RightVector, MovementAxisValue.Y);
	}
	
	// By finding getting the forward vector using the Yaw rotation of the character
	/*if(const AController* PlayerController = GetController())
	{
		const FRotator Rotation = PlayerController->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementAxisValue.X);
	}*/
}

void ASCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if(GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ASCharacter::Jump()
{
	Super::Jump();
}

void ASCharacter::Attack(const FInputActionValue& Value)
{

	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
	// GetWorldTimerManager().ClearTimer(TimerHandle_PrimaryAttack);
}

void ASCharacter::Interact(const FInputActionValue& Value)
{
	if(ensure(InteractionComponent))
	{
		InteractionComponent->PrimaryInteract();
	}
}

void ASCharacter::AbilityShortcut_01(const FInputActionValue& Value)
{
	if(ensure(ProjectileClass))
	{
		CurrentProjectile = ProjectileClass;
	}
}

void ASCharacter::AbilityShortcut_02(const FInputActionValue& Value)
{
	if(ensure(BlinkProjectileClass))
	{
		CurrentProjectile = BlackHoleProjectileClass;
	}
}

void ASCharacter::AbilityShortcut_03(const FInputActionValue& Value)
{
	if(ensure(BlinkProjectileClass))
	{
		CurrentProjectile = BlinkProjectileClass;
	}
}




