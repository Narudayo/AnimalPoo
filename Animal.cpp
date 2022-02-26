// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"
#include "Poop.h"
#include "Components/SphereComponent.h"
#include <Engine/Classes/Components/CapsuleComponent.h>
#include <Engine/Classes/GameFramework/CharacterMovementComponent.h>
#include "GameFramework/SpringArmComponent.h"
#include <Engine/Classes/Components/SkeletalMeshComponent.h>
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"


// Sets default values
AAnimal::AAnimal()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// beginplay함수가 시작한 직후에 Tick함수가 동작하도록 만들어준다.
	PrimaryActorTick.bStartWithTickEnabled = true;

	// 캡슐의 사이즈를 정해준다.
	GetCapsuleComponent()->InitCapsuleSize(100.0f, 100.0f);

	// 3값들을 false로 줘서 캐릭터가 카메라의 회전에 따라 회전하지 않게 해준다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터의 이동을 평면으로 제한
	GetCharacterMovement()->bConstrainToPlane = true;

	// 시작시 캐릭터가 평면이 아니라면 평면에서 시작하도록.
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	AssHole = CreateDefaultSubobject<USphereComponent>(TEXT("AssHole"));
	AssHole->SetupAttachment(GetRootComponent());


	// -------------- [카메라 세팅] ------------------
	// SpringArm 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	// SpringArm의 회전이 RootComponent나 상위컴포넌트를 따르지 않고 월드 좌표계의 회전에 따르도록
	SpringArm->SetUsingAbsoluteRotation(true);

	SpringArm->TargetArmLength = 500.0f;	// 캐릭터와의 거리
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	SpringArm->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));	// Pitch,Yaw,Roll

	// bDoCollisionTest -> 카메라가 벽에 닿으면 충돌 대상을 통해 카메라가 벽을 뚫지 않도록 만들어 줌.
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	//카메라 회전이 pawn에 영향을 주지 않도록 설정.
	Camera->bUsePawnControlRotation = false;
	// -------------- [카메라 세팅 끝] ------------------

	// ----------------- [메쉬 세팅] ---------------------

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -75.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> NewMesh(TEXT("SkeletalMesh'/Game/Characters/Ch03/Ch03_nonPBR'"));
	//GetMesh()->SetSkeletalMesh(NewMesh.Object);

	// ----------------- [메쉬 세팅 끝] ---------------------

	// ---------- [캐릭터 스텟] ----------

	WalkSpeed = 300.f;
	MoveSpeed = WalkSpeed;
	SprintingValue = 2.0f;
	SprintSpeed = WalkSpeed * SprintingValue;
	TurnSpeed = 2.f;
	PoopScale = 1.f;
	PoopSpeed = 1.f;

	MaxHungry = 100.f;
	MaxNeedToPooty = 100.f;
	CurrHungry = 80.f;
	CurrNeedToPooty = 30.f;
	// ---------- [캐릭터 스텟 끝] ----------

	MovementStatus = EMovementStatus::EMS_Stand;
	ConditionStatus = EConditionStatus::ECS_Alive;
	bLMBdown = false;

	
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller != NULL)
	{
		// -------- 마우스 방향 따라 회전 --------- //
		FVector mouseLocation, mouseDirection;

		APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		playerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
		

		FRotator currRotation = this->GetActorRotation();
		FRotator targetRotation = mouseDirection.Rotation();
		FRotator YawRotation = FRotator(currRotation.Pitch, targetRotation.Yaw, currRotation.Roll);

		//FRotator newRotation = FRotator(currRotation.Pitch, targetRotation.Yaw, currRotation.Roll);
		
		FRotator SmoothRotator = FMath::RInterpTo(currRotation, YawRotation, DeltaTime, TurnSpeed);
		this->SetActorRotation(SmoothRotator);

		// -------- 마우스 방향 따라 회전 End--------- //

		AAnimal::MoveForward(MoveSpeed);
	}

	//FString string  = FString::Format(TEXT())

	// 캐릭터가 살아있을 때
	if (is_Alive())
	{

		if (bLMBdown)
		{
			MovementStatus = EMovementStatus::EMS_Sprint;
			//UE_LOG(LogTemp, Warning, TEXT("sprinting"));
			//UE_LOG(LogTemp, Warning, TEXT("MaxSpeed : %.3f"), GetCharacterMovement()->GetMaxSpeed());
			SetMovementStatus(MovementStatus);
		}
			
		if (!bLMBdown)
		{
			MovementStatus = EMovementStatus::EMS_Walk;
			//UE_LOG(LogTemp, Warning, TEXT("walking"));
			//UE_LOG(LogTemp, Warning, TEXT("MaxSpeed : %.3f"), GetCharacterMovement()->GetMaxSpeed());
			SetMovementStatus(MovementStatus);
		}
	}
	// 캐릭터가 죽었을 때
	else if(!is_Alive())
	{
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
	}
	
}

