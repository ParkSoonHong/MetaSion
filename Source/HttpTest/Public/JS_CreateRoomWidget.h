// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_CreateRoomWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UJS_CreateRoomWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    virtual void NativeConstruct() override;
    
    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_Yes;
    
    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_No;
    
    UPROPERTY(meta=(BindWidget))
    class UEditableText* ED_RoomName;

    UPROPERTY(meta=(BindWidget))
    class UButton* btn_CreateRoom_Private;
    
    UPROPERTY(meta=(BindWidget))
    class UWidgetSwitcher* CR_WidgetSwitcher;
    
    UPROPERTY(meta=(BindWidget))
    class UVerticalBox* VB_CreateRoom_Q1;
    
    UPROPERTY(meta=(BindWidget))
    class UVerticalBox* VB_CreateRoom_Q2;
    
    UPROPERTY(meta=(BindWidget))
    class UBorder* CreateRoomCompelete;
    
    UPROPERTY(BlueprintReadWrite)
    bool bPrivate = false;

    FTimerHandle Handler;
    
    UFUNCTION(BlueprintCallable)
    void SwitchToWidget(int32 index);
    
    UFUNCTION()
    void CreateRoomChooseYes();
    
    UFUNCTION()
    void CreateRoomChooseNo();
    
    UFUNCTION()
    void SetPrivate();
    
    UFUNCTION(BlueprintCallable)
    void ShowUIForLimitedTime(float DurationInSeconds);
    
    UFUNCTION()
    void HideUI();
};