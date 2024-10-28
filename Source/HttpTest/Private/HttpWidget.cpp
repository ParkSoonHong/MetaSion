// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpWidget.h"
#include "Components/Button.h"
#include "HttpActor.h"
#include "Components/TextBlock.h"
#include "Components/EditableText.h"
#include "JsonParseLib.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"
#include "JsonObjectConverter.h"
#include "Kismet/GameplayStatics.h"
#include "JS_RoomController.h"

void UHttpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//login
	btn_Login->OnClicked.AddDynamic(this, &UHttpWidget::SendLoginData);
	//sign_up
	btn_SignUp->OnClicked.AddDynamic(this, &UHttpWidget::SendSignUpData);
	////user
	//btn_User->OnClicked.AddDynamic(this, &UHttpWidget::SendUserData);
	////user_like
	//btn_UserLike->OnClicked.AddDynamic(this, &UHttpWidget::SendUserLikeData);

	pc = Cast<AJS_RoomController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
}

//Login
void UHttpWidget::SendLoginData()
{
	// 구조체에 있는 변수에 UI 텍스트 값을 넣어줘야 함.
	FLogin loginData;
	loginData.userId = userId->GetText().ToString();
	loginData.userpass = userpass->GetText().ToString();

	FString json = UJsonParseLib::Login_Convert_StructToJson(loginData);
	//Login요청
	HttpActor->LoginReqPost(HttpActor->ServerURL, json);
}

void UHttpWidget::SendSignUpData()
{
	FSign_up SingupData;
	SingupData.userId = TEXT("123456");
	SingupData.userpass = TEXT("010010");

	//에디터블 textblock에서 가져오는 방식으로 진행
	FString json = UJsonParseLib::SignUp_Convert_StructToJson(SingupData);
	//Login요청
	HttpActor->SignUpReqPost(HttpActor->ServerURL, json);
}

//User
//void UHttpWidget::SendUserData()
//{
//	// 구조체에 있는 변수에 UI 텍스트 값을 넣어줘야 함.
//	FUser UserData;
//	UserData.userId = TEXT("121212");
//	UserData.LoginTime = FDateTime::Now();
//	UserData.MoodScore = 1;
//	UserData.EnergyScore = 2;
//	UserData.StabilityScore = 3;
//	UserData.WeatherChoice = TEXT("cloud");
//	UserData.MainAsset = TEXT("Wood");
//	UserData.SecondaryAsset = TEXT("Sky");
//	UserData.BackgroundColor = TEXT("Green");
//	UserData.FloorMaterial = TEXT("gress");
//	UserData.ClusterId = 4;
//	UserData.RecommendedRoomId = TEXT("RCOMID");
//	UserData.Feedback = TEXT("True");
//	UserData.FeedbackTime = FDateTime::Now();
//	UserData.UpdateTime = FDateTime::Now();
//	UserData.IsRecommended = true;
//	
//	FString json = UJsonParseLib::User_Convert_StructToJson(UserData);
//	//Login요청
//	//JS_gi->UserReqPost(ServerURL, json);
//}
//
////UserLike
//void UHttpWidget::SendUserLikeData()
//{
//	// 구조체에 있는 변수에 UI 텍스트 값을 넣어줘야 함.
//	FUser_like UserLikeData;
//
//	UserLikeData.FeedbackId = 1;
//	UserLikeData.UserId = TEXT("2323123");
//	UserLikeData.VisitedRoomId = TEXT("VisitedRoomID");
//	UserLikeData.VisitedUserId = TEXT("VisitedUserID");
//	UserLikeData.Feedback = TEXT("Feedback");
//	UserLikeData.FeedbackTime = FDateTime::Now();
//	UserLikeData.Mood = TEXT("red");
//	UserLikeData.Weather = TEXT("sunny");
//	UserLikeData.MainAsset = TEXT("sky");
//	UserLikeData.BackgroundColor = TEXT("yellow");
//	UserLikeData.FloorMaterial = TEXT("stone");
//	
//	FString json = UJsonParseLib::UserLike_Convert_StructToJson(UserLikeData);
//	//Login요청
//	//JS_gi->UserLikeReqPost(ServerURL, json);
//}
