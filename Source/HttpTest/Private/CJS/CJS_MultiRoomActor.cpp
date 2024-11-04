// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_MultiRoomActor.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextBlock.h"
#include "Engine/Font.h"

// Sets default values
ACJS_MultiRoomActor::ACJS_MultiRoomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the sphere collision component
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;

	// Create the sphere mesh component and attach it to the collision
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(SphereMesh);

	RefRoomInfoWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("RefRoomInfoWidget"));
	RefRoomInfoWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ACJS_MultiRoomActor::BeginPlay()
{
	Super::BeginPlay();
	
	// Get the user widget instance
	UUserWidget* WidgetInstance = Cast<UUserWidget>(RefRoomInfoWidgetComp->GetUserWidgetObject());
	if (WidgetInstance)
	{
		// Get references to the TextBlock components inside the widget
		Txt_CurNumPlayer = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("Txt_CurNumPlayer")));
		Txt_MaxNumPlayer = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("Txt_MaxNumPlayer")));
		Txt_RefRoomName = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("Txt_RefRoomName")));
		Txt_RefPercent = Cast<UTextBlock>(WidgetInstance->GetWidgetFromName(TEXT("Txt_RefPercent")));
	}
}

// Called every frame
void ACJS_MultiRoomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACJS_MultiRoomActor::InitRefRoomInfoWidget(int32 CurNumPlayer, int32 MaxNumPlayer, const FString& RoomName, const FString& Percent)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_MultiRoomActor::InitRefRoomInfoWidget"));

	// MultiRoomActor 클래스 내에서 InitRefRoomInfoWidget 함수의 시작 부분에 추가
	UE_LOG(LogTemp, Warning, TEXT("Initializing Room Info Widget for Room: %s"), *RoomName);
	UE_LOG(LogTemp, Warning, TEXT("MultiRoomActor location: %s"), *GetActorLocation().ToString());

	if (Txt_CurNumPlayer)
	{
		Txt_CurNumPlayer->SetText(FText::FromString(FString::FromInt(CurNumPlayer)));
	}

	if (Txt_MaxNumPlayer)
	{
		Txt_MaxNumPlayer->SetText(FText::FromString(FString::FromInt(MaxNumPlayer)));
	}

	if (Txt_RefRoomName)
	{
		// FText::FromString 대신 FText::AsCultureInvariant를 사용하여 한글 텍스트 설정
		//Txt_RefRoomName->SetText(FText::AsCultureInvariant(RoomName));
		
		// 폰트 로드
		//UFont* Font = LoadObject<UFont>(nullptr, TEXT("/Game/CJS/Fonts/F_NanumGothic.F_NanumGothic")); // 한글 지원 폰트 사용
		//if (Font)
		//{
		//	FSlateFontInfo FontInfo(Font, 32);

		//	// SetFont 사용
		//	Txt_RefRoomName->SetFont(FontInfo);
		//	Txt_RefRoomName->SetText(FText::AsCultureInvariant(RoomName));
		//	UE_LOG(LogTemp, Warning, TEXT("Font set successfully."));
		//}
		//else
		//{
		//	UE_LOG(LogTemp, Error, TEXT("Failed to load font."));
		//}

		// 한글 텍스트 설정
		Txt_RefRoomName->SetText(FText::FromString(RoomName));
	
	}

	if (Txt_RefPercent)
	{
		// Percent 값을 텍스트로 변환하여 표시 (예: 50%)
		//FString PercentText = FString::Printf(TEXT("%.2f%"), Percent);
		Txt_RefPercent->SetText(FText::FromString(Percent));
	}


	// 메시지 값으로 메쉬 크기 조정
	int32 MessageValue = FCString::Atoi(*Percent); // Percent에서 메시지 값 가져오기 (문자열을 정수로 변환)
	float ScaleFactor = FMath::GetMappedRangeValueClamped(FVector2D(0, 100), FVector2D(2.0f, 10.0f), MessageValue); // 예: 0~100의 메시지를 0.5~2.0의 범위로 변환
	// 스케일 적용
	SphereMesh->SetWorldScale3D(FVector(ScaleFactor)); // 메쉬 크기 조정
}

