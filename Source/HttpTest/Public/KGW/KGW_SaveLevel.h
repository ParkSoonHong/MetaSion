// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "KGW_SaveLevel.generated.h"

/**
 *
 */
UCLASS()
class HTTPTEST_API UKGW_SaveLevel : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "UltraSky")
	float TimeOfDay;

	UPROPERTY(VisibleAnywhere, Category = "UltraWheater")
	float CloudCoverage;

	UPROPERTY(VisibleAnywhere, Category = "UltraWheater")
	float Fog;

	UPROPERTY(VisibleAnywhere, Category = "UltraWheater")
	float Rain;
	
	UPROPERTY(VisibleAnywhere, Category = "UltraWheater")
	float Snow;
	UPROPERTY(VisibleAnywhere, Category = "UltraWheater")
	float Dust;
	
	UPROPERTY(VisibleAnywhere, Category = "UltraWheater")
	float WindIntensity;
	
	UPROPERTY(VisibleAnywhere, Category = "UltraWheater")
	float Thunder;
	
	UPROPERTY(VisibleAnywhere, Category = "Ultra Weather")
	FString WeatherPreset;
	
	UPROPERTY(VisibleAnywhere, Category = "UltraWheater")

	bool bAurora;
	UFUNCTION(BlueprintCallable) 
	static bool SaveGameData(AActor* SkyActor, AActor* WeatherActor);
	UFUNCTION(BlueprintCallable) 
	static void LoadGameData(AActor* SkyActor, AActor* WeatherActor);


};

