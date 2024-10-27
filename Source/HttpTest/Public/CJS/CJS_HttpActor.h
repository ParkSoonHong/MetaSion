// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "CJS_HttpActor.generated.h"

UCLASS()
class HTTPTEST_API ACJS_HttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACJS_HttpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// 로비 -> 멀티방 입장 시 통신
	void ReqPostClickMultiRoom(FString url, FString json);
	void OnResPostClickMultiRoom(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);


};
