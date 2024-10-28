// Fill out your copyright notice in the Description page of Project Settings.


#include "KGW/WBP_Image.h"

void UWBP_Image::PlayShowImageAnimation()
{
    if (ShowImage)
    {
        PlayAnimation(ShowImage, 0.0f, 1, EUMGSequencePlayMode::Forward ,3.0f, true );
        UE_LOG(LogTemp, Log, TEXT("ShowImage Animation is playing"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ShowImage Animation is nullptr"));
    }
}
