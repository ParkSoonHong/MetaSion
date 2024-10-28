// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CJS_BallPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UCJS_BallPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACJS_BallPlayer* Player;

	// 숫자키 1~8 감정 표현
	// Montage 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Anim")
	class UAnimMontage* AngryMontage;
	void PlayAngryMontage();
	UFUNCTION()
	void PlayEnjoyAnim();
	UFUNCTION()
	void PlayFearAnim();
	UFUNCTION()
	void PlayKissAnim();
	UFUNCTION()
	void PlayLaughAnim();
	UFUNCTION()
	void PlaySadAnim();
	UFUNCTION()
	void PlaySpinAnim();
};
