// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IKFootProject/IKFootProjectCharacter.h"
#include "IKFootProject/Library/StructLibrary.h"
#include "IKFootComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IKFOOTPROJECT_API UIKFootComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		FIKFootValues IKFootValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		bool bIsDebugging = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float FootHeight = 13.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
		float fTraceDistance = 35.f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		bool bLeftHit = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		bool bRightHit = false;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		FName LeftIKFootName;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		FName RightIKFootName;

	FHitResult LeftHit;
	FHitResult RightHit;

	UPROPERTY(meta = (AllowPrivateAccess = true))
		USkeletalMeshComponent* MeshComponent = nullptr;

	UPROPERTY(meta = (AllowPrivateAccess = true))
		AIKFootProjectCharacter* Character = nullptr;
	
public:	
	// Sets default values for this component's properties
	UIKFootComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void UpdateFootLocation(FName IKSocketName, FHitResult HitResult, FVector& CurrentOffset, float DeltaTime) const;
	static void UpdateFootRotation(FRotator& CurrentRotation, FRotator NormalRotation, float DeltaTime, float InterpSpeed);
	void UpdateHipOffset(FVector& HipOffset, float DeltaTime, float InterpSpeed) const;
	bool UpdateFootTrace(FName IKFootSocket, FHitResult& HitResult) const;
	
	FRotator NormalToRotator(FHitResult HitResult) const;

	FORCEINLINE FIKFootValues GetIKFootValues() const { return IKFootValues; }

	FORCEINLINE void SetIKDebugging(bool bDebug) { bIsDebugging = bDebug; };
	FORCEINLINE bool GetIKDebugging() const { return bIsDebugging; };	
};
