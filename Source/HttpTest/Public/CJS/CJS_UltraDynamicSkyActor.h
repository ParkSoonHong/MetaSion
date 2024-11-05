// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CJS_UltraDynamicSkyActor.generated.h"

UCLASS()
class HTTPTEST_API ACJS_UltraDynamicSkyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACJS_UltraDynamicSkyActor();

	// 소멸자 (예외 사양 없음)
	virtual ~ACJS_UltraDynamicSkyActor() override; // 예외 사양 없이 정의

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SetModifyAuroraColors();
	FLinearColor Color1;
	FLinearColor Color2;
	FLinearColor Color3;
	UFUNCTION(BlueprintCallable, Category = "Aurora")
	FLinearColor GetAuroraColor1();
	UFUNCTION(BlueprintCallable, Category = "Aurora")
	FLinearColor GetAuroraColor2();
	UFUNCTION(BlueprintCallable, Category = "Aurora")
	FLinearColor GetAurorqaColor3();

	UFUNCTION(BlueprintImplementableEvent, Category = "Aurora")
	void UpdateAuroraColors(FLinearColor AuroraColor1, FLinearColor AuroraColor2, FLinearColor AuroraColor3);

};
