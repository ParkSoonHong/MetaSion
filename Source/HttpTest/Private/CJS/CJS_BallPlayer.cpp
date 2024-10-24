// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_BallPlayer.h"
#include "CJS/CJS_HeartActor.h"
#include "CJS/CJS_BallPlayerAnimInstance.h"
#include "CJS/CJS_AimPointWidget.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequence.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"



// Sets default values
ACJS_BallPlayer::ACJS_BallPlayer() : Super()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector(0, 0, 8.5));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->TargetOffset = FVector(0.f, 0.f, 100.0f);
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = true;

	// 설정할 하트의 초기 위치를 위한 위치 값 (직접 값을 조정 가능)
	HeartSpawnPosition = FVector(100.f, 0.f, 50.f); // 적당히 초기 위치 오프셋 지정
}


// Called when the game starts or when spawned
void ACJS_BallPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Explicitly obtain the PlayerController
	//APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	//if (PlayerController)
	//{
	//	FInputModeGameOnly InputMode;
	//	PlayerController->SetInputMode(InputMode);
	//	PlayerController->bShowMouseCursor = false;
	//	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - Input mode set to Game Only"));

	//	// Possess the character if not already possessed
	//	if (PlayerController->GetPawn() != this)
	//	{
	//		PlayerController->Possess(this);
	//		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - PlayerController has possessed the pawn"));
	//	}
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay() - NO PlayerController"));
	//}	


	// 컨트롤러를 가져와서 캐스팅
	PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay()::PlayerController (pc) is assigned in BeginPlay"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay()::PlayerController (pc) is null in BeginPlay"));
		return;
	}

	// 애니메이션 시퀀스가 제대로 로드되었는지 확인 (1개씩 적용)
	/*if (TestAnimSequence)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - TestAnimSequence loaded successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay() - TestAnimSequence is not set"));
	}*/
	
	// 애니메이션 시퀀스 배열이 제대로 설정되었는지 확인
	if (AnimSequences.Num() == 8)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - AnimSequences initialized with 8 elements"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay() - AnimSequences does not have 8 elements"));
	}

	// WBP_AimPoint 위젯 생성
	if (WBP_AimPoint)  // WBP_aimpoint가 올바르게 할당되어 있는지 확인
	{
		AimPointUI = CreateWidget<UCJS_AimPointWidget>(GetWorld(), WBP_AimPoint);
		if (AimPointUI) 
		{
			AimPointUI->AddToViewport();
			AimPointUI->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT("AimPointUI successfully created and added to viewport & Hidden right now"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create AimPointUI Widget"));
		}
		UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::BeginPlay()::WBP_AimPoint is assigned!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_UserCharacter::BeginPlay()::WBP_AimPoint is not assigned! Please assign it in the Blueprint."));
	}
	
	bAimPointUIShowing = false;
}

// Called every frame
void ACJS_BallPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Ensure direction is updated and normalized before applying movement
	FTransform dir = FTransform(GetControlRotation());
	Direction = dir.TransformVector(Direction);
	Direction.Z = 0;
	Direction.Normalize();
	AddMovementInput(Direction);
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::Tick() - Moving in Direction: X=%f, Y=%f, Z=%f"), Direction.X, Direction.Y, Direction.Z);
	Direction = FVector::ZeroVector;  // Reset direction after movement input is added
}


