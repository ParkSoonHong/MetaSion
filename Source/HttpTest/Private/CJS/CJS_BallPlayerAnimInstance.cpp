// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_BallPlayerAnimInstance.h"
#include "CJS/CJS_BallPlayer.h"

void UCJS_BallPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//UE_LOG(LogTemp, Warning, TEXT("UCJS_BallPlayerAnimInstance::NativeUpdateAnimation()"));

	// 캐릭터가 현재 할당되지 않았다면 시도해서 가져옴
	if (Player == nullptr)
	{
		Player = Cast<ACJS_BallPlayer>(TryGetPawnOwner());
	}

	// Player가 유효하지 않다면 더 이상 실행하지 않음
	if (nullptr == Player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UCJS_BallPlayerAnimInstance::NativeUpdateAnimation()::Player Null "));
		return;
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("UCJS_BallPlayerAnimInstance::NativeUpdateAnimation()::Player Casting Ok"));
	}
}

void UCJS_BallPlayerAnimInstance::PlayAngryMontage()
{
	UE_LOG(LogTemp, Warning, TEXT("UCJS_BallPlayerAnimInstance::PlayAngryMontage()"));
	
	if (AngryMontage)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCJS_BallPlayerAnimInstance::PlayAngryMontage() - Montage is not null, playing animation"));
		Montage_Play(AngryMontage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UCJS_BallPlayerAnimInstance::PlayAngryMontage() - AngryMontage is null, cannot play animation"));
	}
}

void UCJS_BallPlayerAnimInstance::PlayEnjoyAnim()
{

}

void UCJS_BallPlayerAnimInstance::PlayFearAnim()
{

}

void UCJS_BallPlayerAnimInstance::PlayKissAnim()
{

}

void UCJS_BallPlayerAnimInstance::PlayLaughAnim()
{

}

void UCJS_BallPlayerAnimInstance::PlaySadAnim()
{

}

void UCJS_BallPlayerAnimInstance::PlaySpinAnim()
{

}
