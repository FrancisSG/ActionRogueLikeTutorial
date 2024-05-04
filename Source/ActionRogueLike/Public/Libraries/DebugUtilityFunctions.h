#pragma once
#include "CoreMinimal.h"

class UGameplayStatics;

class ACTIONROGUELIKE_API DebugUtilityFunctions
{
	public:
		// Runs a console command using the player controller
		static void RunConsoleCommand(const FString& InCommandString, const UWorld* InWorld);	
};
