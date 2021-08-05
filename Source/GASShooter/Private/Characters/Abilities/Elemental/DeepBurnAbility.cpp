// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/Elemental/DeepBurnAbility.h"


float UDeepBurnAbility::GetBurnAdditionalDamage()
{
	if (BurnMap.Contains(GetAbilityLevel()))
	{
		return BurnMap[GetAbilityLevel()];
	}
	return 0.0f;
}