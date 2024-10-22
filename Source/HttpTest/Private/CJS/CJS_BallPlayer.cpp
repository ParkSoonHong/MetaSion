// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_BallPlayer.h"
#include "CJS/CJS_HeartActor.h"
#include "CJS/CJS_BallPlayerAnimInstance.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ArrowComponent.h"
#include "Components/SkeletalMeshComponent.h"


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
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - Input mode set to Game Only"));

		// Possess the character if not already possessed
		if (PlayerController->GetPawn() != this)
		{
			PlayerController->Possess(this);
			UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - PlayerController has possessed the pawn"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay() - NO PlayerController"));
	}	

	// AnimInstance 초기화
	AnimInstance = Cast<UCJS_BallPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::BeginPlay() - AnimInstance initialized"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::BeginPlay() - AnimInstance is null"));
	}
	
	
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
		// 숫자키
		input->BindAction(IA_1, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionKey1);
		input->BindAction(IA_2, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionKey2);
		input->BindAction(IA_3, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionKey3);
		input->BindAction(IA_4, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionKey4);
		input->BindAction(IA_5, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionKey5);
		input->BindAction(IA_6, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionKey6);
		input->BindAction(IA_7, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionKey7);
		input->BindAction(IA_8, ETriggerEvent::Started, this, &ACJS_BallPlayer::OnMyActionKey8);

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
void ACJS_BallPlayer::OnMyActionKey1(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey1()"));
	if (AnimInstance)
	{	
		AnimInstance->PlayAngryMontage();
		UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey1() - Angry animation played"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ACJS_BallPlayer::OnMyActionKey1() - AnimInstance is null"));
	}
}
void ACJS_BallPlayer::OnMyActionKey2(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey2()"));
}
void ACJS_BallPlayer::OnMyActionKey3(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey3()"));
}
void ACJS_BallPlayer::OnMyActionKey4(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey4()"));
}
void ACJS_BallPlayer::OnMyActionKey5(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey5()"));
}
void ACJS_BallPlayer::OnMyActionKey6(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey6()"));
}
void ACJS_BallPlayer::OnMyActionKey7(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey7()"));
}
void ACJS_BallPlayer::OnMyActionKey8(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ACJS_BallPlayer::OnMyActionKey8()"));
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
// ========================================================================================================================================================
