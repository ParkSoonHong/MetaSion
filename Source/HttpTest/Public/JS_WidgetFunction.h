// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "JS_WidgetFunction.generated.h"

UCLASS()
class HTTPTEST_API AJS_WidgetFunction : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_WidgetFunction();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* meshComp;

	 // À§Á¬ ÄÄÆ÷³ÍÆ® ¼±¾ð
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
class UWidgetComponent* WidgetComponent;
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
class UBillboardComponent* BillboardComp;


void PlayShowImageAnimation();

};
