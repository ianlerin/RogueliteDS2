// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/Elemental/LongBurnAbility.h"


float ULongBurnAbility::GetBurnExtendDuration()
{
	if (BurnExtendMap.Contains(GetAbilityLevel()))
	{
		return BurnExtendMap[GetAbilityLevel()];
	}
	return 0.0f;
}