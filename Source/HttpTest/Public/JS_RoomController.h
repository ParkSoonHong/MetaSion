// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "JS_RoomController.generated.h"

UCLASS()
class HTTPTEST_API AJS_RoomController : public APlayerController
{
    GENERATED_BODY()

    
protected:
    virtual void BeginPlay() override;
    
public:
    AJS_RoomController(); // 생성자 추가

    virtual void SetupInputComponent() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* IMC_Controller;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_LeftMouse;

    void OnMouseClick();
};
