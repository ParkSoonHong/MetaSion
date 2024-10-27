// Fill out your copyright notice in the Description page of Project Settings.

#include "JS_TestWidget.h"
#include "Components/Button.h"
#include "JS_RoomController.h"
#include "Kismet/GameplayStatics.h"

void UJS_TestWidget::NativeConstruct()
{
	btn_TestButton->OnClicked.AddDynamic(this, &UJS_TestWidget::TestUI);
}

void UJS_TestWidget::TestUI()
{
    if (AJS_RoomController* RoomController = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
    {
        RoomController->ShowCreateRoomUI();
        /*RoomController->bShowUI = true;*/
        RemoveFromParent();
    }
}
