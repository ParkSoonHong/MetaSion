// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JS_WidgetFunction.h"
#include "CJS_JS_WidgetFunction.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API ACJS_JS_WidgetFunction : public AJS_WidgetFunction
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> WBP_Chat;
	UPROPERTY()
	class UCJS_ChatWidget* ChatUI;

	bool ChatUIShowing;
	UFUNCTION()
	void ToggleChatUIVisible();


};
