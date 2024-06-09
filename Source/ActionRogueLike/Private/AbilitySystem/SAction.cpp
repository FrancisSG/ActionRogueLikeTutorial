// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SAction.h"
#include "Components/ActorComponent.h"


void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));
}

UWorld* USAction::GetWorld() const
{
	// Since we add actions/abilities through the function AddAction in the ActionAbilitiesComponent, technically
	// the outer in this case is the ActionAbilitiesComponent. It has been set view NewObject<T> 
	if(const UActorComponent* Comp = Cast<UActorComponent>(GetOuter()))
	{
		return Comp->GetWorld();
	}
	return nullptr;
}
