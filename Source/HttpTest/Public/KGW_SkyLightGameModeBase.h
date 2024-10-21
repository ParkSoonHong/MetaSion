// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KGW_SkyLightGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API AKGW_SkyLightGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lighting") 
class ADirectionalLight* DirectionalLight;

UFUNCTION(BlueprintCallable, Category = "Weather")
void AdjustSunBrightness(float SliderValue);

	
	
};
