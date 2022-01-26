#pragma once

#include "CoreMinimal.h"

#include "EProjectiles.h"
#include "SteamBug.h"
#include "ShootComponent.h"
#include "InteractiveComponent.h"
#include "FrogDisaGameInstance.h"
#include "UpdateBillboardComponent.h"
#include "InteractiveWithPuzzlesComponent.h"
#include "GrapplingComponent.h"

#include "AbilitySystemInterface.h"

#include "FrogDisa/GrapplingObject.h"
#include "FrogDisa/MovableObject.h"
#include "FrogDisa/InteractiveObject.h"
#include "FrogDisa/CharacterAttributeSet.h"

#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/HUD.h"

#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"

#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "Movement.generated.h"

//#define THIRD_PERSON


UCLASS()
class FROGDISA_API AMovement : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMovement();
	
	//UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool MeleeAttackIsActive;

		void SetUnwaitingState();

		void AddCollectibles() { Collectibles++; }
		void AddMoney(int money_count) {}//move it to ShopComponent later

		UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
			void DrawGrapplingVariant();
		
protected:
	// Called when the game starts or when spawned

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Action Functions default
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Attack();
	void Jump();
	void StopJumping();
	virtual void AddControllerYawInput(float Val);
	virtual void AddControllerPitchInput(float Val);

	void UseGrapplingHook();
#ifdef THIRD_PERSON
	void Aim(float Value);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* cameraComponent;
#endif
	void Run(float Value);

	void InteractionWithObject();
	void TakeCollectibles();
	bool CanMakeAction();

	void ChangeCharacter();

	void PauseMenu();

	UFUNCTION(BlueprintCallable)
		void SetMeleeAttackInactive();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool isClimbing;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void SetNewPosses();

	

	UUpdateBillboardComponent* TakenActorsComponent;

	void ChoiceWrench() { 
		g_Projectile_Type = EWeaponType::EW_Wrench; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Wrench"); 
		if(shootComponent)
			shootComponent->SwitchProjectile();
	}
	void ChoiceStone()  { g_Projectile_Type = EWeaponType::EW_Stone; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Stone");
	if(shootComponent)
		shootComponent->SwitchProjectile();
	}
	void ChoiceMine()   { g_Projectile_Type = EWeaponType::EW_Mine; GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Mine"); 
	if(shootComponent)
		shootComponent->SwitchProjectile();
	}

	UFUNCTION(BlueprintCallable)
		UGrapplingComponent* GetGrapplingComponent() { return grapplingComponent; }

	void ChangeCrouchMode();

	void SetCrouchModeSettings();
public:

	void DetachInteractiveObject();

	UFUNCTION(BlueprintCallable)
		bool GetWaitingState();
	UFUNCTION(BlueprintCallable)
		bool GetCrouchMode() { return isCrouching; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "meshComponent")
		UStaticMeshComponent* meshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShootComp")
		UShootComponent* shootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactive")
		UInteractiveComponent* InteractiveComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UFrogDisaGameInstance* game_Instance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UInteractiveWithPuzzlesComponent* InteractiveWithPuzzlesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UGrapplingComponent* grapplingComponent;
	//UPROPERTY(VisibleAnywhere, Category = "Gameplay")
	//	UAbilitySystemComponent* AbilityComponent;
	//UPROPERTY(Transient)
	//	UCharacterAttributeSet* AttributeSet;


	UFUNCTION(BlueprintCallable)
		void SetStartSettings(int countStone, int countCollectibles, bool isHaveBug, bool isBearObj);
	UFUNCTION(BlueprintCallable)
		bool GetAimingState();
	UFUNCTION(BlueprintCallable)
		bool GetRunningState();
	UFUNCTION(BlueprintCallable)
		AActor* GetThrowProjectile();

	UFUNCTION(BlueprintCallable)
		bool GetPauseState();
	UFUNCTION(BlueprintCallable)
		void SetUnPause();

	UFUNCTION(BlueprintCallable)
		bool IsHaveASteamBug();
	UFUNCTION(BlueprintCallable)
		bool IsBearObject();

	UFUNCTION(BlueprintCallable)
		int GetCountStones();

	UFUNCTION(BlueprintCallable)
		int GetCountCollectibles();
	UFUNCTION(BlueprintCallable)
	EWeaponType GetCurrentWeaponType();

	void LerpTo();
	UFUNCTION(BlueprintCallable)
		bool GetIsGrappling(){ return isGrappling; }
private:
	void ForwardTrace();
	void HeightTrace();
	void SwitchProjectile();
	void Fire();

	FTimeline MyTimeline;
	float CurveFloatValue;
	float TimelineValue;
	FTimerHandle GrapplingTimer;

	bool isBearObject;
	bool isAiming;
	bool isRunning;
	bool nearClimbingObject;
	bool isGrappling;
	bool circleMenuIsOpen;
	bool pauseMenuOpen;
	bool isWaitingWrench;
	bool HaveSteamBug;
	bool isCrouching;

	int Collectibles;
	int stoneCount;

	FVector endLoc;
	FVector wallNormal, wallLocation;

	const float DefaultSpeed = 400.f, RunningSpeed = DefaultSpeed + 200.f;
	const float ChangeTargetArmSpeed = 20.f;
	const float ThrowImpulseValue = 1500.f;
	const float DropImpulseValue = 0;
	const float GrapplingForceValue = 200.f;

	const float DefaultCapsuleHeight = 88;
	const float DefaultCameraHeight = 64;

	EWeaponType g_Projectile_Type;

	FCollisionQueryParams CollisionParams;
	UStaticMeshComponent* InteractiveObject;
	AMovableObject* movableActor;
	ASteamBug* SteamBug;
	FCollisionQueryParams queryParams;

	TSubclassOf<ASteamBug> SteamBug_ClassBP;
	TSubclassOf<ASteamBug> SteamBug_Class;

	friend class UInteractiveWithPuzzlesComponent;
	//friend class ACollectiblesObject;
	friend class UGrapplingComponent;
};
