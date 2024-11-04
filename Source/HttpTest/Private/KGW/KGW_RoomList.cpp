// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_RoomList.h"
#include "KGW/KGW_UserRoomName.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "JsonParseLib.h"

void UKGW_RoomList::AddSessionSlotWidget(const TArray<FString>& RoomNames)
{

	TMap<FString, FString> UserData;
	FLogin UserIDData;

	FString TextID = UserIDData.userId;
	UserData.Add("userId", TextID);

	FString json = UJsonParseLib::MakeJson(UserData);


	for (const FString& RoomName : RoomNames)
	{
		// UKGW_UserRoomName 위젯 생성
		auto* RoomSlot = CreateWidget<UKGW_UserRoomName>(this, UserRoomNameFactory);
		if (RoomSlot)
		{
			// roomName 정보를 위젯에 업데이트
// 			Slot->UpdateInfo(RoomName);

			// ScrollBox에 위젯 추가
			ScrollBox->AddChild(RoomSlot);
		}
	}
	


}

void UKGW_RoomList::SetFindActive(bool value)
{
	//찾기가 끝나면 Empty 텍스트 안보이게 하고싶다.
	if (value) {
		Text_Finding->SetVisibility(ESlateVisibility::Visible);

	}
	else {
		Text_Finding->SetVisibility(ESlateVisibility::Hidden);

		
	}


}
