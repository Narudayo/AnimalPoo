// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Poop.generated.h"

/**
 * 
 */
UCLASS()
class ANIMALPOO_API APoop : public AItem
{
	GENERATED_BODY()
public:
	APoop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poop | Particles")
	class UParticleSystem* DisAppearParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poop | Status")
	bool is_ripened;

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	
	void Disappear();
	
};
