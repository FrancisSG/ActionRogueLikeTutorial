// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Libraries/Interfaces/SGameplayInterface.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BaseMesh;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UStaticMeshComponent> LidMesh;

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch = 110.0f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Interact BlueprintNativeEvent function from the interface
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
