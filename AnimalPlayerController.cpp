// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/world.h"

void AAnimalPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void LookAtCursor()
{
	/*
	APlayerController* Controller = UGameplayStatics::GetPlayerController(GEngine->GetWorld(), 0);
	if (Controller)
	{
			FVector mouseLocation, mouseDirection;
		}
		PC->DeprojectMousePositionToWorld();
	*/


	// 아래는 멀티플레이어 전용
	// for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
}

