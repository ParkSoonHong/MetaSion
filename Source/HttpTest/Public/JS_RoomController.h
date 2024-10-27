// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "JS_RoomWidget.h"
#include "JS_RoomController.generated.h"

//블루프린트에서 바인딩할 수 있는 이벤트 선언
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTaggedActorClickedSignature);

UCLASS()
class HTTPTEST_API AJS_RoomController : public APlayerController
{
    GENERATED_BODY()

    
protected:
    virtual void BeginPlay() override;
    
public:
    AJS_RoomController(); // 생성자 추가

	virtual void Tick(float DeltaTime) override;

    virtual void SetupInputComponent() override;


    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* IMC_Controller;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* IA_LeftMouse;

	/*UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnTaggedActorClickedSignature OnTaggedActorClicked;*/

    // 인터페이스를 위한 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_CreateRoomWidget> CR_UIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_RoomWidget> R_UIFactory;

	UPROPERTY(EditAnywhere)
	class UJS_CreateRoomWidget* CR_WidgetUI;

	UPROPERTY(EditAnywhere)
	class UJS_RoomWidget* R_UI;

    // 무조건 인덱스 보내기
    bool bOnlyIndexSend = false;

    
	// UI세팅 함수
    bool bShowUI = false;
    FTimerHandle PlayAnimHandle;

    void InitializeUIWidgets();
    void ShowCreateRoomUI();
    void HideCreateRoomUI();
    void ShowRoomUI();
    void HideRoomUI();
    void PlayUIAnimation();
    void OnMouseClick();
};