// Called to bind functionality to input  =========================================================================================================================================
void ACJS_BallPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}

	UEnhancedInputComponent* input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if (input)
	{
		// 이동
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ACJS_BallPlayer::OnMyActionMove);
		// 방향
		input->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ACJS_BallPlayer::OnMyActionLook);
		// 점프
		input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionJump);
		// 던지기
		input->BindAction(IA_Throw, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionThrow);
		// 클릭
		input->BindAction(IA_Click, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionClick);
		// 토글 조준점
		input->BindAction(IA_AimPoint, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionToggleAimPointUI);
		// 숫자키 애니메이션 (인덱스 사용해 바인딩)
		for (int32 i = 0; i < 8; i++)
		{
			if (IA_NumKeys[i])
			{
				// 로그 출력 추가
				UE_LOG(LogTemp, Warning, TEXT("Binding action for Key %d with Index %d"), i, i);
				input->BindAction(IA_NumKeys[i], ETriggerEvent::Started, this, &ACJS_BallPlayer::OnNumberKeyPressed, i);
			}
			else
			{
				// 로그 출력: 인풋 액션이 null일 경우
				UE_LOG(LogTemp, Error, TEXT("IA_NumKeys[%d] is null"), i);
			}
		}


		// Log to check if input actions are bound
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::SetupPlayerInputComponent() - Input actions are bound"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::SetupPlayerInputComponent():: EnhancedInputComponent is null"));
	}
}
void ACJS_BallPlayer::OnMyActionMove(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionMove()"));

	FVector2D v = Value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
	Direction.Normalize();

	// Log to check if the input value is being received
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionMove():: Move Direction: X=%f, Y=%f"), Direction.X, Direction.Y);
}
void ACJS_BallPlayer::OnMyActionLook(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionLook()"));

	FVector2D v = Value.Get<FVector2D>();

	// Log to check if the look input is being received
	//UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionLook():: Look Direction: X=%f, Y=%f"), v.X, v.Y);

	AddControllerPitchInput(-v.Y);
	AddControllerYawInput(v.X);
}
void ACJS_BallPlayer::OnMyActionJump(const FInputActionValue& Value)
{
	Jump();
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionJump() - Jump Triggered"));
}
void ACJS_BallPlayer::OnMyActionThrow(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionThrow()"));
	if (HeartItemFactory)
	{
		// Get the spawn location and rotation from the actor
		FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * HeartSpawnPosition.X + GetActorUpVector() * HeartSpawnPosition.Z;
		FRotator SpawnRotation = GetActorRotation();

		// Spawn the heart actor
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		ACJS_HeartActor* SpawnedHeart = GetWorld()->SpawnActor<ACJS_HeartActor>(HeartItemFactory, SpawnLocation, SpawnRotation, SpawnParams);

		if (SpawnedHeart && SpawnedHeart->ProjectileMovementComp)
		{
			// Apply an initial impulse to make the heart fly forward
			FVector LaunchDirection = SpawnRotation.Vector();
			SpawnedHeart->ProjectileMovementComp->Velocity = LaunchDirection * SpawnedHeart->ProjectileMovementComp->InitialSpeed;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionThrow() - HeartItemFactory is null"));
	}
}

void ACJS_BallPlayer::OnMyActionClick(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()"));

	if (!bAimPointUIShowing)
	{
		// AimPointUI가 표시되지 않았을 때는 클릭을 무시
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionClick()::bAimPointUIShowing==false return"));
		return;
	}

	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 100000.0f;
	FHitResult Outhit;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Outhit, Start, End, TraceChannel, CollisionParams);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit something!"));

		if (Outhit.Component.IsValid())
		{
			FString HitComponentName = Outhit.Component->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Hit Component: %s"), *HitComponentName);
		}

		AActor* HitActor = Outhit.GetActor();
		if (HitActor)
		{
			FString HitActorName = HitActor->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActorName);

			if (HitActorName.Contains("BP_MultiRoom"))
			{
				UE_LOG(LogTemp, Warning, TEXT("BP_MultiRoom Clicked"));
				if (PC)
				{
					RequestMapTravel("/Game/CJS/Maps/CJS_MultiRoomMap");
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr. Cannt Move to the MultiRoomMap"));
				}
			}
			else if (HitActorName.Contains("BP_MyRoom"))
			{
				UE_LOG(LogTemp, Warning, TEXT("BP_MyRoom Clicked"));
				if (PC)
				{
					RequestMapTravel("/Game/CJS/Maps/CJS_MyRoomMap");
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("PlayerController is nullptr. Cannt Move to the MyRoomMap"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()::Hit Actor is NULL"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_UserCharacter::OnMyActionClick()::No Hit Detected"));
	}

}

void ACJS_BallPlayer::OnMyActionToggleAimPointUI(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionToggleAimPointUI()"));

	if (AimPointUI)
	{
		if (!bAimPointUIShowing)
		{
			bAimPointUIShowing = true;
			AimPointUI->SetVisibility(ESlateVisibility::Visible);
			UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Visible, Visibility State: %d"), (int32)AimPointUI->GetVisibility());
		}
		else
		{
			bAimPointUIShowing = false;
			AimPointUI->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT("AimPointUI is now Hidden, Visibility State: %d"), (int32)AimPointUI->GetVisibility());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AimPointUI is null! UI cannot be toggled."));
	}
}


