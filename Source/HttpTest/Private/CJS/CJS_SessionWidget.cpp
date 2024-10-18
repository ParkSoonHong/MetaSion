// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_SessionWidget.h"
#include "CJS/SessionGameInstance.h"

#include "../HttpTest.h"

#include "Components/Button.h"



void UCJS_SessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("UCJS_SessionWidget::NativeConstruct()"));
	PRINTLOG(TEXT("Click Create Button"));
	if (Btn_Create)
	{
		Btn_Create->OnClicked.AddDynamic(this, &UCJS_SessionWidget::OnClickCreateSession);
		UE_LOG(LogTemp, Warning, TEXT("Btn_Create is bound successfully."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Btn_Create is null. Please check if it is properly bound in the widget blueprint."));
	}
}

void UCJS_SessionWidget::OnClickCreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_SessionWidget::OnClickCreateSession()"));
	auto* gi = Cast<USessionGameInstance>(GetWorld()->GetGameInstance());
	if(!gi) return;

	PRINTLOG(TEXT("Serching for Exsiting Session"));
	gi->FindSessions();
}
