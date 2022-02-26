// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Poop.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Animal.generated.h"


UENUM(BlueprintType)
enum class EMovementStatus : uint8
{	
	// 캐릭터 이동 스테이터스.
	EMS_Stand UMETA(DisplayName = "Stand"),
	EMS_Walk UMETA(DisplayName = "Walk"),
	EMS_Sprint UMETA(DisplayName = "Sprint"),

	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EConditionStatus : uint8
{
	// 캐릭터 컨디션 스테이터스.

	ECS_Alive UMETA(DisplayName = "Alive"),
	ECS_Depth UMETA(DisplayName = "Depth"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class ANIMALPOO_API AAnimal : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAnimal();


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		class USoundCue* DepthSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "poop")
		class USphereComponent* AssHole;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "poop")
		TArray<APoop*> MyPoops;


	// -------- [캐릭터 statistics] -------- //

	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float SprintSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float SprintingValue;	// 달리면 얼만큼 빨라지는지.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float TurnSpeed;		// 회전 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float PoopScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float PoopSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float MaxHungry;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
		float MaxNeedToPooty;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float CurrHungry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
		float CurrNeedToPooty;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status Enums")
		EMovementStatus MovementStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status Enums")
		EConditionStatus ConditionStatus;

	void SetMovementStatus(EMovementStatus Status);

	// -------- [캐릭터 statistics 끝] -------- //

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);	// 앞으로 이동

	// 지금까지 싼 똥 기록
	UFUNCTION(BlueprintCallable, Category = "poop")
	void AddMyPoop(APoop* poop) { MyPoops.Add(poop); }

	void Depth();		// 죽음
	bool is_Alive();

	bool bLMBdown;
	void LMBdown();		// 왼쪽 마우스 클릭
	void LMBup();		// 왼쪽 마우스 떼기

	// AToB, A부터 B속도로 전환. Time -> 1.0f이면 약 1초에 걸쳐 전환된다는 뜻.

	void WalkToSprint(float Time);	
	void SprintToWalk(float Time);	

};
