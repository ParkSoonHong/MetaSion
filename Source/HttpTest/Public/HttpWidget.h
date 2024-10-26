// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HttpWidget.generated.h"

/**
 * 
 */
UCLASS()
class HTTPTEST_API UHttpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	class AHttpActor* HttpActor;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextLog;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* UserID;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* UserPW;

	UPROPERTY(meta=(BindWidget))
	class UButton* ButtonPostSend;

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_User;

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_UserLike;

	UPROPERTY(meta=(BindWidget))
	class UButton* btn_SignUp;

	UFUNCTION()
	void SendLoginData();

	UFUNCTION()
	void SendSignUpData();

	UFUNCTION()
	void SendUserData();

	UFUNCTION()
	void SendUserLikeData();

	void SetTextLog(FString log);

	void SetHttpActor(class AHttpActor* actor);

	//FString ServerURL = "https://jsonplaceholder.typicode.com/posts";
	FString ServerURL = "https://webhook.site/a0cbc113-e54b-4c1b-a92a-acb925a13d24";

	//FString URL = "https://apis.data.go.kr/4050000/libnewbk/getLibnewbk";
	//FString Key = "1B4jOfk0801JYukDA2FApT%2Bs0VOwrTVSE5qPJFlZ1mgXYs1UhkQ53Zj23EbsaJAITIcsaLGVB2gDGEMSk6IaDA%3D%3D";
};
