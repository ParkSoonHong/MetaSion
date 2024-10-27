// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpFwd.h"
#include "HttpActor.generated.h"

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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UHttpWidget> Sign_Factory;

	UPROPERTY(EditAnywhere)
	class UHttpWidget* HttpWidgetUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UJS_TestWidget> Test_Factory;

	UPROPERTY(EditAnywhere)
	class UJS_TestWidget* TestWidgetUI;

	UPROPERTY(BlueprintReadWrite, Category = "JSON Data")
	FString StoredJsonResponse;

	FString StoredJsonResponsetest = TEXT("{\"UserId\":\"1\",\"R\":1.0,\"G\":0.9225690792809692,\"B\":0.4,\"SimilarUsers\":[{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sunny World\"},{\"UserId\":\"user_8\",\"EmotionScore\":82.0,\"RoomName\":\"Sol World\"},{\"UserId\":\"abc11\",\"EmotionScore\":81.0,\"RoomName\":\"KW World\"}],\"OppositeUsers\":[{\"UserId\":\"user_1\",\"EmotionScore\":283.5,\"RoomName\":\"JW World\"},{\"UserId\":\"user_3\",\"EmotionScore\":321.0,\"RoomName\":\"DL World\"}]}");

	// Login
	void LoginReqPost(FString url, FString json);
	void LoginResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// Sign_Up
	void SignUpReqPost(FString url, FString json);
	void SignUpResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// User
	void UserReqPost(FString url, FString json);
	void UserResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// User_like
	void UserLikeReqPost(FString url, FString json);
	void UserLikeResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	
	//ChangeIndex
	void ChangeIndexReqPost(FString url, FString json);
	void ChangeIndexResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//MyRoomInfo
	void MyRoomInfoReqPost(FString url, FString json);
	void MyRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	//MyCreateRoomInfo
	void MyCreateRoomInfoReqPost(FString url, FString json);
	void MyCreateRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ReqPostChoice(FString url, FString json);
	void OnResPostChoice(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	UFUNCTION(BlueprintCallable, Category = "JSON")
    FString StoreJsonResponse();


	//FString ServerURL = "https://webhook.site/a0cbc113-e54b-4c1b-a92a-acb925a13d24";
	FString ServerURL = "https://jsonplaceholder.typicode.com/posts";
};
