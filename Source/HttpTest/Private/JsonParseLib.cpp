// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "JsonObjectConverter.h"
#include "CJS/SessionGameInstance.h"
// Login ----------------------------------------------------------------------------
FString UJsonParseLib::Login_Convert_StructToJson(const FLogin& LoginStrcut)
{
	//Json 문자열을 저장할 변수
	FString JsonString;

	//구조체를 JSON 문자열로 변환
	FJsonObjectConverter::UStructToJsonObjectString(LoginStrcut, JsonString, 0, 0);

	//완성된 Json 반환
	return JsonString;
}


FLogin UJsonParseLib::Login_Convert_JsonToStruct(const FString& JsonString)
{
	FLogin loginJson;

	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &loginJson, 0, 0);

	//변환된 구조체를 반환
	return loginJson;
}

// Login End----------------------------------------------------------------------------

// Sign up ----------------------------------------------------------------------------
FString UJsonParseLib::SignUp_Convert_StructToJson(const FSign_up& SignUpStruct)
{
	FString JsonString;
	//Json을 구조체로 변환
	FJsonObjectConverter::UStructToJsonObjectString(SignUpStruct, JsonString, 0, 0);
	//변환된 구조체를 반환
	return JsonString;
}

FSign_up UJsonParseLib::SignUp_Convert_JsonToStruct(const FString& JsonString)
{
	FSign_up SingupStruct;
	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &SingupStruct, 0, 0);
	//변환된 구조체를 반환
	return SingupStruct;
}
// Sign up End----------------------------------------------------------------------------

// User ----------------------------------------------------------------------------
FString UJsonParseLib::User_Convert_StructToJson(const FUser& UserStrcut)
{
	//Json 문자열을 저장할 변수
	FString JsonString;

	//구조체를 JSON 문자열로 변환
	FJsonObjectConverter::UStructToJsonObjectString(UserStrcut, JsonString, 0, 0);

	//완성된 Json 반환
	return JsonString;
}

FUser UJsonParseLib::User_Convert_JsonToStruct(const FString& JsonString)
{
	FUser UserJson;

	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UserJson, 0, 0);

	//변환된 구조체를 반환
	return UserJson;

}
// User End----------------------------------------------------------------------------

// ChangeIndex ----------------------------------------------------------------------------
FString UJsonParseLib::ChangeIndex_Convert_StructToJson(const FChangeIndex& FWallPaperIndexStruct)
{
	//Json 문자열을 저장할 변수
	FString JsonString;

	//구조체를 JSON 문자열로 변환
	FJsonObjectConverter::UStructToJsonObjectString(FWallPaperIndexStruct, JsonString, 0, 0);

	//완성된 Json 반환
	return JsonString;
}

FChangeIndex UJsonParseLib::ChangeIndex_Convert_JsonToStruct(const FString& JsonString)
{
	FChangeIndex WallPaperJson;
	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &WallPaperJson, 0, 0);
	//변환된 구조체를 반환
	return WallPaperJson;
}
// ChangeIndex End ----------------------------------------------------------------------------

// MyRoomInfo ----------------------------------------------------------------------------
FString UJsonParseLib::MyRoomInfo_Convert_StructToJson(const FMyRoomInfo& MyRoomInfo)
{
	//Json 문자열을 저장할 변수
	FString JsonString;

	//구조체를 JSON 문자열로 변환
	FJsonObjectConverter::UStructToJsonObjectString(MyRoomInfo, JsonString, 0, 0);

	//완성된 Json 반환
	return JsonString;
}
FMyRoomInfo UJsonParseLib::MyRoomInfo_Convert_JsonToStruct(const FString& JsonString)
{
	FMyRoomInfo MyRoomInfo;

	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &MyRoomInfo, 0, 0);

	//변환된 구조체를 반환
	return MyRoomInfo;
}
// MyRoomInfo End----------------------------------------------------------------------------

// MyCreateRoomInfo ----------------------------------------------------------------------------
FString UJsonParseLib::FMyCreateRoomInfo_Convert_StructToJson(const FMyCreateRoomInfo& MyCreateRoomInfo)
{
	//Json 문자열을 저장할 변수
	FString JsonString;

	//구조체를 JSON 문자열로 변환
	FJsonObjectConverter::UStructToJsonObjectString(MyCreateRoomInfo, JsonString, 0, 0);

	//완성된 Json 반환
	return JsonString;
}
FMyCreateRoomInfo UJsonParseLib::FMyCreateRoomInfo_Convert_JsonToStruct(const FString& JsonString)
{
	FMyCreateRoomInfo MyCreateRoomInfo;

	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &MyCreateRoomInfo, 0, 0);

	//변환된 구조체를 반환
	return MyCreateRoomInfo;
}
FRoomData UJsonParseLib::RoomData_Convert_JsonToStruct(const FString& JsonString)
{
	FRoomData RoomData;

	//Json을 구조체로 변환
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &RoomData, 0, 0);

	//변환된 구조체를 반환
	return RoomData;
}
// MyCreateRoomInfo End ----------------------------------------------------------------------------

FString UJsonParseLib::MakeJson(const TMap<FString, FString>& source)
{
	// source를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer를 만들어서 JsonObject를 인코딩해서 
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	// 반환한다.
	return json;
}

TArray<FMyCreatedRoom>  UJsonParseLib::JsonParseRoomList(const FString& json)
{
	TArray<FMyCreatedRoom> RoomInfos;
	// 리더기를 만들고
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	// 파싱 결과를 담을 변수 선언
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
	// 해석을 한다.
	FString returnValue;
	if (FJsonSerializer::Deserialize(reader, result))
	{
		TArray<TSharedPtr<FJsonValue>> parseDataList = result->GetArrayField(TEXT("userRooms"));
		for (TSharedPtr<FJsonValue> data : parseDataList)
		{
			
			FString roomNum = data->AsObject()->GetStringField("roomNum");
			FString roomName = data->AsObject()->GetStringField("roomName");
			returnValue.Append(FString::Printf(TEXT("roomNum : %s / roomName : %s\n"), *roomNum, *roomName));
			FMyCreatedRoom RoomInfo;
			RoomInfo.RoomNum = roomNum;
			RoomInfo.RoomName = roomName;
			RoomInfos.Add(RoomInfo);
		}
	}
	// 반환한다.
	return RoomInfos;
}
