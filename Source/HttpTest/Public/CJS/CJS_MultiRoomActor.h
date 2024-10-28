// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CJS_MultiRoomActor.generated.h"

UCLASS()
class HTTPTEST_API ACJS_MultiRoomActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACJS_MultiRoomActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	 // Components
    UPROPERTY(VisibleAnywhere)
    class USphereComponent* SphereCollision;
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* SphereMesh;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* RefRoomInfoWidgetComp;


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_CurNumPlayer;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_MaxNumPlayer;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_RefRoomName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_RefPercent;

	// Function to update the widget text values
	void InitRefRoomInfoWidget(int32 CurNumPlayer, int32 MaxNumPlayer, const FString& RoomName, float Percent);

};
