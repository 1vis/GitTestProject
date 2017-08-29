// Fill out your copyright notice in the Description page of Project Settings.

#include "GitFormation.h"
#include "Utilities/GitCore.h"
#include "Kismet/KismetMathLibrary.h"

TArray<FVector> UGitFormation::GetLineFormation(const int32 UnitCount, const float UnitSizeX, const float UnitSizeY, const FVector UnitLocation, const FVector ForwardDirection)
{
	TArray<FVector> Positions;

	if (UnitCount <= 0)
	{
		return Positions;
	}	

	int32 UnitCountInRow = FMath::CeilToInt(FMath::Sqrt(UnitCount));	
	int32 UnitCountInLastRow = UnitCount % UnitCountInRow;

	float PosX = 0.0f;
	float PosY = 0.0f;	
	float CurrentOffsetX = 0.0f;
	float X_Offset = ((UnitCountInRow - UnitCountInLastRow) / 2.0f) * UnitSizeX;

	for (int32 i = 0; i < UnitCount; ++i)
	{
		bool bIsInLastRow = i >= UnitCount - UnitCountInLastRow;
		if (bIsInLastRow)
		{			
			CurrentOffsetX = X_Offset;
		}

		Positions.Add(FVector(PosX + CurrentOffsetX, PosY, 0.0f));

		PosX += UnitSizeX;		

		if ((i + 1) % UnitCountInRow == 0)
		{
			PosY += UnitSizeY;
			PosX = 0.0f;
		}
	}	

	FRotator Rotator = UKismetMathLibrary::MakeRotFromX(ForwardDirection);
	Rotator += FRotator(0, 90, 0);
	for (auto& Position : Positions)
	{
		Position = Rotator.RotateVector(Position);
	}

	FVector Pivot = UKismetMathLibrary::GetVectorArrayAverage(Positions);
	for (auto& Position : Positions)
	{
		Position = UnitLocation + Position;
		Position -= Pivot;
	}

	return Positions;
}
