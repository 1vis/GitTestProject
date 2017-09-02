// Fill out your copyright notice in the Description page of Project Settings.

#include "GitStatics.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine.h"
#include "Player/GitPlayerController.h"
#include "Kismet/GameplayStatics.h"

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

AGitPlayerController * UGitStatics::GetGitPlayerController(const UObject* WorldContextObject, int32 Index /*= 0 */)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, Index);
	return Cast<AGitPlayerController>(PC);
}

//template<class Type>
//bool UGitStatics::FindNextValidIndex(const TArray<Type>& Array, const int32 StartAtIndex, int32& OutIndex)
//{
//	if (Array.Num() <= 0)
//	{
//		return false;
//	}
//	
//	int32 CurrentIndex = StartAtIndex;
//	while (Array.IsValidIndex(CurrentIndex) == false)
//	{
//		if (CurrentIndex < Waypoints.Num())
//		{
//			CurrentGoalIndex++;
//		}
//		else
//		{
//			CurrentGoalIndex = 0;
//		}
//
//		if (CurrentIndex == IndexAtBeginning)
//		{
//			return false;			
//		}
//	}
//
//	OutIndex == CurrentIndex;
//	return true;
//}
