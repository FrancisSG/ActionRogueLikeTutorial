// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USActionComponent;
class AProjectileBase;
class USInteractionComponent;
class UInputAction;
class UAnimMontage;
struct FInputActionValue;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USInteractionComponent> InteractionComponent;
	// References
	UPROPERTY(EditAnywhere, Category = "References|Projectiles")
	TSubclassOf<AProjectileBase> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category = "References|Projectiles")
	TSubclassOf<AProjectileBase> BlinkProjectileClass;
	
	
	
	// Input Variables
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AbilityShortcut_01_Action;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AbilityShortcut_02_Action;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AbilityShortcut_03_Action;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> SprintAction;
	
	FName CurrentProjectileAbilityName = FName("PrimaryAttack");

	// Ability system component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USActionComponent> ActionComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	// Inputs Functions

	/** Called for movement input **/
	void Move(const FInputActionValue& Value);
	
	/** Called for turning and camera movement **/
	void Look(const FInputActionValue& Value);
	
	/** Called for jumping **/
	virtual void Jump() override;

	/** Called when attacking **/
	virtual void Attack(const FInputActionValue& Value);

	/** Called when pressing the interact key **/
	virtual void Interact(const FInputActionValue& Value);

	/** Abilities switcher function **/
	virtual void AbilityShortcut_01(const FInputActionValue& Value);
	virtual void AbilityShortcut_02(const FInputActionValue& Value);
	virtual void AbilityShortcut_03(const FInputActionValue& Value);

	void SprintStart(const FInputActionValue& Value);
	void SprintEnd(const FInputActionValue& Value);

	// GetPawnViewLocation to get camera component
	virtual FVector GetPawnViewLocation() const override;
};
