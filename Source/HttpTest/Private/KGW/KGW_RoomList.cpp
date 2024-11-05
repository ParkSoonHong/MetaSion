// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_RoomList.h"
#include "KGW/KGW_UserRoomName.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "HttpActor.h"

void UKGW_RoomList::AddSessionSlotWidget(const TArray<FMyCreatedRoom>& RoomInfos)
{

    if (!ScrollBox)
    {
        UE_LOG(LogTemp, Error, TEXT("ScrollBox is null! Make sure it is set correctly in the widget."));
        return;
    }
//     ScrollBox->ClearChildren();
    for (const FMyCreatedRoom& Room : RoomInfos)
    {
        auto* RoomSlot = CreateWidget<UKGW_UserRoomName>(this, UserRoomNameFactory);
        if (RoomSlot)
        {
            // RoomWidget의 정보를 업데이트 (예: 방 이름 설정)
            RoomSlot->UpdateInfo(Room);
            ScrollBox->AddChild(RoomSlot);
        }
    }  
    UE_LOG(LogTemp, Log, TEXT("Room list updated with %d rooms."), RoomInfos.Num());

}

// void UKGW_RoomList::SetFindActive(bool value)
// {
// 	//찾기가 끝나면 Empty 텍스트 안보이게 하고싶다.
// 	if (value) {
// 		Text_Finding->SetVisibility(ESlateVisibility::Visible);
// 
// 	}
// 	else {
// 		Text_Finding->SetVisibility(ESlateVisibility::Hidden);
// 
// 		
// 	}
// 
// 
// }
