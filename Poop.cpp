// Fill out your copyright notice in the Description page of Project Settings.


#include "Poop.h"
#include "Animal.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"


APoop::APoop()
{
	is_ripened = false;

}

void APoop::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AAnimal* Animal = Cast<AAnimal>(OtherActor);
		if (Animal)
		{
			UE_LOG(LogTemp, Warning, TEXT("Animal OnOverlap Begin"));
			if (is_ripened)
			{
				Animal->Depth();
				Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
			}
		}
	}
}

void APoop::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	//UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapEnd()"));
}

void APoop::Disappear()
{
	if (DisAppearParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DisAppearParticles, GetActorLocation(), FRotator(0.f), true);
	}
	//Destroy();
}