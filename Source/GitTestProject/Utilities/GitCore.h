// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GitCore.generated.h"

#define PRINT(text) GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor(0, 166, 255), text)
#define PRINT_INT(Integer) GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor(0, 166, 255), FString::FromInt(Integer))
#define PRINT_FLOAT(Float) GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.0f, FColor(0, 166, 255), FString::SanitizeFloat(Float))

/**
 * 
 */
UCLASS()
class GITTESTPROJECT_API UGitCore : public UObject
{
	GENERATED_BODY()
	
	
	
	
};
