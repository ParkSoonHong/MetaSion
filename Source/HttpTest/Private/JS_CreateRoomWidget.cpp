// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_CreateRoomWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"

void UJS_CreateRoomWidget::NativeConstruct()
{
	Super::NativeConstruct();

    // 위젯들이 올바르게 바인딩되었는지 확인
    if (btn_CreateRoom_Yes == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("btn_CreateRoom_Yes is not bound!"));
    }

    if (btn_CreateRoom_No == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("btn_CreateRoom_No is not bound!"));
    }

    if (btn_CreateRoom_Private == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("btn_CreateRoom_Private is not bound!"));
    }

    // 이벤트 바인딩
    if (btn_CreateRoom_Yes)
    {
        btn_CreateRoom_Yes->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseYes);
    }

    if (btn_CreateRoom_No)
    {
        btn_CreateRoom_No->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::CreateRoomChooseNo);
    }

    if (btn_CreateRoom_Private)
    {
        btn_CreateRoom_Private->OnClicked.AddDynamic(this, &UJS_CreateRoomWidget::SetPrivate);
    }

}

void UJS_CreateRoomWidget::SwitchToWidget(int32 index)
{
	if (CR_WidgetSwitcher) {
		// 인덱스에 따라 활성화할 위젯 전환
		CR_WidgetSwitcher->SetActiveWidgetIndex(index);
	}
}
//방 꾸미기 완료 시 SwitchToWidget(0)을 호출하게 만들어야함.
void UJS_CreateRoomWidget::CreateRoomChooseYes()
{
	// 방 꾸미기 완료 시 넣어야 할 것
	// 방 생성 2번 째 질문 UI 보이기
	SwitchToWidget(1);
}

void UJS_CreateRoomWidget::CreateRoomChooseNo()
{
	// 방 꾸미기 아니요 시 넣어야 할 것
	// 방 꾸미기 완료 UI 숨기기
	this->SetVisibility(ESlateVisibility::Hidden);
}

void UJS_CreateRoomWidget::SetPrivate()
{
	bPrivate = !bPrivate;
	SwitchToWidget(2);
	ShowUIForLimitedTime(3);
}

void UJS_CreateRoomWidget::ShowUIForLimitedTime(float DurationInSeconds)
{
	SetVisibility(ESlateVisibility::Visible);
	if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(Handler, this, &UJS_CreateRoomWidget::HideUI, DurationInSeconds, false);
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetWorld() returned NULL!"));
    }
}

void UJS_CreateRoomWidget::HideUI()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}
