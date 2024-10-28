// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_SoundActor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "HttpActor.h"
#include "JsonParseLib.h"

// Sets default values
AJS_SoundActor::AJS_SoundActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Audio Component 생성 및 설정
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false;  // 자동 재생 해제
}

// Called when the game starts or when spawned
void AJS_SoundActor::BeginPlay()
{
	Super::BeginPlay();

	HttpActor = Cast<AHttpActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AHttpActor::StaticClass()));
	if (HttpActor) {
		FRoomData RoomData = HttpActor->GetRoomData();
		FString FileName = RoomData.RecommendedMusic;
		SetBackgroundSoundByFileName(FileName);
	}
}

// Called every frame
void AJS_SoundActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_SoundActor::SetBackgroundSoundByFileName(const FString& FileName)
{
	// 사운드 파일이 콘텐츠 폴더의 특정 경로에 있는 경우
	// 예를 들어, "/Game/Audio/" 디렉토리에 사운드 파일이 있다고 가정합니다.
	FString AssetPath = FString::Printf(TEXT("/Game/Main/Sound/%s.%s"), *FileName, *FileName);

	// 사운드 파일을 동적으로 로드
	USoundBase* NewSound = Cast<USoundBase>(StaticLoadObject(USoundBase::StaticClass(), nullptr, *AssetPath));

	if (NewSound)
	{
		// 오디오 컴포넌트에 새 사운드를 설정하고 재생
		AudioComponent->SetSound(NewSound);
		AudioComponent->Play();
		UE_LOG(LogTemp, Warning, TEXT("Background sound changed and started playing: %s"), *FileName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load sound: %s"), *AssetPath);
	}
}

