// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JS_GameInstance.generated.h"

//DECLARE_DELEGATE_OneParam(DelLogin)

/**
 * 
 */
UCLASS()
class HTTPTEST_API UJS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
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


	void ReqPostChoice(FString url, FString json);

	void OnResPostChoice(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	/*DelLogin LoginDelegate;
	
	void BindDelegates();
	void LoginDelegateFunction();*/
};
