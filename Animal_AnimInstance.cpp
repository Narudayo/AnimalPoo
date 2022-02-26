// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal_AnimInstance.h"

void UAnimal_AnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)	Pawn = TryGetPawnOwner();
	
}

void UAnimal_AnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)	Pawn = TryGetPawnOwner();
	if (Pawn) 
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
	}

}