// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_UserRoomName.h"
#include "Components/TextBlock.h"
#include "JsonParseLib.h"

void UKGW_UserRoomName::UpdateInfo(const struct FMyCreateRoomInfo& info)
{
	Text_RoomName->SetText(FText::FromString(info.RoomName));

}
