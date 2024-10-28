// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"
#include "JS_RoomWidget.h"
#include "JS_RoomController.generated.h"

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

    // 인터페이스를 위한 UI
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UHttpWidget>  LoginUIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_CreateRoomWidget> CR_UIFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_RoomWidget> R_UIFactory;

	UPROPERTY(EditAnywhere)
	class UHttpWidget* LoginUI;

	UPROPERTY(EditAnywhere)
	class UJS_CreateRoomWidget* CR_WidgetUI;

	UPROPERTY(EditAnywhere)
	class UJS_RoomWidget* R_UI;

    // index 요청 시 send만 할 경우 처리를 위한 변수
    bool bOnlyIndexSend = false;
    
	// UI세팅 함수
    bool bShowUI = false;

    // UI 초기화
    void InitializeUIWidgets();

    //LoginUI
    void ShowLoginUI();
    void HideLoginUI();

    //CreateRoomUI
    void ShowCreateRoomUI();
    void HideCreateRoomUI();

    //RoomUI
    void ShowRoomUI();
    void HideRoomUI();
    void PlayUIAnimation();

    

    //Mouse Interaction
    void OnMouseClick();

    //Mouse Hover
    void OnMouseHover(AActor* HoveredActor);

    void OnMouseHoverEnd(AActor* HoveredActor);

    AActor* CurrentHoveredActor = nullptr;

};
