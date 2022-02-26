// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AnimalPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALPOO_API AAnimalPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	void LookAtCursor();

protected:
	virtual void BeginPlay() override;
};
