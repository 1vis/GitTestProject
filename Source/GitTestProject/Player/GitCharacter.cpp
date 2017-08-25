// Fill out your copyright notice in the Description page of Project Settings.

#include "GitCharacter.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGitCharacter::AGitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGitCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGitCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGitCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGitCharacter::NotifyActorOnReleased(FKey ButtonReleased)
{
	Super::NotifyActorOnReleased(ButtonReleased);
}


FVector AGitCharacter::GetDestination(UCameraComponent* Camera)
{
	if (!Camera)
	{
		return FVector();
	}

	check(Camera && "Camera not found. (AGitCharacter)");
		
	FVector WorldLocation,WorldDirection;
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	WorldDirection *= 10'000.0f;	
		
	FVector TraceStart = Camera->GetComponentLocation();
	FVector TraceEnd = TraceStart + WorldDirection;
		
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	
	Destination = HitResult.Location;
	return Destination;
}
