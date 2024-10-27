// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CJS_BallPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API ACJS_BallPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    FString UserId = "Sunny321";

    /*void SetUserId(const FString& NewUserId)
    {
        UserId = NewUserId;
    }*/

    FString GetUserId() const
    {
        return UserId;
    }
};
