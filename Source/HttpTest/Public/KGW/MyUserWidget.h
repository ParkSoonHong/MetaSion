// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY(EditDefaultsOnly, meta = (BindWidgetAnim), Transient, Category = MySettings)
	class UWidgetAnimation* ShowImage;

	
};
