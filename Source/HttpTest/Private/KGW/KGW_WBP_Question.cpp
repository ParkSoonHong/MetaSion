// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_WBP_Question.h"
#include "Components/Button.h"

void UKGW_WBP_Question::NativeConstruct()
{
	Super::NativeConstruct();

	BT_Select->OnClicked.AddDynamic(this, &UKGW_WBP_Question::OnClickSelect);

}

void UKGW_WBP_Question::OnClickSelect()
{
}
