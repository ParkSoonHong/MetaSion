// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_SaveLevel.h"
#include "Kismet/GameplayStatics.h"


bool UKGW_SaveLevel::SaveGameData(AActor* SkyActor, AActor* WeatherActor)
{
    // 세이브 게임 객체 생성
    UKGW_SaveLevel* SaveGameInstance = Cast<UKGW_SaveLevel>(UGameplayStatics::CreateSaveGameObject(UKGW_SaveLevel::StaticClass()));
    bool bSaveSuccessful = false;
    if (SaveGameInstance)
    {
        // Ultra Sky 상태 저장
        if (SkyActor)
        {
            // 블루프린트에서 TimeOfDay라는 속성이 있을 때 접근
            FProperty* TimeOfDayProperty = SkyActor->GetClass()->FindPropertyByName(FName("TimeOfDay"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(TimeOfDayProperty))
            {
                float LocalTimeOfDay = NumericProperty->GetFloatingPointPropertyValue(TimeOfDayProperty->ContainerPtrToValuePtr<void>(SkyActor));
                SaveGameInstance->TimeOfDay = LocalTimeOfDay;
            }
        }

        // Ultra Weather 상태 저장
        if (WeatherActor)
        {
            // 각각의 속성을 같은 방식으로 접근
            FProperty* CloudCoverageProperty = WeatherActor->GetClass()->FindPropertyByName(FName("CloudCoverage"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(CloudCoverageProperty))
            {
                float LocalCloudCoverage = NumericProperty->GetFloatingPointPropertyValue(CloudCoverageProperty->ContainerPtrToValuePtr<void>(WeatherActor));
                SaveGameInstance->CloudCoverage = LocalCloudCoverage;
                UE_LOG(LogTemp, Warning, TEXT("Generated Server URL: %f"), LocalCloudCoverage);

            }

            // 다른 속성들에 대해서도 같은 방식으로 설정
            FProperty* FogProperty = WeatherActor->GetClass()->FindPropertyByName(FName("Fog"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(FogProperty))
            {
                float LocalFog = NumericProperty->GetFloatingPointPropertyValue(FogProperty->ContainerPtrToValuePtr<void>(WeatherActor));
                SaveGameInstance->Fog = LocalFog;
            }

            FProperty* WindIntensityProperty = WeatherActor->GetClass()->FindPropertyByName(FName("WindIntensity"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(WindIntensityProperty))
            {
                float LocalWindIntensity = NumericProperty->GetFloatingPointPropertyValue(WindIntensityProperty->ContainerPtrToValuePtr<void>(WeatherActor));
                SaveGameInstance->WindIntensity = LocalWindIntensity;
            }

            // 마찬가지로 다른 속성들도 처리 (Rain, Snow, Dust 등등)
        }

        // 세이브 게임 파일로 저장
        bSaveSuccessful =UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("MySaveSlot"), 0);
    }
    return bSaveSuccessful;
}

void UKGW_SaveLevel::LoadGameData(AActor* SkyActor, AActor* WeatherActor)
{
    // 세이브 게임 데이터를 불러오기 (MySaveSlot에서 0번 슬롯)
    UKGW_SaveLevel* LoadGameInstance = Cast<UKGW_SaveLevel>(UGameplayStatics::LoadGameFromSlot(TEXT("MySaveSlot"), 0));

    if (LoadGameInstance)
    {
        // 세이브 게임 데이터를 성공적으로 불러온 경우

        // Ultra Sky 데이터 복원
        if (SkyActor)
        {
            FProperty* TimeOfDayProperty = SkyActor->GetClass()->FindPropertyByName(FName("TimeOfDay"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(TimeOfDayProperty))
            {
                // 세이브된 데이터를 SkyActor에 적용
                NumericProperty->SetFloatingPointPropertyValue(TimeOfDayProperty->ContainerPtrToValuePtr<void>(SkyActor), LoadGameInstance->TimeOfDay);
            }
        }

        // Ultra Weather 데이터 복원
        if (WeatherActor)
        {
            // CloudCoverage
            FProperty* CloudCoverageProperty = WeatherActor->GetClass()->FindPropertyByName(FName("CloudCoverage"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(CloudCoverageProperty))
            {
                NumericProperty->SetFloatingPointPropertyValue(CloudCoverageProperty->ContainerPtrToValuePtr<void>(WeatherActor), LoadGameInstance->CloudCoverage);
            }

            // Fog
            FProperty* FogProperty = WeatherActor->GetClass()->FindPropertyByName(FName("Fog"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(FogProperty))
            {
                NumericProperty->SetFloatingPointPropertyValue(FogProperty->ContainerPtrToValuePtr<void>(WeatherActor), LoadGameInstance->Fog);
            }

            // WindIntensity
            FProperty* WindIntensityProperty = WeatherActor->GetClass()->FindPropertyByName(FName("WindIntensity"));
            if (FNumericProperty* NumericProperty = CastField<FNumericProperty>(WindIntensityProperty))
            {
                NumericProperty->SetFloatingPointPropertyValue(WindIntensityProperty->ContainerPtrToValuePtr<void>(WeatherActor), LoadGameInstance->WindIntensity);
            }
        }

        // 로그 출력: 세이브 데이터를 성공적으로 불러왔음을 알림
        UE_LOG(LogTemp, Log, TEXT("Game data loaded successfully"));
    }
    else
    {
        // 세이브 데이터를 불러오는 데 실패한 경우
        UE_LOG(LogTemp, Error, TEXT("Failed to load game data"));
    }
}
