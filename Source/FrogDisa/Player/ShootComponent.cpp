// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootComponent.h"
#include "Camera/CameraComponent.h"
//#include "UObject/ConstructorHelpers.h"
// Sets default values for this component's properties
UShootComponent::UShootComponent()
{
	ConstructorHelpers::FClassFinder<AThrowProjectile> projectile(TEXT("Class'/Script/FrogDisa.ThrowProjectile'"));
	ProjectileWrench = projectile.Class;

	ConstructorHelpers::FClassFinder<AThrowProjectile> projectiles(TEXT("/Game/Blueprint/BP_MyThrowProjectile"));
	BlueprintWrench = projectiles.Class;
	
	ConstructorHelpers::FClassFinder<AThrowingStone> stoneProjectiles(TEXT("/Game/Blueprint/BP_ThrowingStone"));
	StoneClass = stoneProjectiles.Class;
}


// Called when the game starts
void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Wrench = GetWorld()->SpawnActor<AThrowProjectile>(BlueprintWrench, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform(), SpawnParameters);
	Wrench->AttachToPlayerCharacter(GetOwner());

}

void UShootComponent::ThrowProjectile(EProjectiles projectile_type)
{
	if (projectile_type == EP_Wrench)
		Wrench->Launch();
	else
	{
		FHitResult hitPoint;
		FCollisionQueryParams CollisionParams;
		FVector Start = GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentLocation(), End = GetOwner()->FindComponentByClass<UCameraComponent>()->GetForwardVector() * 5000 + Start;
		if (GetWorld()->LineTraceSingleByChannel(hitPoint, Start, End, ECC_Visibility, CollisionParams) == true)
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			StoneActor = GetWorld()->SpawnActor<AThrowingStone>(StoneClass, GetOwner()->FindComponentByClass<UCameraComponent>()->GetComponentTransform(), SpawnParameters);
			StoneActor->ThrowStone(hitPoint.Location - Start);
		}
		
	}

}

void UShootComponent::ReturnProjectile()
{
	Wrench->ReturnToCharacter();
}

AActor *UShootComponent::GetActorWrench()
{
	return Cast<AActor>(Wrench);
}



