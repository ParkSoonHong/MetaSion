// Fill out your copyright notice in the Description page of Project Settings.


#include "CJS/CJS_HttpActor.h"

#include "HttpModule.h"
#include "JsonParseLib.h"
#include "CJS/CJS_BallPlayer.h"

// Sets default values
ACJS_HttpActor::ACJS_HttpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACJS_HttpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACJS_HttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACJS_HttpActor::ReqPostClickMultiRoom(FString url, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	req->OnProcessRequestComplete().BindUObject(this, &ACJS_HttpActor::OnResPostClickMultiRoom);

	req->ProcessRequest();
}
void ACJS_HttpActor::OnResPostClickMultiRoom(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnResPosLogin OK... %d"), Response->GetResponseCode());

		int32 res = Response->GetResponseCode();
		if (res == 200)
		{
			UE_LOG(LogTemp, Warning, TEXT("Response ... OK!! "));
			FString str = Response->GetContentAsString();
			UE_LOG(LogTemp, Warning, TEXT(" %s"), *str);

			// 플레이어 캐스팅 및 RequestMoveMultiRoom 호출
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				ACJS_BallPlayer* player = Cast<ACJS_BallPlayer>(PlayerController->GetPawn());
				if (player)
				{
					player->RequestMoveMultiRoom(PlayerController);
					UE_LOG(LogTemp, Warning, TEXT("RequestMoveMultiRoom called successfully."));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Failed to cast PlayerController's Pawn to ACJS_BallPlayer."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to get PlayerController."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Response ... not OK!! "));	
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed...%d"), Response->GetResponseCode());
	}
}

