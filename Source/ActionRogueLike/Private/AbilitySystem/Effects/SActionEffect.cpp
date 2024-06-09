// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect.h"

#include "Components/SActionComponent.h"



USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	
}

void USActionEffect::ExecuteEffect_Implementation(AActor* Instigator)
{
	
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if (Duration > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "StopAction", Instigator);
		GetWorld()->GetTimerManager().SetTimer(DurationHandle, Delegate, Duration, false);

		ExecuteEffect(Instigator);
		
	}

	if (Period > 0.0f)
	{
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);

		GetWorld()->GetTimerManager().SetTimer(PeriodHandle, Delegate, Period, true);
	}
	
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	// This has been implemented for safety and to not miss out on any remaining ticks for the periodic effect
	if(GetWorld()->GetTimerManager().GetTimerRemaining(PeriodHandle) < KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(PeriodHandle);
	GetWorld()->GetTimerManager().ClearTimer(DurationHandle);

	USActionComponent* Component = GetOwningComponent();
	if(Component)
	{
		Component->RemoveAction(this);
	}
	
}
