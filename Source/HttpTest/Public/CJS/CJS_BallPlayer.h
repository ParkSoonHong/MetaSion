// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CJS_BallPlayer.generated.h"

UCLASS()
class HTTPTEST_API ACJS_BallPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACJS_BallPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// 카메라 ==============================================================================================
	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;
	FVector Direction; 

	// 인풋 동작 ==============================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputMappingContext* IMC_Player;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Look;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Jump;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_Throw;
	
	void OnMyActionMove(const FInputActionValue& Value);
	void OnMyActionLook(const FInputActionValue& Value);
	void OnMyActionJump(const FInputActionValue& Value);
	void OnMyActionThrow(const FInputActionValue& Value);


	// 인풋 애니메이션 ====================================================================
	UPROPERTY()
	class UCJS_BallPlayerAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_1;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_2;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_3;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_4;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_5;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_6;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_7;
	UPROPERTY(EditDefaultsOnly, Category = "INPUT")
	class UInputAction* IA_8;
	
	void OnMyActionKey1(const FInputActionValue& Value);
	void OnMyActionKey2(const FInputActionValue& Value);
	void OnMyActionKey3(const FInputActionValue& Value);
	void OnMyActionKey4(const FInputActionValue& Value);
	void OnMyActionKey5(const FInputActionValue& Value);
	void OnMyActionKey6(const FInputActionValue& Value);
	void OnMyActionKey7(const FInputActionValue& Value);
	void OnMyActionKey8(const FInputActionValue& Value);


	// 부딪혔을 때 ==============================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UParticleSystem* HitVFXFactory;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UParticleSystem* SelfHitVFXFactory;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class USoundBase* HitSFX;
	void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);
	void TriggerSelfHitEffects(FVector HitLocation);


	// 하트 던질 때 ============================================================================================
	UPROPERTY(EditDefaultsOnly, Category = "Heart")
	TSubclassOf<class ACJS_HeartActor> HeartItemFactory;
	UPROPERTY(EditAnyWhere)
	FVector HeartSpawnPosition;


};
