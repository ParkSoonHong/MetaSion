// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW_SkyLightGameModeBase.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/LightComponent.h"



void AKGW_SkyLightGameModeBase::AdjustSunBrightness(float SliderValue)
{

    if (DirectionalLight)
    {
        // LightComponent를 UDirectionalLightComponent로 캐스팅
//         UDirectionalLightComponent* LightComponent = Cast<UDirectionalLightComponent>(DirectionalLight->GetLightComponent());
        if (DirectionalLight)
        {
            // 슬라이더 값에 따라 밝기 조절
            DirectionalLight->SetIntensity(SliderValue * 500.0f); // 밝기 값 범위 설정
        }
    }
    
}
