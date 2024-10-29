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
#include "CJS/CJS_BallPlayer.h"
#include "CJS/SessionGameInstance.h"


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


    // SessionGameInstance 할당
    SessionGI = Cast<USessionGameInstance>(GetGameInstance());
    if (SessionGI)
    {
        UE_LOG(LogTemp, Warning, TEXT("USessionGameInstance is set"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("USessionGameInstance is not set"));
    }
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

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::LoginResPost);
    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::LoginResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::LoginResPost()"));

    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }

    //FString result1 = Response->GetContentAsString();
    //UE_LOG(LogTemp, Warning, TEXT("AHttpActor::LoginResPost() result: %s"), *result1);
   //
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    UE_LOG(LogTemp, Warning, TEXT("Response code : %d"), Response->GetResponseCode());
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        //UJsonParseLib::Login_Convert_JsonToStruct(result);
        FLogin LoginData = UJsonParseLib::Login_Convert_JsonToStruct(result); // <---- 추가
        FString uesrid = LoginData.userId;
        if (SessionGI)
        {
            SessionGI->InitSessionName(uesrid);
            UE_LOG(LogTemp, Warning, TEXT("Session Name set to UserId: %s"), *SessionGI->MySessionName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("AHttpActor::LoginResPost():: No SessionGM"));
        }

        
        UE_LOG(LogTemp, Warning, TEXT("Login Post Request Success: %s"), *result);
        if (pc) {
            pc->HideLoginUI();
            //���ʿ� ���� UI�� �Ѿ�� �� ����
            ShowQuestionUI();
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("LoginResPost() Failed..."));
    }
}
//Login End-------------------------------------------------------------

//Sign UP -------------------------------------------------------------
void AHttpActor::SignUpReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::SignUpResPost);
    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::SignUpResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
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

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::UserResPost);
    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::UserResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
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

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::UserLikeResPost);
    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::UserLikeResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
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

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ChangeIndexResPost);

    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::ChangeIndexResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        FChangeIndex ChangerIndexData = UJsonParseLib::ChangeIndex_Convert_JsonToStruct(result);

        // �������� ��ȯ�� �����͸� �α׷� ���
        UE_LOG(LogTemp, Log, TEXT("Response Received: userId = %s, index = %d"), *ChangerIndexData.userId, ChangerIndexData.index);

        // ���� �������� RoomWidget�� ã��
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            AJS_RoomController* RoomController = Cast<AJS_RoomController>(PlayerController);
            if (RoomController && RoomController->R_UI)
            {
                // ���� Index ������ UI�� ������Ʈ
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

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::MyRoomInfoResPost);

    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::MyRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    //if (!Response.IsValid())
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
    //    return;
    //}
    //// ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    //if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    //{
    //    // ������ ���ڿ��� ��������
    //    FString result = Response->GetContentAsString();
    //    FMyRoomInfo MyRoomInfoData = UJsonParseLib::MyRoomInfo_Convert_JsonToStruct(result);

    //    // �������� ��ȯ�� �����͸� �α׷� ���
    //    UE_LOG(LogTemp, Log, TEXT("Response Received: RoomName = %s, room_pp = %s"),
    //        *MyRoomInfoData.RoomName,
    //        MyRoomInfoData.room_pp ? TEXT("true") : TEXT("false"));
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
    //}
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }

    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString JsonResponse = Response->GetContentAsString();
        RoomData = UJsonParseLib::RoomData_Convert_JsonToStruct(JsonResponse);

        UE_LOG(LogTemp, Warning, TEXT("RoomData initialized: %s"), *RoomData.userMusic);

        // RoomData가 초기화되었음을 알리기 위해 델리게이트 호출
        OnRoomDataInitialized.Broadcast(RoomData);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Request Failed: %d"), Response->GetResponseCode());
    }
}
//MyRoomInfo End-------------------------------------------------------------

//MyCreateRoomInfo -------------------------------------------------------------
void AHttpActor::MyCreateRoomInfoReqPost(FString url, FString json)
{
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::MyCreateRoomInfoResPost);

    // ������ ��û
    req->ProcessRequest();
}

void AHttpActor::MyCreateRoomInfoResPost(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }
    // ��û�� ���������� �Ϸ�Ǿ����� Ȯ��
    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // ������ ���ڿ��� ��������
        FString result = Response->GetContentAsString();
        FMyCreateRoomInfo MyCreateRoomInfoData = UJsonParseLib::FMyCreateRoomInfo_Convert_JsonToStruct(result);

        // �������� ��ȯ�� �����͸� �α׷� ���
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

    // ��û�� ������ ����
    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);

    // ������� �Լ��� ����
    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostChoice);
    // ������ ��û

    req->ProcessRequest();

}
void AHttpActor::OnResPostChoice(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (bConnectedSuccessfully && Response.IsValid())
    {
        // ���������� ������ �޾��� ��
        FString ResponseContent = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("POST Response: %s"), *ResponseContent);
        //StoredJsonResponse = ResponseContent;
        UE_LOG(LogTemp, Warning, TEXT("Stored JSON Response: %s"), *StoredJsonResponse);
        StoredJsonResponse = StoredJsonResponsetest;    // <-------------------------------------------- 여기부터 수정
        if (SessionGI)
        {
            UE_LOG(LogTemp, Warning, TEXT("SessionGM is OK"));
            SessionGI->SetNetInfoCharacterTOLobby(StoredJsonResponse);
            SessionGI->FindSessions();
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("SessionGM is NULL"));
        }

        


        // JSON ���信�� RGB ������ �Ľ�
