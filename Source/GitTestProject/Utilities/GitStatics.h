// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GitStatics.generated.h"

/**
 * 
 */
UCLASS()
class GITTESTPROJECT_API UGitStatics : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, meta = (Category = "GitStatics"))
	static FVector2D GetViewportSize();

	UFUNCTION(BlueprintPure, meta = (Category = "GitStatics"))
	static FVector2D GetMousePosition(APlayerController* PC);

	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject", Category = "GitStatics"))
	static class AGitPlayerController* GetGitPlayerController(const UObject* WorldContextObject, int32 Index = 0);

	
	
};
