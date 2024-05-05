#pragma once
#include "Kismet/GameplayStatics.h"

// Running Commands
#define DRAW_DEBUG_BOX(StartLocation, Duration) if (GetWorld()) \
	{ \
		DrawDebugBox(GetWorld(), StartLocation, FVector(40.0f, 40.0f,40.0f), FColor::Cyan, false, Duration); \
	}