// Called to bind functionality to input
void AAnimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &AAnimal::LMBdown);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &AAnimal::LMBup);
}

void AAnimal::LMBdown()
{
	//UE_LOG(LogTemp, Warning, TEXT("LMBdown"));
	bLMBdown = true;
}

void AAnimal::LMBup()
{
	//UE_LOG(LogTemp, Warning, TEXT("LMBup"));
	bLMBdown = false;
}

void AAnimal::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		const FRotator Rotation = GetActorRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAnimal::SetMovementStatus(EMovementStatus Status)
{
	//UE_LOG(LogTemp, Warning, TEXT("In Set"));
	switch (Status)
	{
	case EMovementStatus::EMS_Stand:
	{
		//UE_LOG(LogTemp, Warning, TEXT("In Stand"));
		MoveSpeed = 0.f;
		break;
	}
	case EMovementStatus::EMS_Walk:
	{
		//UE_LOG(LogTemp, Warning, TEXT("In Walk"));
		SprintToWalk(0.3f);
		//MoveSpeed = WalkSpeed;
		break;
	}
	case EMovementStatus::EMS_Sprint:
	{
		//UE_LOG(LogTemp, Warning, TEXT("In Sprint"));
		WalkToSprint(0.5f);
		//MoveSpeed = SprintSpeed;
		break;
	}
	}
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}


// param -> Time : 몇 초만에 max를 찍는지, ex -> Time = 1.5면 1.5초만에 해당 max를 찍는다는 뜻.
// FPS의 기준마다 달라짐. 22/01/29 기준 우선은 100으로 고정해놓음.
void AAnimal::WalkToSprint(float Time)
{
	//UE_LOG(LogTemp, Warning, TEXT("WalkToSprint"));
	const int FPS = 100;
	float frame = Time * FPS;	

	float SpeedRange = SprintSpeed - WalkSpeed;
	//UE_LOG(LogTemp, Warning, TEXT("V : %f"), SpeedRange / frame);

	if (MoveSpeed <= SprintSpeed)
	{
		MoveSpeed += (SpeedRange / frame);
	}
	else MoveSpeed = SprintSpeed;
}

void AAnimal::SprintToWalk(float Time)
{
	//UE_LOG(LogTemp, Warning, TEXT("SprintToWalk"));
	const int FPS = 100;
	float frame = Time * FPS;

	float SpeedRange = SprintSpeed - WalkSpeed;
	//UE_LOG(LogTemp, Warning, TEXT("V : %f"), SpeedRange / frame);
	//UE_LOG(LogTemp, Warning, TEXT("frame : %f"), frame);

	if (MoveSpeed >= WalkSpeed)
	{
		MoveSpeed -= (SpeedRange / frame);
	}
	else MoveSpeed = WalkSpeed;
}

void AAnimal::Depth()
{
	if (DepthSound) {
		UGameplayStatics::PlaySound2D(this, DepthSound);
	}
	UE_LOG(LogTemp, Warning, TEXT("You Are Die."));

	for (auto& poop : MyPoops) poop->Disappear();
	//MyPoops.Empty();
}

bool AAnimal::is_Alive()
{
	if (ConditionStatus == EConditionStatus::ECS_Alive)	return true;
	if (ConditionStatus == EConditionStatus::ECS_Depth) return false;
	return false;
}

/*
void AAnimal::AddMyPoop(APoop* poop)
{
	MyPoops.Add(poop);
}
*/