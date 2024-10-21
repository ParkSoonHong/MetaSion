// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW_ChoiceSaveBF.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"


TArray<FChoiceData> UKGW_ChoiceSaveBF::ChoiceList;

bool UKGW_ChoiceSaveBF::SaveChoicesToJsonFile(const FString& FileName)
{

    // JSON 배열을 생성
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    TArray<TSharedPtr<FJsonValue>> JsonArray;

    // 각 선택지를 JSON 객체로 변환하여 배열에 추가
    for (const FChoiceData& ChoiceData : ChoiceList)
    {
        TSharedPtr<FJsonObject> ChoiceObject = MakeShareable(new FJsonObject);
        ChoiceObject->SetStringField(TEXT("Question"), ChoiceData.Question);
        ChoiceObject->SetStringField(TEXT("SelectedChoice"), ChoiceData.SelectedChoice);

        JsonArray.Add(MakeShareable(new FJsonValueObject(ChoiceObject)));
    }

    // JSON 객체에 선택지 배열 추가
    JsonObject->SetArrayField(TEXT("Choices"), JsonArray);

    // JSON 문자열로 직렬화
    FString json;
    TSharedRef<TJsonWriter<TCHAR>> Writer = TJsonWriterFactory<TCHAR>::Create(&json);
    if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
    {
        // 저장할 파일 경로 설정 (Saved 디렉토리)
        FString FullPath = FPaths::ProjectSavedDir() + FileName;
        return FFileHelper::SaveStringToFile(json, *FullPath);
    }

    return false;
}


void UKGW_ChoiceSaveBF::AddChoice(const FChoiceData& ChoiceData)
{
    ChoiceList.Add(ChoiceData);

}

TArray<FColorData> UKGW_ChoiceSaveBF::ParseJsonToRGB(const FString& JsonString)
{
    // 결과로 반환할 배열 생성
    TArray<FColorData> Colors;

    // JSON 데이터를 읽기 위한 Reader 생성
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);

    // 파싱 결과를 담을 JsonObject 생성
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    // JSON 파싱 시도
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // JSON 배열인 "items" 필드 가져오기
        TArray<TSharedPtr<FJsonValue>> ItemsArray = JsonObject->GetArrayField(TEXT("items"));

        // 배열 순회하면서 각 객체에서 데이터를 추출
        for (TSharedPtr<FJsonValue> Item : ItemsArray)
        {
            TSharedPtr<FJsonObject> ItemObject = Item->AsObject();

            // FColorData 구조체 생성
            FColorData ColorData;

            // RGB 값 추출
            ColorData.R = ItemObject->GetNumberField(TEXT("R"));
            ColorData.G = ItemObject->GetNumberField(TEXT("G"));
            ColorData.B = ItemObject->GetNumberField(TEXT("B"));

            // 결과 배열에 추가
            Colors.Add(ColorData);
        }
    }

    // RGB 값이 저장된 배열 반환
    return Colors;
}

// FString UKGW_ChoiceSaveBF::ParseJsonToColorData(const FString& json)
// {
//     TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
//     // 파싱 결과를 담을 변수 선언
//     TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
//     // 해석을 한다.
//     FString returnValue;
//     TArray<FColorData> Colors;
// 
//     if (FJsonSerializer::Deserialize(reader, result))
//     {
//         TArray<TSharedPtr<FJsonValue>> ColorObject = result->GetArrayField(TEXT("items"));
//         for (TSharedPtr<FJsonValue> data : ColorObject)
//         {
//             
// //  // RGB 값 추출
// //             float R = data->AsObject()->GetNumberField(TEXT("R"));
// //             float G = data->AsObject()->GetNumberField(TEXT("G"));
// //             float B = data->AsObject()->GetNumberField(TEXT("B"));
// // 
// //             // 추출한 데이터를 출력 형식으로 추가
// //             returnValue.Append(FString::Printf(TEXT("R: %.2f / G: %.2f / B: %.2f\n"), R, G, B));
//             TSharedPtr<FJsonObject> ItemObject = data->AsObject();
// 
//             // FColorData 구조체 생성
//             FColorData ColorData;
// 
//             // RGB 값 추출
//             ColorData.R = ItemObject->GetNumberField(TEXT("R"));
//             ColorData.G = ItemObject->GetNumberField(TEXT("G"));
//             ColorData.B = ItemObject->GetNumberField(TEXT("B"));
// 
//             // 결과 배열에 추가
//             Colors.Add(ColorData);
//         }
//     }
//     
//     // 반환한다.
//     return Colors;
// 
// }
