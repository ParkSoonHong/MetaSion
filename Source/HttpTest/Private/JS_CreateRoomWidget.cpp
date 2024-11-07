// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_CreateRoomWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"
#include "HttpActor.h"
#include "JsonParseLib.h"
#include "JS_WidgetFunction.h"

void UJS_CreateRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_CreateRoom_Yes->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseYes);
	btn_CreateRoom_No->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseNo);
	btn_CreateRoom_Private->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::SetPrivate);
	btn_CreateRoom_Public->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::SetPrivate);
	btn_CompleteCreateRoom->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CompleteCreateRoom);

	pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	httpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
	widgetActor = Cast<AJS_WidgetFunction>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_WidgetFunction::StaticClass()));
}
//widget Switch
void UJS_CreateRoomWidget::SwitchToWidget(int32 index)
{
	if (CR_WidgetSwitcher) {
		CR_WidgetSwitcher->SetActiveWidgetIndex(index);
	}
}
//private, public
void UJS_CreateRoomWidget::SwitchToWidget_PP(int32 index)
{
	if (CR_WidgetSwitcherPP) {
		CR_WidgetSwitcherPP->SetActiveWidgetIndex(index);
	}
}
void UJS_CreateRoomWidget::CreateRoomChooseYes()
{
	SwitchToWidget(1);
}

void UJS_CreateRoomWidget::CreateRoomChooseNo()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UJS_CreateRoomWidget::CompleteCreateRoom()
{
	if (ED_RoomName && !ED_RoomName->GetText().IsEmpty()) {
		SwitchToWidget(2);
		ShowUIForLimitedTime(3);
		widgetActor->SetActorVisibilityVisible();
	}
}

void UJS_CreateRoomWidget::SetPrivate()
{
	bPrivate = 1 - bPrivate; // 0 -> 1 -> 0 
	SwitchToWidget_PP(bPrivate);
	SendSetPrivateRoom(bPrivate);
}

void UJS_CreateRoomWidget::ShowUIForLimitedTime(float DurationInSeconds)
{
	//Timer
	SetVisibility(ESlateVisibility::Visible);
	if (GetWorld()){
		GetWorld()->GetTimerManager().SetTimer(Handler, this, &UJS_CreateRoomWidget::HideUI, DurationInSeconds, false);
	}
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL!"));
    }
}

void UJS_CreateRoomWidget::HideUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
	if (pc) {
		pc->bShowUI = true;
	}
}

void UJS_CreateRoomWidget::SendSetPrivateRoom(int32 Room_pp)
{
	if (!httpActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("httpActor is null in SendSetPrivateRoom"));
		return;
	}
	FMyRoomInfo MyRoomInfo;
	MyRoomInfo.RoomName = ED_RoomName->GetText().ToString();
	MyRoomInfo.room_pp = FString::FromInt(Room_pp);

	FString json = UJsonParseLib::MyRoomInfo_Convert_StructToJson(MyRoomInfo);

	httpActor->MyRoomInfoReqPost(httpActor->MyRoomURL, json);
}

