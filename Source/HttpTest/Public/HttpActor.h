// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpActor.generated.h"

USTRUCT(BlueprintType) 
struct FLoginStr
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString UserID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString UserPW;

	//기본 생성자
	FLoginStr() : UserID(TEXT("")), UserPW(TEXT("")) {}
};

UCLASS()
class HTTPTEST_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 인터페이스를 위한 UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HttpUIFactory;

	UPROPERTY()
	class UHttpWidget* HttpUI;


	//Login -----------------------------------------------------------
	// ButtonSendPost를 눌렀을 때 요청할 함수
	void ReqPostTest(FString url, FString json);
	// 응답받을 함수
	void OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//Struct To Json : Req
	FString Login_Convert_StructToJson(FString userID, FString userPW);
	//Json To Struct : Response
	FLoginStr Login_Convert_JsonToStruct(const FString& JsonString);

	//Login End-----------------------------------------------------------

	// ButtonGetWebImage를 눌렀을 때 요청할 함수
	void ReqGetWebImage(FString url);
	// 응답받을 함수
	void OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// 웹이미지를 백엔드 서버로 송신
	void ReqGetWebImageToServer(FString url, TArray<uint8> resource);
	// 웹이미지를 백엔드 서버에서 수신
	void OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
};
