// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JS_RoomWidget.h"
#include "JS_TestWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UJS_TestWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_TestButton;

	UFUNCTION()
	void TestUI();

};
