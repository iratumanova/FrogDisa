// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleActor.h"
#include "TestPuzzleActor.generated.h"

UCLASS()
class FROGDISA_API ATestPuzzleActor : public APuzzleActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestPuzzleActor();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};