//void ACJS_BallPlayer::OnMyActionKey1(const FInputActionValue& Value)
//{
//	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey1()"));
//	/*if (AnimInstance)
//	{	
//		AnimInstance->PlayAngryMontage();
//		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey1() - Angry animation played"));
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionKey1() - AnimInstance is null"));
//	}*/
//
//	// 애니메이션 재생 함수 호출
//	PlayTestAnimation();
//}


void ACJS_BallPlayer::OnNumberKeyPressed(const FInputActionValue& Value, int32 KeyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnNumberKeyPressed() - Key %d pressed"), KeyIndex + 1);
	PlayAnimationByIndex(KeyIndex);
}

//void ACJS_BallPlayer::PlayTestAnimation()
//{
//	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::void ACJS_BallPlayer::PlayTestAnimation()"));
//
//	if (TestAnimSequence && GetMesh())
//	{
//		// Skeletal Mesh Component에서 애니메이션 재생
//		GetMesh()->PlayAnimation(TestAnimSequence, false);
//		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::PlayTestAnimation() - Animation played"));
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::PlayTestAnimation() - TestAnimSequence or SkeletalMeshComponent is null"));
//	}
//}

void ACJS_BallPlayer::PlayAnimationByIndex(int32 Index)
{
	if (AnimSequences.IsValidIndex(Index) && AnimSequences[Index] && GetMesh())
	{
		GetMesh()->PlayAnimation(AnimSequences[Index], false);
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::PlayAnimationByIndex() - Animation %d played"), Index + 1);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::PlayAnimationByIndex() - Invalid index or animation sequence"));
	}
}

// ========================================================================================================================================================


// 공끼리 충돌 시  =========================================================================================================================================
void ACJS_BallPlayer::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	// Check if the hit actor is another BallPlayer
	if (Other && Other != this && Other->IsA(ACJS_BallPlayer::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::NotifyHit() - Collided with another BallPlayer"));

		// Activate particle effect at the hit location
		if (HitVFXFactory)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitVFXFactory, HitLocation);
		}
		if (HitSFX)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), HitSFX);
		}
		TriggerSelfHitEffects(HitLocation);
	}
}
void ACJS_BallPlayer::TriggerSelfHitEffects(FVector HitLocation)
{
	// Log to indicate collision effects triggered
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::TriggerCollisionEffects() - Triggered on collision"));

	// Activate particle effect at the hit location
	if (SelfHitVFXFactory)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelfHitVFXFactory, HitLocation);
	}
	//// Play sound effect
	//if (HitSFX)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSFX, HitLocation);
	//}
}


// 방 클릭 시 (클라 이동) ========================================================================================================================================
void ACJS_BallPlayer::RequestMapTravel(const FString& MapPath)
{
	if (PC && !HasAuthority())  // 클라이언트만 요청
	{
		ServerRPC_RequestMapTravel(MapPath);
	}
}

void ACJS_BallPlayer::ServerRPC_RequestMapTravel_Implementation(const FString& MapPath)
{
	float StartYValue = 0.0f; // 시작 Y 값
	float YOffsetIncrement = 100.0f; // 각 클라이언트마다 Y 값 증가량
	int32 ClientIndex = 0; // 클라이언트 인덱스

	// 서버가 모든 플레이어 컨트롤러를 탐색합니다.
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* OtherPC = Iterator->Get();

		// 서버 겸 클라이언트가 아닌 일반 클라이언트만 이동합니다. 서버 겸 클라이언트(호스트)는 ROLE_Authority
		if (OtherPC && OtherPC->GetRemoteRole() == ROLE_AutonomousProxy)
		{
			// listen 파라미터 없이 이동하여 기존 서버 세션을 따르게 합니다.		
			//OtherPC->ClientTravel(MapPath, ETravelType::TRAVEL_Absolute);  // <-- 서버가 같은 공간에 있는 게 아니라서 계속 따로 이동함

			// 그래서 그냥 위치 이동하는 걸로 변경해 봄
			APawn* ControlledPawn = OtherPC->GetPawn();
			if (ControlledPawn)
			{
				// 캐릭터의 위치를 변경합니다.
				//ControlledPawn->SetActorLocation(FVector(9950.0f, 0.0f, 0.0f));

				// 각 클라이언트마다 다른 Y 값을 사용하여 위치를 변경합니다.
				FVector NewLocation(9950.0f, StartYValue + (YOffsetIncrement * ClientIndex), 0.0f);
				ControlledPawn->SetActorLocation(NewLocation);
				ClientIndex++; // 다음 클라이언트를 위해 인덱스 증가
			}
		}
	}
}

