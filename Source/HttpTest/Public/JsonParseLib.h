// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLib.generated.h"

USTRUCT(BlueprintType) 
struct FLogin //로그인
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Login/Struct")
	FString userpass;

	//기본 생성자
	FLogin() : 
			  userId(TEXT("")), 
			  userpass(TEXT(""))
	{}
};

USTRUCT(BlueprintType) 
struct FSign_up // 회원가입
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sign_up/Struct")
	FString userpass;

	//기본 생성자
	FSign_up() : 
				userId(TEXT("")), 
				userpass(TEXT("")) 
	{}
};

USTRUCT(BlueprintType)
struct FUser // 유저 테이블
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString userId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FDateTime LoginTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	int32 MoodScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	int32 EnergyScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	int32 StabilityScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString WeatherChoice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString MainAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString SecondaryAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString BackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString FloorMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	int32 ClusterId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString RecommendedRoomId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FString Feedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FDateTime FeedbackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	FDateTime UpdateTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "User/Struct")
	bool IsRecommended;

	// 기본 생성자
	FUser() 
		: userId(TEXT("")), 
		  LoginTime(FDateTime::Now()), 
		  MoodScore(0), 
		  EnergyScore(0), 
		  StabilityScore(0), 
		  WeatherChoice(TEXT("")), 
		  MainAsset(TEXT("")), 
		  SecondaryAsset(TEXT("")), 
		  BackgroundColor(TEXT("")), 
		  FloorMaterial(TEXT("")), 
		  ClusterId(0), 
		  RecommendedRoomId(TEXT("")), 
		  Feedback(TEXT("")), 
		  FeedbackTime(FDateTime::Now()), 
		  UpdateTime(FDateTime::Now()), 
		  IsRecommended(false) 
	{}
};

USTRUCT(BlueprintType)
struct FUser_like
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	int32 FeedbackId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString UserId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString VisitedRoomId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString VisitedUserId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString Feedback;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FDateTime FeedbackTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString Mood;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString Weather;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString MainAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString BackgroundColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FeedbackData/Struct")
	FString FloorMaterial;

	// 기본 생성자
	FUser_like()
		: FeedbackId(0), 
		  UserId(TEXT("")), 
		  VisitedRoomId(TEXT("")), 
		  VisitedUserId(TEXT("")), 
		  Feedback(TEXT("")), 
		  FeedbackTime(FDateTime::Now()), 
		  Mood(TEXT("")), 
		  Weather(TEXT("")), 
		  MainAsset(TEXT("")), 
		  BackgroundColor(TEXT("")), 
		  FloorMaterial(TEXT("")) 
	{}
};

USTRUCT(BlueprintType) 
struct FChangeIndex //ChangeIndex
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallPaper/Struct")
	FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallPaper/Struct")
	int32 index;

	//기본 생성자
	FChangeIndex() 
			: userId(TEXT("")),
			  index(0)
	{}
};

USTRUCT(BlueprintType) 
struct FMyRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoomInfo/Struct")
	FString RoomName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyRoomInfo/Struct")
	bool room_pp;

	//기본 생성자
	FMyRoomInfo() :
				RoomName(TEXT("")),
				room_pp(true)
	{}
};

USTRUCT(BlueprintType)
struct FMyCreateRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString UserId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	int32 RoomNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString RoomName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString RecommendedMusic;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString EmotionImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyCreateRoomInfo/Struct")
	FString Analysiscontent;

	// 기본 생성자
	FMyCreateRoomInfo()
		: UserId(TEXT("DefaultUser"))        
		, RoomNum(0)                          
		, RoomName(TEXT("Default Room"))       
		, RecommendedMusic(TEXT("None"))       
		, EmotionImage(TEXT("DefaultImage"))   
		, Analysiscontent(TEXT("No Analysis"))
	{}
};

USTRUCT(BlueprintType)
struct FRoomData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    FString question_03;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    int32 RoomNum;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    FString RoomName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    FString RecommendedMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    FString EmotionImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoomData")
    FString Analysiscontent;

    FRoomData() : question_03(TEXT("")),
				  RoomNum(0),
				  RoomName(TEXT("")),
				  RecommendedMusic(TEXT("")),
				  EmotionImage(TEXT("")),
				  Analysiscontent(TEXT(""))
				  {}
};

/**
 * 
 */
UCLASS()
class HTTPTEST_API UJsonParseLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static FString MakeJson(const TMap<FString, FString> source);

	//Struct To Json : Req
	static FString Login_Convert_StructToJson(const FLogin& LoginStrcut);
	//Json To Struct : Response
	static FLogin Login_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString SignUp_Convert_StructToJson(const FSign_up& SignUpStruct);
	//Json To Struct : Response
	static FSign_up SignUp_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString User_Convert_StructToJson(const FUser& UserStrcut);
	//Json To Struct : Response
	static FUser User_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString UserLike_Convert_StructToJson(const FUser_like& User_LikeStrcut);
	//Json To Struct : Response
	static FUser_like UserLike_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString ChangeIndex_Convert_StructToJson(const FChangeIndex& FWallPaperIndexStruct);
	//Json To Struct : Response
	static FChangeIndex ChangeIndex_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString MyRoomInfo_Convert_StructToJson(const FMyRoomInfo& MyRoomInfo);
	//Json To Struct : Response
	static FMyRoomInfo MyRoomInfo_Convert_JsonToStruct(const FString& JsonString);

	//Struct To Json : Req
	static FString FMyCreateRoomInfo_Convert_StructToJson(const FMyCreateRoomInfo& MyCreateRoomInfo);
	//Json To Struct : Response
	static FMyCreateRoomInfo FMyCreateRoomInfo_Convert_JsonToStruct(const FString& JsonString);

	//Json To Struct : Response
	static FRoomData RoomData_Convert_JsonToStruct(const FString& JsonString);
};
