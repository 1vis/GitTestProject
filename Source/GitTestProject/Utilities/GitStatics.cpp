// Fill out your copyright notice in the Description page of Project Settings.

#include "GitStatics.h"

FVector2D UGitStatics::GetViewportSize()
{
	return GEngine->GameViewport->Viewport->GetSizeXY();
}


