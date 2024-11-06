// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "JsonObjectConverter.h"
#include "CJS/SessionGameInstance.h"
// Login ----------------------------------------------------------------------------
FString UJsonParseLib::Login_Convert_StructToJson(const FLogin& LoginStrcut)
{
	//Json ���ڿ��� ������ ����
	FString JsonString;

	//����ü�� JSON ���ڿ��� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(LoginStrcut, JsonString, 0, 0);

	//�ϼ��� Json ��ȯ
	return JsonString;
}


FLogin UJsonParseLib::Login_Convert_JsonToStruct(const FString& JsonString)
{
	FLogin loginJson;

	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &loginJson, 0, 0);

	//��ȯ�� ����ü�� ��ȯ
	return loginJson;
}

// Login End----------------------------------------------------------------------------

// Sign up ----------------------------------------------------------------------------
FString UJsonParseLib::SignUp_Convert_StructToJson(const FSign_up& SignUpStruct)
{
	FString JsonString;
	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(SignUpStruct, JsonString, 0, 0);
	//��ȯ�� ����ü�� ��ȯ
	return JsonString;
}

FSign_up UJsonParseLib::SignUp_Convert_JsonToStruct(const FString& JsonString)
{
	FSign_up SingupStruct;
	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &SingupStruct, 0, 0);
	//��ȯ�� ����ü�� ��ȯ
	return SingupStruct;
}
// Sign up End----------------------------------------------------------------------------

// User ----------------------------------------------------------------------------
FString UJsonParseLib::User_Convert_StructToJson(const FUser& UserStrcut)
{
	//Json ���ڿ��� ������ ����
	FString JsonString;

	//����ü�� JSON ���ڿ��� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(UserStrcut, JsonString, 0, 0);

	//�ϼ��� Json ��ȯ
	return JsonString;
}

FUser UJsonParseLib::User_Convert_JsonToStruct(const FString& JsonString)
{
	FUser UserJson;

	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &UserJson, 0, 0);

	//��ȯ�� ����ü�� ��ȯ
	return UserJson;

}
// User End----------------------------------------------------------------------------

// ChangeIndex ----------------------------------------------------------------------------
FString UJsonParseLib::ChangeIndex_Convert_StructToJson(const FChangeIndex& FWallPaperIndexStruct)
{
	//Json ���ڿ��� ������ ����
	FString JsonString;

	//����ü�� JSON ���ڿ��� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(FWallPaperIndexStruct, JsonString, 0, 0);

	//�ϼ��� Json ��ȯ
	return JsonString;
}

FChangeIndex UJsonParseLib::ChangeIndex_Convert_JsonToStruct(const FString& JsonString)
{
	FChangeIndex WallPaperJson;
	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &WallPaperJson, 0, 0);
	//��ȯ�� ����ü�� ��ȯ
	return WallPaperJson;
}
// ChangeIndex End ----------------------------------------------------------------------------

// MyRoomInfo ----------------------------------------------------------------------------
FString UJsonParseLib::MyRoomInfo_Convert_StructToJson(const FMyRoomInfo& MyRoomInfo)
{
	//Json ���ڿ��� ������ ����
	FString JsonString;

	//����ü�� JSON ���ڿ��� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(MyRoomInfo, JsonString, 0, 0);

	//�ϼ��� Json ��ȯ
	return JsonString;
}
FMyRoomInfo UJsonParseLib::MyRoomInfo_Convert_JsonToStruct(const FString& JsonString)
{
	FMyRoomInfo MyRoomInfo;

	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &MyRoomInfo, 0, 0);

	//��ȯ�� ����ü�� ��ȯ
	return MyRoomInfo;
}
// MyRoomInfo End----------------------------------------------------------------------------

// MyCreateRoomInfo ----------------------------------------------------------------------------
FString UJsonParseLib::FMyCreateRoomInfo_Convert_StructToJson(const FMyCreateRoomInfo& MyCreateRoomInfo)
{
	//Json ���ڿ��� ������ ����
	FString JsonString;

	//����ü�� JSON ���ڿ��� ��ȯ
	FJsonObjectConverter::UStructToJsonObjectString(MyCreateRoomInfo, JsonString, 0, 0);

	//�ϼ��� Json ��ȯ
	return JsonString;
}
FMyCreateRoomInfo UJsonParseLib::FMyCreateRoomInfo_Convert_JsonToStruct(const FString& JsonString)
{
	FMyCreateRoomInfo MyCreateRoomInfo;

	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &MyCreateRoomInfo, 0, 0);

	//��ȯ�� ����ü�� ��ȯ
	return MyCreateRoomInfo;
}
FRoomData UJsonParseLib::RoomData_Convert_JsonToStruct(const FString& JsonString)
{
	FRoomData RoomData;

	//Json�� ����ü�� ��ȯ
	FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &RoomData, 0, 0);

	//��ȯ�� ����ü�� ��ȯ
	return RoomData;
}
// MyCreateRoomInfo End ----------------------------------------------------------------------------

FString UJsonParseLib::MakeJson(const TMap<FString, FString>& source)
{
	// source�� JsonObject �������� �����.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer�� ���� JsonObject�� ���ڵ��ؼ� 
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	// ��ȯ�Ѵ�.
	return json;
}

TArray<FMyCreatedRoom>  UJsonParseLib::JsonParseRoomList(const FString& json)
{
	TArray<FMyCreatedRoom> RoomInfos;
	// �����⸦ �����
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	// �Ľ� ����� ���� ���� ����
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
	// �ؼ��� �Ѵ�.
	FString returnValue;
	if (FJsonSerializer::Deserialize(reader, result))
	{
		TArray<TSharedPtr<FJsonValue>> parseDataList = result->GetArrayField(TEXT("userRooms"));
		for (TSharedPtr<FJsonValue> data : parseDataList)
		{
			
			FString roomNum = data->AsObject()->GetStringField(TEXT("roomNum"));
			FString roomName = data->AsObject()->GetStringField(TEXT("roomName"));
			returnValue.Append(FString::Printf(TEXT("roomNum : %s / roomName : %s\n"), *roomNum, *roomName));
			FMyCreatedRoom RoomInfo;
			RoomInfo.RoomNum = roomNum;
			RoomInfo.RoomName = roomName;
			RoomInfos.Add(RoomInfo);
		}
	}
	// ��ȯ�Ѵ�.
	return RoomInfos;
}
