// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_GameModeBase.h"

void ACJS_GameModeBase::BeginPlay()
{
    Super::BeginPlay();

    // 방에 맞는 PlayerController 클래스를 설정
    PlayerControllerClass = LobbyControllerClass;
}
