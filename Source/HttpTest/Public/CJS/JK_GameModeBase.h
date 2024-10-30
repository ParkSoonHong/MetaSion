// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JK_GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API AJK_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayerController")
	TSubclassOf<class AJS_RoomController> RoomControllerClass;
	
};
