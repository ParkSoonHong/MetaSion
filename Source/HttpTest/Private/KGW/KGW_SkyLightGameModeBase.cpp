// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/KGW_SkyLightGameModeBase.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/LightComponent.h"



void AKGW_SkyLightGameModeBase::AdjustSunBrightness(float SliderValue)
{

    if (DirectionalLight)
    {
        // LightComponent�� UDirectionalLightComponent�� ĳ����
//         UDirectionalLightComponent* LightComponent = Cast<UDirectionalLightComponent>(DirectionalLight->GetLightComponent());
        if (DirectionalLight)
        {
            // �����̴� ���� ���� ��� ����
            DirectionalLight->SetIntensity(SliderValue * 500.0f); // ��� �� ���� ����
        }
    }
    
}
