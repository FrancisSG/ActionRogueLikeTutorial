// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SAction.generated.h"

/**
 * 
 */

class UWorld;

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:

	// Action's identifier
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;
	
	/**
	 *
	 * Function responsible for starting the action and its functionality
	 * 
	 * @param Instigator is the actor that has started this action
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);

	/**
	 *
	 * Function responsible for ending the action
	 * 
	 * @param Instigator 
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor* Instigator);

	UWorld* GetWorld() const override;
	
};
