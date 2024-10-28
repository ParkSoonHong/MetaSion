// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CJS_SessionWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UCJS_SessionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Create;

	UFUNCTION()
	void OnClickCreateSession();
};
