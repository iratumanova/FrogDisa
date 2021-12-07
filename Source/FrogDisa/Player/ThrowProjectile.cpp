// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "Movement.h"
#include "FrogDisa/DropItAfterShot.h"
#include "FrogDisa/BreakingObject.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
AThrowProjectile::AThrowProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> mesh(TEXT("StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Axe.SM_Axe'"));
	ProjectileMesh -> SetStaticMesh(mesh.Object);
	ProjectileMesh->SetGenerateOverlapEvents(true);
	ProjectileMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ProjectileMesh->SetupAttachment(RootComponent);

	OnActorBeginOverlap.AddDynamic(this, &AThrowProjectile::OnOverlap);

}

// Called when the game starts or when spawned
void AThrowProjectile::BeginPlay()
{
	Super::BeginPlay();
	inAir = false;
	isLaunched = false;
	projectileIsReturning = false;
}

void AThrowProjectile::AttachToPlayerCharacter(AActor* Character)
{
	
	OwnerPlayer = Character;
	ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(),FAttachmentTransformRules::KeepWorldTransform,TEXT("hand_RSocket"));
	SetActorRelativeLocation(FVector::ZeroVector);
}

void AThrowProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThrowProjectile::ReturnToCharacter()
{
	if (projectileIsReturning == false && isLaunched == true)
	{
		GetWorldTimerManager().ClearTimer(MoveTimer);
		EndLocation = OldStartLocation;
		projectileIsReturning = true;
		inAir = true;
		halfDistance = GetDistanceTo(OwnerPlayer) / 1.4;
		GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AThrowProjectile::Move, 0.01f, true, 0.f);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("ReturnSt"));
	}
}

void AThrowProjectile::ReturnStart()
{

}

void AThrowProjectile::Launch()
{
	if (isLaunched == false)
	{

		DetachRootComponentFromParent(true);
		StartLocation = GetActorLocation();

		SetActorRotation(Cast<AMovement>(OwnerPlayer)->_Camera->GetComponentRotation());
		EndLocation = GetActorLocation() + GetActorForwardVector() * 1050.f;

		MoveProjectileVector = GetActorForwardVector();
		halfDistance = FVector::Distance(EndLocation, GetActorLocation()) / 1.4;

		GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AThrowProjectile::Move, 0.01f, true, 0.f);
		isLaunched = true;
		inAir = true;
	}
}


void AThrowProjectile::Move()
{
	if (projectileIsReturning)
	{
		
		RuleToMove(1);//1 is a left direction and -1 is a right direction

		if (FVector::Distance(Cast<AMovement>(OwnerPlayer)->GetMesh()->GetSocketLocation(TEXT("hand_RSocket")), GetActorLocation()) < 7)
		{
			ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			ProjectileMesh->AttachToComponent(Cast<AMovement>(OwnerPlayer)->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("hand_RSocket"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Returned"));
			GetWorldTimerManager().ClearTimer(MoveTimer);
			Cast<AMovement>(OwnerPlayer)->SetUnwaitingState();
			SetActorRotation(OwnerPlayer->GetActorRotation() + FRotator(-15, 0, 0));
			projectileIsReturning = false;
			isLaunched = false;
			inAir = false;
		}
	}
	else
	{
		if (inAir)
		{

			RuleToMove(-1);
			if (FVector::Distance(StartLocation, GetActorLocation()) >= 1000.f)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("ReturnSt"));
				ReturnToCharacter();
			}
		}
	}

}

void AThrowProjectile::RuleToMove(int direction)
{
	if (direction == 1)
	{
		EndLocation = Cast<AMovement>(OwnerPlayer)->GetMesh()->GetSocketLocation(TEXT("hand_RSocket"));
	}

	currentDistance = FVector::Distance(EndLocation, GetActorLocation());
	float currentAlpha;
	currentAlpha = FMath::Abs(halfDistance - currentDistance) / (halfDistance / 2);
	currentAlpha = FMath::Clamp(currentAlpha, 0.f, 1.f);

	FVector nextRightVector = EndLocation - GetActorRightVector() * UKismetMathLibrary::Lerp(halfDistance, 0, currentAlpha) * direction / 2.f;

	FVector nextLoc = UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), nextRightVector, 0.1f, 200.f);

	SetActorLocation(nextLoc);
	AddActorLocalRotation(FRotator(-10, 0, 0), false);
}

void AThrowProjectile::Stop()
{
	if (projectileIsReturning == false && inAir)
	{
		SetActorRotation(FRotator(OwnerPlayer->GetActorRotation().Pitch - 15, GetActorRotation().Yaw, GetActorRotation().Roll));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("STOOOOOOOOOOP"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::SanitizeFloat(GetActorRotation().Pitch));
		inAir = false;
	}
}

void AThrowProjectile::OnOverlap_Implementation(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor)
	{
		bool cast = Cast<ADropItAfterShot>(OtherActor) == nullptr && Cast<ABreakingObject>(OtherActor) == nullptr;

		if (Cast<ADropItAfterShot>(OtherActor))
		{
			if (cast && Cast<ADropItAfterShot>(OtherActor)->isABreakableMesh() == false)
			{
				AThrowProjectile::ReturnToCharacter();
			}
		}
		else
		{
			if (cast)
			{
				AThrowProjectile::ReturnToCharacter();
			}
		}
	}
}


bool AThrowProjectile::GetInAirState()
{
	return inAir;
}

AActor* AThrowProjectile::GetOwnerPlayer()
{
	return OwnerPlayer;
}