//         ParsedColors = UKGW_ChoiceSaveBF::ParseJsonToRGB(ResponseContent);
        //         // �Ľ̵� RGB ���� ���� ��� ó��
        //         if (Colors.Num() > 0)
        //         {
        //             // ù ��° RGB �� ��� ���� (��Ƽ���� ����)
        //             FColorData FirstColor = Colors[0];
        // 
        //             UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CharacterMaterial, this);
        //             if (DynamicMaterial)
        //             {
        //                 DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(FirstColor.R, FirstColor.G, FirstColor.B));
        //                 CharacterMesh->SetMaterial(0, DynamicMaterial);
        //             }
        // 
        //             // �߰� ó�� ����
        //         }
        //         if (ParsedColors.Num() > 0)
        //         {
        //             // �Ľ̵� �����Ͱ� ���� ��� ù ��° ���� �α׷� ��� (������)
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
            // ��û�� �������� ��
            UE_LOG(LogTemp, Warning, TEXT("POST Request Failed"));
        }

}
void AHttpActor::ShowQuestionUI()
{
    // MyWidgetClass�� ��ȿ���� Ȯ��
    if (QuestionUIFactory && !QuestionUI)
    {
        // UI ���� �ν��Ͻ��� ����
        QuestionUI = CreateWidget<UUserWidget>(GetWorld(), QuestionUIFactory);

        if (QuestionUI)
        {
            // ȭ�鿡 �߰�
            QuestionUI->AddToViewport();
        }
    }
}
FString AHttpActor::StoreJsonResponse()
{
    FString JsonString = StoredJsonResponsetest;

    return JsonString;
}

void AHttpActor::ReqPostClickMultiRoom(FString url, FString json)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ReqPostClickMultiRoom()"));
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);
    req->SetTimeout(60.0f); // 타임아웃 설정

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostClickMultiRoom);

    if (req->ProcessRequest())
    {
        UE_LOG(LogTemp, Warning, TEXT("Http Request processed successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Http Request failed to process"));
    }
}

void AHttpActor::OnResPostClickMultiRoom(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::OnResPostClickMultiRoom()"));

    /*if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }

    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        FString result = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("Post Request Success: %s"), *result);
    }
    else
    {
        int32 ResponseCode = Response->GetResponseCode();
        UE_LOG(LogTemp, Warning, TEXT("OnResPostClickMultiRoom() Failed... Response Code: %d, Connected Successfully: %s"),
            ResponseCode, bConnectedSuccessfully ? TEXT("True") : TEXT("False"));
    }*/

    if (bConnectedSuccessfully)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPosLogin OK... %d"), Response->GetResponseCode());

        int32 res = Response->GetResponseCode();
        /*if (res == 200)
        {*/
            UE_LOG(LogTemp, Warning, TEXT("Response ... OK!! "));
            FString str = Response->GetContentAsString();
            UE_LOG(LogTemp, Warning, TEXT(" %s"), *str);

            // 플레이어 캐스팅 및 RequestMoveMultiRoom 호출
            if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
            {
                ACJS_BallPlayer* player = Cast<ACJS_BallPlayer>(PlayerController->GetPawn());
                if (player)
                {
                    player->RequestMoveMultiRoom(PlayerController);
                    UE_LOG(LogTemp, Warning, TEXT("RequestMoveMultiRoom called successfully."));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to cast PlayerController's Pawn to ACJS_BallPlayer."));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to get PlayerController."));
            }
       /* }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Response ... not OK!! "));
        }*/
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed...%d"), Response->GetResponseCode());
    }
}

//JS ReWrite 내방 통신 추가 부분
void AHttpActor::ReqPostClickMyRoom(FString url, FString json)
{
    UE_LOG(LogTemp, Warning, TEXT("AHttpActor::ReqPostClickMyRoom()"));
    FHttpModule& httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

    req->SetURL(url);
    req->SetVerb(TEXT("POST"));
    req->SetHeader(TEXT("content-type"), TEXT("application/json"));
    req->SetContentAsString(json);
    req->SetTimeout(60.0f); // 타임아웃 설정

    req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResPostClickMyRoom);

    if (req->ProcessRequest())
    {
        UE_LOG(LogTemp, Warning, TEXT("Http Request processed successfully"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Http Request failed to process"));
    }
}

void AHttpActor::OnResPostClickMyRoom(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
    if (!Response.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Response"));
        return;
    }

    if (bConnectedSuccessfully && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
    {
        // 응답에서 JSON 문자열 얻기
        FString JsonResponse = Response->GetContentAsString();
        UE_LOG(LogTemp, Warning, TEXT("Response JSON: %s"), *JsonResponse);
       
        // FRoomData 구조체로 변환
        RoomData = UJsonParseLib::RoomData_Convert_JsonToStruct(JsonResponse);
        SessionGI->RoomMusicData = RoomData.userMusic;
		UE_LOG(LogTemp, Warning, TEXT("RecommendedMusic: %s"), *SessionGI->RoomMusicData);

        //MyRoom으로 이동
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController) {
            PlayerController->ClientTravel("/Game/Main/Maps/Main_Room", ETravelType::TRAVEL_Absolute);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Request Failed: %d"), Response->GetResponseCode());
    }
}
//Getter 함수
FRoomData AHttpActor::GetRoomData() const
{
    FString Rdata = RoomData.userMusic;
    UE_LOG(LogTemp, Warning, TEXT("RoomData music : %s"), *Rdata);
    return RoomData;
}
//MyCreateRoomInfo End-------------------------------------------------------------


