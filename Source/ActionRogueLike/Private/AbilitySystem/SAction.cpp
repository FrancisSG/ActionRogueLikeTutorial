// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SAction.h"
#include "Components/ActorComponent.h"
#include "Components/SActionComponent.h"


void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Running: %s"), *GetNameSafe(this));

	// We will add any gameplay tags to the Outer's ActionComponent and not through the instigator
	// Because the Outer is often times in this system, the ActionComponent you feel? Since SAction.cpp is a action that is
	// instantiated in the ActionComponent in the first place
	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.AppendTags(GrantsTags);
	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	
	UE_LOG(LogTemp, Log, TEXT("Stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);
	
	USActionComponent* ActionComponent = GetOwningComponent();
	ActionComponent->ActiveGameplayTags.RemoveTags(GrantsTags);
	bIsRunning = false;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}

bool USAction::IsRunning() const
{
	return bIsRunning;
}

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if(!bIsRunning)
	{
		if(const USActionComponent* Component = GetOwningComponent())
		{
			if(Component->ActiveGameplayTags.HasAny(BlockedTags))
			{
				return false;
			}
			return true;
		}
	}
	
	return false;
}

UWorld* USAction::GetWorld() const
{
	// Since we add actions/abilities through the function AddAction in the ActionAbilitiesComponent, technically
	// the outer in this case is the ActionAbilitiesComponent. It has been set view NewObject<T> 
	if(const UActorComponent* Component = Cast<UActorComponent>(GetOuter()))
	{
		return Component->GetWorld();
	}
	return nullptr;
}
