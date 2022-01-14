#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ThrowProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "EProjectiles.h"
#include "ThrowingStone.h"
#include "ShootComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FROGDISA_API UShootComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShootComponent();

	void Fire();

	void ThrowProjectile(EProjectiles projectile_type);

	void ReturnProjectile();

		TSubclassOf<AThrowProjectile> BlueprintWrench;

		TSubclassOf<AThrowingStone> StoneClass;

		AActor* GetActorWrench();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:
	AThrowingStone* StoneActor;

	AThrowProjectile* Wrench;

};
