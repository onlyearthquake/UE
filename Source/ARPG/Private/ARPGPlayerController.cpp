
#include "ARPGPlayerController.h"

AARPGPlayerController::AARPGPlayerController()
{
}

bool AARPGPlayerController::Reborn()
{
	if(RebornTime <= 0)
	{
		return false;
	}
	else
	{
		RebornTime--;
		return true;
	}
}
