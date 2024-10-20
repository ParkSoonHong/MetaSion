// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CJS_UserCharacter.generated.h"

UCLASS()
class HTTPTEST_API ACJS_UserCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* IMC_Player;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Jump;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Move;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Look;

	/** Click Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_Click;

	/** Toggle AimPoint Widget Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* IA_AimPoint;

	//FVector Direction;

public:
	// Sets default values for this character's properties
	ACJS_UserCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called for movement input
	void OnMyActionMove(const FInputActionValue& Value);

	// Called for looking input
	void OnMyActionLook(const FInputActionValue& Value);

	// Called for clicking input
	void OnMyActionClick(const FInputActionValue& Value);

	// Called for toggling AimPoint Widget input
	void OnMyActionToggleAimPointUI(const FInputActionValue& Value);



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	/** Widgets */
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> WBP_AimPoint;
	UPROPERTY()
	class UCJS_AimPointWidget* AimPointUI;
	bool bAimPointUIShowing;
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OnAimPointUIStateChanged(bool bIsVisible);


	// ClientTravel
	void RequestMapTravel(const FString& MapPath);

	// 클라 -> 서버에 이동 요청
	UFUNCTION(Server, Reliable)
	void Server_RequestMapTravel(const FString& MapPath);
};
