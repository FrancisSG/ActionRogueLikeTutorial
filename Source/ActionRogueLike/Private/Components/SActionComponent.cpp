// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SActionComponent.h"

#include "AbilitySystem/SAction.h"


// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Init Array of actions from default actions array
	for(const TSubclassOf<USAction> Action : DefaultActions)
	{
		AddAction(Action);
	}
	
}

// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if(NewAction)
	{
		Actions.Add(NewAction);
	}
	
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	// Iterate through the array of actions using FName and determine if the action can be started
	for(USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	// Iterate through the available actions using FName and determine if the action can be stopped
	for(USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			Action->StopAction(Instigator);
			return true;
		}
	}

	return false;
}