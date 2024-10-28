// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WBP_Image.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UWBP_Image : public UUserWidget
{
	GENERATED_BODY()

public:	
UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
UWidgetAnimation* ShowImage;
	

	UFUNCTION(BlueprintCallable)
    void PlayShowImageAnimation();
};
