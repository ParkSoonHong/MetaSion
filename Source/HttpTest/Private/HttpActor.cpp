// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpActor.h"
#include "Blueprint/UserWidget.h"
#include "HttpWidget.h"
#include "JS_CreateRoomWidget.h"
#include "JS_RoomWidget.h"
#include "HttpModule.h"
#include "JsonParseLib.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "JS_RoomController.h"
#include "JS_TestWidget.h"
#include "Kismet/GameplayStatics.h"


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

    pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

    FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	/*  if (LevelName == TEXT("TestLevel")) {
		  TestWidgetUI = CreateWidget<UJS_TestWidget>(GetWorld()->GetFirstPlayerController(), Test_Factory);
		  if (TestWidgetUI) {
			  TestWidgetUI->AddToViewport();
		  }
	  }*/
}

// Called every frame
void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Login -------------------------------------------------------------
void AHttpActor::LoginReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::LoginResPost);
    // 서버에 요청
    req->ProcessRequest();
}

void AHttpActor::LoginResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
        UJsonParseLib::Login_Convert_JsonToStruct(result);
        
        UE_LOG(LogTemp, Log, TEXT("Login Post Request Success: %s"), *result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
    if (pc) {
        pc->HideLoginUI();
        //이쪽에 다음 UI로 넘어가는 거 구현
        ShowQuestionUI();
    }
}
//Login End-------------------------------------------------------------

//Sign UP -------------------------------------------------------------
void AHttpActor::SignUpReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::SignUpResPost);
    // 서버에 요청
    req->ProcessRequest();
}

void AHttpActor::SignUpResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
        UJsonParseLib::SignUp_Convert_JsonToStruct(result);
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}

//Sign UP End -------------------------------------------------------------

//User -------------------------------------------------------------
void AHttpActor::UserReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::UserResPost);
    // 서버에 요청
    req->ProcessRequest();
}

void AHttpActor::UserResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
        UJsonParseLib::UserLike_Convert_JsonToStruct(result);
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//User End-------------------------------------------------------------

//User Like-------------------------------------------------------------
void AHttpActor::UserLikeReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::UserLikeResPost);
    // 서버에 요청
    req->ProcessRequest();
}

void AHttpActor::UserLikeResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
        UJsonParseLib::UserLike_Convert_JsonToStruct(result);
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//User Like End-------------------------------------------------------------

//ChangeIndex -------------------------------------------------------------
void AHttpActor::ChangeIndexReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ChangeIndexResPost);

    // 서버에 요청
    req->ProcessRequest();
}

void AHttpActor::ChangeIndexResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
        FChangeIndex ChangerIndexData = UJsonParseLib::ChangeIndex_Convert_JsonToStruct(result);

        // 서버에서 반환된 데이터를 로그로 출력
        UE_LOG(LogTemp, Log, TEXT("Response Received: userId = %s, index = %d"), *ChangerIndexData.userId, ChangerIndexData.index);

        // 현재 레벨에서 RoomWidget을 찾음
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            AJS_RoomController* RoomController = Cast<AJS_RoomController>(PlayerController);
            if (RoomController && RoomController->R_UI)
            {
                // 받은 Index 값으로 UI를 업데이트
                RoomController->R_UI->SetIndex(ChangerIndexData.index, 100);
            }
        }

    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//ChangeIndex End-------------------------------------------------------------

//MyRoomInfo -------------------------------------------------------------
void AHttpActor::MyRoomInfoReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::MyRoomInfoResPost);

    // 서버에 요청
    req->ProcessRequest();
}

void AHttpActor::MyRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
        FMyRoomInfo MyRoomInfoData = UJsonParseLib::MyRoomInfo_Convert_JsonToStruct(result);

        // 서버에서 반환된 데이터를 로그로 출력
        UE_LOG(LogTemp, Log, TEXT("Response Received: RoomName = %s, room_pp = %s"),
            *MyRoomInfoData.RoomName,
            MyRoomInfoData.room_pp ? TEXT("true") : TEXT("false"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
//MyRoomInfo End-------------------------------------------------------------

//MyCreateRoomInfo -------------------------------------------------------------
void AHttpActor::MyCreateRoomInfoReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::MyCreateRoomInfoResPost);

    // 서버에 요청
    req->ProcessRequest();
}

void AHttpActor::MyCreateRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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
        FMyCreateRoomInfo MyCreateRoomInfoData = UJsonParseLib::FMyCreateRoomInfo_Convert_JsonToStruct(result);

        // 서버에서 반환된 데이터를 로그로 출력
        UE_LOG(LogTemp, Log, TEXT("Response Received: userId = %s,RoomNum = %d, RoomName = %s"),
            *MyCreateRoomInfoData.UserId,
             MyCreateRoomInfoData.RoomNum,
            *MyCreateRoomInfoData.RoomName);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    }
}
void AHttpActor::ReqPostChoice(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // 요청할 정보를 설정
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // 응답받을 함수를 연결
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostChoice);
    // 서버에 요청

    req->ProcessRequest();

}
void AHttpActor::OnResPostChoice(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (bConnectedSuccessfully && Response.IsValid())
    {
        // 성공적으로 응답을 받았을 때
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("POST Response: %s"), *ResponseContent);
        StoredJsonResponse = ResponseContent;
        UE_LOG(LogTemp, Warning, TEXT("Stored JSON Response: %s"), *StoredJsonResponse);
        // JSON 응답에서 RGB 값들을 파싱
//         ParsedColors = UKGW_ChoiceSaveBF::ParseJsonToRGB(ResponseContent);
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
        //         if (ParsedColors.Num() > 0)
        //         {
        //             // 파싱된 데이터가 있을 경우 첫 번째 값을 로그로 출력 (디버깅용)
        //             FColorData FirstColor = ParsedColors[0];
        //             UE_LOG(LogTemp, Log, TEXT("Parsed Color - R: %f, G: %f, B: %f"), FirstColor.R, FirstColor.G, FirstColor.B);
        //         }
        //         else
        //         {
        //             UE_LOG(LogTemp, Warning, TEXT("No valid RGB data found in response."));
        //         }
        }
        else
        {
            // 요청이 실패했을 때
            UE_LOG(LogTemp, Warning, TEXT("POST Request Failed"));
        }

}
void AHttpActor::ShowQuestionUI()
{
    // MyWidgetClass가 유효한지 확인
    if (QuestionUIFactory && !QuestionUI)
    {
        // UI 위젯 인스턴스를 생성
        QuestionUI = CreateWidget<UUserWidget>(GetWorld(), QuestionUIFactory);

        if (QuestionUI)
        {
            // 화면에 추가
            QuestionUI->AddToViewport();
        }
    }
}
FString AHttpActor::StoreJsonResponse()
{
    FString JsonString = StoredJsonResponsetest;

    return JsonString;
}
//MyCreateRoomInfo End-------------------------------------------------------------


