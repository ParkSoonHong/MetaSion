// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_GameInstance.h"
#include "HttpModule.h"
#include "JsonParseLib.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

//Login -------------------------------------------------------------
void UJS_GameInstance::LoginReqPost(FString url, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청할 정보를 설정
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::LoginResPost);
	// 서버에 요청
	req->ProcessRequest();
}
void UJS_GameInstance::LoginResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // 요청이 성공적으로 완료되었는지 확인
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // 응답을 문자열로 가져오기
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
        UJsonParseLib::Login_Convert_JsonToStruct(result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//Login End-------------------------------------------------------------

//Sign UP -------------------------------------------------------------
void UJS_GameInstance::SignUpReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::SignUpResPost);
    // 서버에 요청
    req->ProcessRequest();
}

void UJS_GameInstance::SignUpResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // 요청이 성공적으로 완료되었는지 확인
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // 응답을 문자열로 가져오기
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
        UJsonParseLib::SignUp_Convert_JsonToStruct(result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}

//Sign UP End -------------------------------------------------------------

//User -------------------------------------------------------------
void UJS_GameInstance::UserReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::UserResPost);
    // 서버에 요청
    req->ProcessRequest();
}

void UJS_GameInstance::UserResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // 요청이 성공적으로 완료되었는지 확인
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // 응답을 문자열로 가져오기
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
        UJsonParseLib::UserLike_Convert_JsonToStruct(result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//User End-------------------------------------------------------------

//User Like-------------------------------------------------------------
void UJS_GameInstance::UserLikeReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::UserLikeResPost);
    // 서버에 요청
    req->ProcessRequest();
}

void UJS_GameInstance::UserLikeResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // 요청이 성공적으로 완료되었는지 확인
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // 응답을 문자열로 가져오기
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
        UJsonParseLib::UserLike_Convert_JsonToStruct(result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//User Like End-------------------------------------------------------------


//void UJS_GameInstance::BindDelegates()
//{
//    LoginDelegate.BindUObject(this, &UJS_GameInstance::LoginDelegateFunction);
//
//    LoginDelegate.Execute();
//}