// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpActor.h"
#include "HttpWidget.h"
#include "HttpModule.h"
#include "JsonParseLib.h"
#include "Engine/Texture2D.h"
#include "ImageUtils.h"
#include "JsonObjectConverter.h"

// Sets default values
AHttpActor::AHttpActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHttpActor::BeginPlay()
{
	Super::BeginPlay();

	// UI를 생성해서 기억하고싶다.
	HttpUI = Cast<UHttpWidget>(CreateWidget(GetWorld(), HttpUIFactory));
	if (HttpUI)
	{
		HttpUI->AddToViewport();
		HttpUI->SetHttpActor(this);
	}

	auto* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
	pc->SetInputMode(FInputModeUIOnly());
}

// Called every frame
void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//Login -----------------------------------------------------------
void AHttpActor::ReqPostTest(FString url, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청할 정보를 설정
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostTest);
	// 서버에 요청
	req->ProcessRequest();
}
void AHttpActor::OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		// 성공
		FString result = Response->GetContentAsString();
		// Json을 구조체로 변경
		auto login = Login_Convert_JsonToStruct(result);
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest() OK. Code : %d  Str : %s"), Response->GetResponseCode(), *result);

		// 필요한 정보만 뽑아서 화면에 출력하고싶다.
		HttpUI->SetTextLog(result);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
	}
}
FString AHttpActor::Login_Convert_StructToJson(FString userID, FString userPW)
{
	//사용할 구조체 변수로 선언
	FLoginStr loginStruct;
	// 입력 받은 ID와 PW를 구조체에 입력
	loginStruct.UserID = userID; 
	loginStruct.UserPW = userPW;

	//Json 문자열을 저장할 변수
	FString JsonString;

	//구조체를 JSON 문자열로 변환
	FJsonObjectConverter::UStructToJsonObjectString(loginStruct, JsonString, 0, 0);
	//완성된 Json 반환
	return JsonString;
}
FLoginStr AHttpActor::Login_Convert_JsonToStruct(const FString& JsonString)
{
	FLoginStr loginJson;
	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &loginJson, 0, 0);
	//변환된 구조체를 반환
	return loginJson;
}
//Login End-----------------------------------------------------------

void AHttpActor::ReqGetWebImage(FString url)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청할 정보를 설정
	req->SetURL(url);
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("content-type"), TEXT("image/jpeg"));

	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImage);
	// 서버에 요청

	req->ProcessRequest();
}

void AHttpActor::OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		TArray<uint8> data = Response->GetContent();
		FString fileServerURL = "http://mtvs.helloworldlabs.kr:7771/api/byte";
		ReqGetWebImageToServer(fileServerURL, data);
	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImage Failed..."));
	}
}

void AHttpActor::ReqGetWebImageToServer(FString url, TArray<uint8> resource)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청할 정보를 설정
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/octet-stream"));
	req->SetContent(resource);

	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImageFromServer);
	// 서버에 요청

	req->ProcessRequest();

}

void AHttpActor::OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		TArray<uint8> data = Response->GetContent();
		FString imagePath = FPaths::ProjectPersistentDownloadDir() + "/Cat.jpg";
		FFileHelper::SaveArrayToFile(data, *imagePath);
		UTexture2D* realTexture = FImageUtils::ImportBufferAsTexture2D(data);
		HttpUI->SetWebImage(realTexture);
	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImageFromServer Failed..."));
	}
}
