// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_GameModeBase.h"
#include "JS_RoomController.h"

void AJS_GameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // 방에 맞는 PlayerController 클래스를 설정
    PlayerControllerClass = RoomControllerClass;
}
