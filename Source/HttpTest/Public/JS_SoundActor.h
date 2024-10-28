// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_SoundActor.generated.h"

UCLASS()
class HTTPTEST_API AJS_SoundActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_SoundActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class AHttpActor* HttpActor;

	UPROPERTY()
	UAudioComponent* AudioComponent;  // 사운드를 재생하는 오디오 컴포넌트

	// 서버에서 받은 파일 이름을 통해 배경음을 변경하는 함수
	void SetBackgroundSoundByFileName(const FString& FileName);
};
