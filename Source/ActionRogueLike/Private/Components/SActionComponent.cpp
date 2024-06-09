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
		AddAction(Action, GetOwner());
	}
	
}

// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const FString DebugMessage = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMessage);
}

void USActionComponent::AddAction(TSubclassOf<USAction> ActionClass, AActor* Instigator)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if(NewAction)
	{
		Actions.Add(NewAction);
		if(NewAction->bAutoStart && ensure(NewAction->CanStart(Instigator)))
		{
			NewAction->StartAction(Instigator);
		}
	}
	
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	// Iterate through the array of actions using FName and determine if the action can be started
	for(USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				FString FailedMessage = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMessage);
				continue;
			}
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
			if(Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}

	return false;
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if(!ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		return;
	}
	Actions.Remove(ActionToRemove);

}
