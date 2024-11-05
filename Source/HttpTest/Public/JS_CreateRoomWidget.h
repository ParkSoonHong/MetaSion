// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "JS_CreateRoomWidget.generated.h"
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class HTTPTEST_API UJS_CreateRoomWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere)
	class AJS_WidgetFunction* widgetActor;

	UPROPERTY(EditAnywhere)
	class AJS_RoomController* pc;

    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_Yes;
    
    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_No;

    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CompleteCreateRoom;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    class UEditableText* ED_RoomName;
    
    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_Private;

     UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_Public;
    
    UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* CR_WidgetSwitcher;

    UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* CR_WidgetSwitcherPP;
    
    UPROPERTY(meta=(BindWidget))
    class UVerticalBox* VB_CreateRoom_Q1;
    
    UPROPERTY(meta=(BindWidget))
    class UVerticalBox* VB_CreateRoom_Q2;
    
    UPROPERTY(meta=(BindWidget))
    class UBorder* CreateRoomCompelete;
    
    UPROPERTY(EditAnywhere)
	class AHttpActor* httpActor;

    UPROPERTY(BlueprintReadWrite)
    int32 bPrivate = 0;

    FTimerHandle Handler;

    UFUNCTION(BlueprintCallable)
    void SwitchToWidget(int32 index);
    
    UFUNCTION(BlueprintCallable)
    void SwitchToWidget_PP(int32 index);
    UFUNCTION()
    void CreateRoomChooseYes();
    
    UFUNCTION()
    void CreateRoomChooseNo();

    UFUNCTION()
    void CompleteCreateRoom();

    UFUNCTION()
    void SetPrivate();
    
    UFUNCTION(BlueprintCallable)
    void ShowUIForLimitedTime(float DurationInSeconds);
    
    UFUNCTION()
    void HideUI();

    UFUNCTION()
	void SendSetPrivateRoom(int32 Room_pp);

    //void ChangeNewline(const FText& NewText);
};