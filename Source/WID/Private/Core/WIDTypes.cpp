// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WIDTypes.h"

bool WID::CheckEventInfo(const FHudEventInfo& EvnetInfo)
{
	return !EvnetInfo.IsEmpty();
}

bool WID::CheckEventInfo(const FHudEventInfoList& EvnetInfoList, int32 Index)
{
	if (EvnetInfoList.IsValidIndex(Index))
	{
		return !EvnetInfoList[Index].IsEmpty();
	}
	return false;
}