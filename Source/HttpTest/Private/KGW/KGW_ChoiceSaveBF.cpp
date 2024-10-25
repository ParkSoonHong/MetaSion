// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_ChoiceSaveBF.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "JsonParseLib.h"
#include "JS_GameInstance.h"
#include "Kismet/GameplayStatics.h"


TArray<FChoiceData> UKGW_ChoiceSaveBF::ChoiceList;
TMap<FString, int32> SelectedChoices;

// void UKGW_ChoiceSaveBF::SaveChoicesToJsonFile(UObject* WorldContextObject)
// {
//     // ���õ� �����͸� �����ϴ� TMap (int32 �����͸� FString���� ��ȯ�Ͽ� ����)
//     TMap<FString, FString> ChoiceMap;
// 
//     // ������ TMap<FString, int32> �����͸� FString���� ��ȯ�ؼ� ����
//     for (const TPair<FString, int32>& Pair : SelectedChoices)
//     {
//         ChoiceMap.Add(Pair.Key, FString::FromInt(Pair.Value));  // int32 ���� FString���� ��ȯ
//     }
// 
//     // MakeJson�� ȣ���� TMap �����͸� JSON ���ڿ��� ��ȯ
//     FString JsonString = UJsonParseLib::MakeJson(ChoiceMap);
// //    UE_LOG(LogTemp, Warning, TEXT(" % s"), *JsonString);
//    FString ServerURL = "https://webhook.site/43600b7c-b435-42f0-851e-e3085ca8d187";
//    UJS_GameInstance* PostChoice = Cast<UJS_GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
// 
//    // ���� �ν��Ͻ��� ��ȿ�ϸ� ReqPostTest �Լ� ȣ��
//    if (PostChoice)
//    {
//        PostChoice->ReqPostChoice(ServerURL, JsonString);
//        UE_LOG(LogTemp, Warning, TEXT("ReqPostText..."));
// 
//    }
//    else {
//        UE_LOG(LogTemp, Warning, TEXT("fail..."));
// 
//    }
// 
// 
//     // ������ JSON ���ڿ��� ���Ϸ� ���� (Saved ����)
// //     FString FullPath = FPaths::ProjectSavedDir() + "SelectedChoiced.json";
// //     FFileHelper::SaveStringToFile(JsonString, *FullPath);
// }
void UKGW_ChoiceSaveBF::SaveChoicesToJsonFile(UObject* WorldContextObject)
{
    // WorldContextObject�� null���� Ȯ��
    if (!WorldContextObject)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid WorldContextObject: null object passed."));
        return;  // null�̸� �Լ� ����
    }

    // ���õ� �����͸� �����ϴ� TMap (int32 �����͸� FString���� ��ȯ�Ͽ� ����)
    TMap<FString, FString> ChoiceMap;

    // ������ TMap<FString, int32> �����͸� FString���� ��ȯ�ؼ� ����
    for (const TPair<FString, int32>& Pair : SelectedChoices)
    {
        ChoiceMap.Add(Pair.Key, FString::FromInt(Pair.Value));  // int32 ���� FString���� ��ȯ
    }

    // MakeJson�� ȣ���� TMap �����͸� JSON ���ڿ��� ��ȯ
    FString JsonString = UJsonParseLib::MakeJson(ChoiceMap);

    // JSON ���ڿ� Ȯ�� (������ �α�)
    UE_LOG(LogTemp, Warning, TEXT("Generated JSON: %s"), *JsonString);

    // ���� URL ����
    FString ServerURL = TEXT("https://explicitly-premium-catfish.ngrok-free.app/character");

//      WorldContextObject�� ���� ���� �ν��Ͻ��� ������
     UJS_GameInstance* PostChoice = Cast<UJS_GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject->GetWorld()));

    // ���� �ν��Ͻ��� ��ȿ�ϸ� ReqPostChoice �Լ� ȣ��
	 if (PostChoice)
	 {
        // POST ��û ������
        PostChoice->ReqPostChoice(ServerURL, JsonString);
        UE_LOG(LogTemp, Warning, TEXT("ReqPostChoice called successfully."));
	 }
	 else
	 {
// 		 ���� �ν��Ͻ��� �������� ���� ��� ��� �޽��� ���
			 UE_LOG(LogTemp, Warning, TEXT("Failed to get GameInstance from WorldContextObject."));
	 }
}



void UKGW_ChoiceSaveBF::AddChoice(const FChoiceData& ChoiceData)
{
    ChoiceList.Add(ChoiceData);

}

