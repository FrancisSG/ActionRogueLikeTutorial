#include "Libraries/DebugUtilityFunctions.h"
#include "Kismet/GameplayStatics.h"

void DebugUtilityFunctions::RunConsoleCommand(const FString& InCommandString, const UWorld* InWorld)
{
	if(InWorld)
	{
		if(APlayerController* PlayerController = UGameplayStatics::GetPlayerController(InWorld, 0))
		{
			PlayerController->ConsoleCommand(InCommandString, true);
		}

	}
}
