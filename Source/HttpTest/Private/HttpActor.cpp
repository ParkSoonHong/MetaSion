// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpActor.h"
#include "Blueprint/UserWidget.h"
#include "HttpWidget.h"
#include "JS_CreateRoomWidget.h"

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
	CR_WidgetUI = Cast<UJS_CreateRoomWidget>(CreateWidget(GetWorld(), HttpUIFactory));
	if (CR_WidgetUI)
	{
		CR_WidgetUI->AddToViewport();
		/*CR_WidgetUI->SetHttpActor(this);*/
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

// image 처리
//void AHttpActor::ReqGetWebImage(FString url)
//{
//	FHttpModule& httpModule = FHttpModule::Get();
//	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
//
//	// 요청할 정보를 설정
//	req->SetURL(url);
//	req->SetVerb(TEXT("GET"));
//	req->SetHeader(TEXT("content-type"), TEXT("image/jpeg"));
//
//	// 응답받을 함수를 연결
//	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImage);
//	// 서버에 요청
//
//	req->ProcessRequest();
//}
//
//void AHttpActor::OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
//{
//	if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
//	{
//		TArray<uint8> data = Response->GetContent();
//		FString fileServerURL = "http://mtvs.helloworldlabs.kr:7771/api/byte";
//		ReqGetWebImageToServer(fileServerURL, data);
//	}
//	else {
//		// 실패
//		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImage Failed..."));
//	}
//}
//
//void AHttpActor::ReqGetWebImageToServer(FString url, TArray<uint8> resource)
//{
//	FHttpModule& httpModule = FHttpModule::Get();
//	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
//
//	// 요청할 정보를 설정
//	req->SetURL(url);
//	req->SetVerb(TEXT("POST"));
//	req->SetHeader(TEXT("content-type"), TEXT("application/octet-stream"));
//	req->SetContent(resource);
//
//	// 응답받을 함수를 연결
//	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImageFromServer);
//	// 서버에 요청
//
//	req->ProcessRequest();
//
//}
//
//void AHttpActor::OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
//{
//	if (bConnectedSuccessfully)
//	{
//		TArray<uint8> data = Response->GetContent();
//		FString imagePath = FPaths::ProjectPersistentDownloadDir() + "/Cat.jpg";
//		FFileHelper::SaveArrayToFile(data, *imagePath);
//		UTexture2D* realTexture = FImageUtils::ImportBufferAsTexture2D(data);
//		HttpUI->SetWebImage(realTexture);
//	}
//	else {
//		// 실패
//		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImageFromServer Failed..."));
//	}
//}