// TArray<FColorData> UKGW_ChoiceSaveBF::ParseJsonToRGB(const FString& JsonString)
// {
//     // ����� ��ȯ�� �迭 ����
//     TArray<FColorData> Colors;
// 
//     // JSON �����͸� �б� ���� Reader ����
//     TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);
// 
//     // �Ľ� ����� ���� JsonObject ����
//     TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
// 
//     // JSON �Ľ� �õ�
//     if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
//     {
//         // JSON �迭�� "CharacterRGB" �ʵ� ��������
//         TArray<TSharedPtr<FJsonValue>> ItemsArray = JsonObject->GetArrayField(TEXT("CharacterRGB"));
// 
//         // �迭 ��ȸ�ϸ鼭 �� ��ü���� �����͸� ����
//         for (TSharedPtr<FJsonValue> CharacterRGB : ItemsArray)
//         {
//             TSharedPtr<FJsonObject> ItemObject = CharacterRGB->AsObject();
// 
//             // FColorData ����ü ����
//             FColorData ColorData;
// 
//             // RGB �� ����
//             ColorData.R = ItemObject->GetNumberField(TEXT("R"));
//             ColorData.G = ItemObject->GetNumberField(TEXT("G"));
//             ColorData.B = ItemObject->GetNumberField(TEXT("B"));
// 
//             // ��� �迭�� �߰�
//             Colors.Add(ColorData);
//         }
//     }
// 
//     // RGB ���� ����� �迭 ��ȯ
//     return Colors;
// }

TArray<FColorData> UKGW_ChoiceSaveBF::ParseJsonToRGB(const FString& JsonString)
{
    // ����� ��ȯ�� �迭 ����
    TArray<FColorData> Colors;

    // JSON �����͸� �б� ���� Reader ����
    TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonString);

    // �Ľ� ����� ���� JsonObject ����
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    // JSON �Ľ� �õ�
    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // "R", "G", "B" �ʵ尡 �����ϴ��� Ȯ�� �� �Ľ�
        if (JsonObject->HasField(TEXT("R")) && JsonObject->HasField(TEXT("G")) && JsonObject->HasField(TEXT("B")))
        {
            // FColorData ����ü�� RGB ���� ����
            FColorData ColorData;
            ColorData.R = JsonObject->GetNumberField(TEXT("R"));
            ColorData.G = JsonObject->GetNumberField(TEXT("G"));
            ColorData.B = JsonObject->GetNumberField(TEXT("B"));

            // ��� �迭�� �߰�
            Colors.Add(ColorData);
            FColorData FirstColor = Colors[0];
            UE_LOG(LogTemp, Warning, TEXT("Succed."));
            UE_LOG(LogTemp, Log, TEXT("R: %f, G: %f, B: %f"), ColorData.R, ColorData.G, ColorData.B);

//             UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(CharacterMaterial, this);
//             if (DynamicMaterial)
//             {
//                 DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(FirstColor.R, FirstColor.G, FirstColor.B));
//                 CharacterMesh->SetMaterial(0, DynamicMaterial);
//             }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("R, G, B fields not found in JSON response."));
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Array length: %d"), Colors.Num());
    // RGB ���� ����� �迭 ��ȯ
    return Colors;
}

void UKGW_ChoiceSaveBF::StoreChoice(FString Question, int32 SelectedValue)
{
    SelectedChoices.Add(Question, SelectedValue);
}

// FString UKGW_ChoiceSaveBF::ParseJsonToColorData(const FString& json)
// {
//     TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
//     // �Ľ� ����� ���� ���� ����
//     TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
//     // �ؼ��� �Ѵ�.
//     FString returnValue;
//     TArray<FColorData> Colors;
// 
//     if (FJsonSerializer::Deserialize(reader, result))
//     {
//         TArray<TSharedPtr<FJsonValue>> ColorObject = result->GetArrayField(TEXT("items"));
//         for (TSharedPtr<FJsonValue> data : ColorObject)
//         {
//             
// //  // RGB �� ����
// //             float R = data->AsObject()->GetNumberField(TEXT("R"));
// //             float G = data->AsObject()->GetNumberField(TEXT("G"));
// //             float B = data->AsObject()->GetNumberField(TEXT("B"));
// // 
// //             // ������ �����͸� ��� �������� �߰�
// //             returnValue.Append(FString::Printf(TEXT("R: %.2f / G: %.2f / B: %.2f\n"), R, G, B));
//             TSharedPtr<FJsonObject> ItemObject = data->AsObject();
// 
//             // FColorData ����ü ����
//             FColorData ColorData;
// 
//             // RGB �� ����
//             ColorData.R = ItemObject->GetNumberField(TEXT("R"));
//             ColorData.G = ItemObject->GetNumberField(TEXT("G"));
//             ColorData.B = ItemObject->GetNumberField(TEXT("B"));
// 
//             // ��� �迭�� �߰�
//             Colors.Add(ColorData);
//         }
//     }
//     
//     // ��ȯ�Ѵ�.
//     return Colors;
// 
// }
