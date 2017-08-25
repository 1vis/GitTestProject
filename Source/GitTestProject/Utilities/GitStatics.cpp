// Fill out your copyright notice in the Description page of Project Settings.

#include "GitStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine.h"

FVector2D UGitStatics::GetViewportSize()
{
	return GEngine->GameViewport->Viewport->GetSizeXY();
}

FVector2D UGitStatics::GetMousePosition(APlayerController * PC)
{
	float MouseX = 0.0f;
	float MouseY = 0.0f;
	PC->GetMousePosition(MouseX, MouseY);

	return FVector2D(MouseX, MouseY);
}
