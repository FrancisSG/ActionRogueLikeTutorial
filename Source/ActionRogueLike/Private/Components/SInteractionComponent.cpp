// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SInteractionComponent.h"

#include "Libraries/Interfaces/SGameplayInterface.h"

void USInteractionComponent::PrimaryInteract()
{
	// Using line trace or sphere trace - We are tracing regardless
	
	// Query parameters to check what kind of objects we want to detect
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();

	// Initialising the start and end location of the trace
	FVector EyeLocation;
	FRotator EyeRotation;
	// Get actor eyes view point is a convenient way to get the sphere trace started
	// Passing variables by references and they get initialized
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	FVector End  = EyeLocation + (EyeRotation.Vector() * 1000);
	
	/*FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);*/

	float Radius = 30.0f;
	
	TArray<FHitResult> Hits;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	// Hits is required as the SweepMulti may hit multiple times so it needs an array of hits to store
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	for(FHitResult Hit : Hits)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
				// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 12, LineColor, false, 5.0f, 0, 1.0f);
				break;
			}
		}
	}
	DrawDebugLine(GetWorld(), EyeLocation, End, LineColor, false, 3.0f, 0, 2.0f);

	/*if (AActor* HitActor = Hit.GetActor())
	{
		if(HitActor->Implements<USGameplayInterface>())
		{
			APawn* MyPawn = Cast<APawn>(MyOwner);
			ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
		}
	}*/


}

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

