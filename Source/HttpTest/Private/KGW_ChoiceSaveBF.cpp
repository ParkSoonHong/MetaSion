// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW_ChoiceSaveBF.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "JsonParseLib.h"


TArray<FChoiceData> UKGW_ChoiceSaveBF::ChoiceList;
TMap<FString, int32> SelectedChoices;

void UKGW_ChoiceSaveBF::SaveChoicesToJsonFile()
{
    // 선택된 데이터를 저장하는 TMap (int32 데이터를 FString으로 변환하여 저장)
    TMap<FString, FString> ChoiceMap;

    // 기존의 TMap<FString, int32> 데이터를 FString으로 변환해서 저장
    for (const TPair<FString, int32>& Pair : SelectedChoices)
    {
        ChoiceMap.Add(Pair.Key, FString::FromInt(Pair.Value));  // int32 값을 FString으로 변환
    }

    // MakeJson을 호출해 TMap 데이터를 JSON 문자열로 변환
    FString JsonString = UJsonParseLib::MakeJson(ChoiceMap);

    // 생성된 JSON 문자열을 파일로 저장 (Saved 폴더)
    FString FullPath = FPaths::ProjectSavedDir() + "SelectedChoiced.json";
    FFileHelper::SaveStringToFile(JsonString, *FullPath);
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

void UKGW_ChoiceSaveBF::StoreChoice(FString Question, int32 SelectedValue)
{
    SelectedChoices.Add(Question, SelectedValue);
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
