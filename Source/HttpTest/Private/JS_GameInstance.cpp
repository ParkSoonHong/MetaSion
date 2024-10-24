// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_GameInstance.h"
#include "HttpModule.h"
#include "JsonParseLib.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "KGW/KGW_ChoiceSaveBF.h"

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
void UJS_GameInstance::ReqPostChoice(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &UJS_GameInstance::OnResPostChoice);
    // 서버에 요청

    req->ProcessRequest();


}
void UJS_GameInstance::OnResPostChoice(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (bConnectedSuccessfully && Response.IsValid())
    {
        // 성공적으로 응답을 받았을 때
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("POST Response: %s"), *ResponseContent);

        // JSON 응답에서 RGB 값들을 파싱
        TArray<FColorData> Colors = UKGW_ChoiceSaveBF::ParseJsonToRGB(ResponseContent);

        //         // 파싱된 RGB 값이 있을 경우 처리
        //         if (Colors.Num() > 0)
        //         {
        //             // 첫 번째 RGB 값 사용 예시 (머티리얼에 적용)
        //             FColorData FirstColor = Colors[0];
        // 
        //             UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CharacterMaterial, this);
        //             if (DynamicMaterial)
        //             {
        //                 DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(FirstColor.R, FirstColor.G, FirstColor.B));
        //                 CharacterMesh->SetMaterial(0, DynamicMaterial);
        //             }
        // 
        //             // 추가 처리 가능
        //         }
    }
    else
    {
        // 요청이 실패했을 때
        UE_LOG(LogTemp, Warning, TEXT("POST Request Failed"));
    }
}
