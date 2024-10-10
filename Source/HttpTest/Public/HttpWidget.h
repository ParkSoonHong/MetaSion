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

	UPROPERTY()
	class AHttpActor* HttpActor;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextLog;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* UserID;

	UPROPERTY(meta=(BindWidget))
	class UEditableText* UserPW;

	UPROPERTY(meta=(BindWidget))
	class UButton* ButtonPostSend;

	UFUNCTION()
	void OnMyClickSendPost();

	UPROPERTY(meta=(BindWidget))
	class UButton* ButtonGetWebImage;

	UFUNCTION()
	void OnMyClickGetWebImage();

	UPROPERTY(meta=(BindWidget))
	class UImage* ImageWeb;

	void SetWebImage(class UTexture2D* newTexture);

	void SetTextLog(FString log);

	void SetHttpActor(class AHttpActor* actor);


	FString URL = "https://apis.data.go.kr/4050000/libnewbk/getLibnewbk";

	FString Key = "1B4jOfk0801JYukDA2FApT%2Bs0VOwrTVSE5qPJFlZ1mgXYs1UhkQ53Zj23EbsaJAITIcsaLGVB2gDGEMSk6IaDA%3D%3D";

	FString ServerURL = "http://mtvs.helloworldlabs.kr:7771/api/json";

	FString WebImageURL = "https://t1.daumcdn.net/news/202105/25/koreadognews/20210525045023050wquu.jpg";
};
