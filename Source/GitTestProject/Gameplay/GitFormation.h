// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GitFormation.generated.h"

/**
 * 
 */
UCLASS()
class GITTESTPROJECT_API UGitFormation : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (Catergory = "Git"))
	static TArray<FVector> GetLineFormation(const int32 UnitCount, const float UnitSizeX, const float UnitSizeY, const FVector UnitLocation, const FVector ForwardDirection);
	
	
	
};
