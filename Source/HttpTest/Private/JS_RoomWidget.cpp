// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_RoomWidget.h"
#include "Components/Button.h"
#include "JsonParseLib.h"
#include "Components/TextBlock.h"
#include "JS_RoomController.h"
#include "HttpActor.h"
#include "Kismet/GameplayStatics.h"

void UJS_RoomWidget::NativeConstruct()
{
	btn_SignComplete->OnClicked.AddDynamic(this, &UJS_RoomWidget::SendChangeIndexData);

    httpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
}
//index 요청
void UJS_RoomWidget::SendChangeIndexData()
{
    if (!httpActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("httpActor is null in SendChangeIndexData"));
        return;
    }

    FChangeIndex ChangeIndexData;
    //지금은 서버가 없어서 임시로 임의의 값을 넣음.
    ChangeIndexData.index = 1;

    FString json = UJsonParseLib::ChangeIndex_Convert_StructToJson(ChangeIndexData);
    

    httpActor->ChangeIndexReqPost(httpActor->ServerURL, json);
}

void UJS_RoomWidget::SetIndex(int WallPaperIndex, int absWallPaperIndex)
{
	txt_index->SetText(FText::AsNumber(WallPaperIndex));
	txt_absindex->SetText(FText::AsNumber(absWallPaperIndex));
}
