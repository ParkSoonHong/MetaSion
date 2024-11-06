// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_JS_WidgetFunction.h"
#include "CJS/CJS_ChatWidget.h"


void ACJS_JS_WidgetFunction::BeginPlay()
{
	Super::BeginPlay();

	// WBP_AimPoint 위젯 생성
	if (WBP_Chat)  // WBP_aimpoint가 올바르게 할당되어 있는지 확인
	{
		ChatUI = CreateWidget<UCJS_ChatWidget>(GetWorld(), WBP_Chat);
		if (ChatUI)
		{
			ChatUI->AddToViewport();
			UE_LOG(LogTemp, Warning, TEXT("ChatUI successfully created and added to viewport & Hidden right now"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create ChatUI Widget"));
		}
		UE_LOG(LogTemp, Warning, TEXT("ACJS_JS_WidgetFunction::BeginPlay()::WBP_Chat is assigned!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_JS_WidgetFuntcion::BeginPlay()::WBP_Chat is not assigned! Please assign it in the Blueprint."));
	}

	ChatUIShowing = false;
}

void ACJS_JS_WidgetFunction::ToggleChatUIVisible()
{
	if (!ChatUIShowing)
	{
		ChatUIShowing = true;
		ChatUI->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		ChatUIShowing = false;
		ChatUI->SetVisibility(ESlateVisibility::Hidden);
	}
}
