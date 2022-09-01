// Fill out your copyright notice in the Description page of Project Settings.


#include "IKFootComponent.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

static FName NAME_RootBone = FName("root");

// Sets default values for this component's properties
UIKFootComponent::UIKFootComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	LeftIKFootName = FName("ik_foot_l");
	RightIKFootName = FName("ik_foot_r");
}


// Called when the game starts
void UIKFootComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = StaticCast<AIKFootProjectCharacter*>(GetOwner());

	MeshComponent = Character->GetMesh();
}


// Called every frame
void UIKFootComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Character->GetCharacterMovement()->IsWalking())
	{
		bLeftHit = UpdateFootTrace(LeftIKFootName, LeftHit);
		bRightHit = UpdateFootTrace(RightIKFootName, RightHit);

		UpdateFootLocation(LeftIKFootName, LeftHit, IKFootValues.LeftFootOffset, DeltaTime);
		UpdateFootLocation(RightIKFootName, RightHit, IKFootValues.RightFootOffset, DeltaTime);

		GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Green, NormalToRotator(LeftHit).ToString());
		GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, NormalToRotator(RightHit).ToString());
	
		UpdateFootRotation(IKFootValues.LeftFootRotation, NormalToRotator(LeftHit), DeltaTime, 20.f);
		UpdateFootRotation(IKFootValues.RightFootRotation, NormalToRotator(RightHit), DeltaTime, 20.f);

		UpdateHipOffset(IKFootValues.HipOffset, DeltaTime, 20.f);
	}
	else
	{
		bLeftHit = false;
		bRightHit = false;

		UpdateFootRotation(IKFootValues.LeftFootRotation, FRotator::ZeroRotator, DeltaTime, 20.f);
		UpdateFootRotation(IKFootValues.RightFootRotation, FRotator::ZeroRotator, DeltaTime, 20.f);

		UpdateHipOffset(IKFootValues.HipOffset, DeltaTime, 20.f);
			
	}
	
}

void UIKFootComponent::UpdateFootLocation(FName IKSocketName, FHitResult HitResult, FVector& CurrentOffset,
                                          float DeltaTime) const
{
	FVector IKFootPosition = MeshComponent->GetSocketLocation(IKSocketName);
	IKFootPosition.Z = MeshComponent->GetSocketLocation(NAME_RootBone).Z;

	FVector TargetLocation = FVector::ZeroVector;

	if (Character->GetCharacterMovement()->IsWalkable(HitResult))
	{
		FVector ImpactNormal = HitResult.ImpactNormal;
		FVector ImpactPoint = HitResult.ImpactPoint;

		TargetLocation = (ImpactPoint + ImpactNormal * FootHeight) - (IKFootPosition +
			FVector(0.f, 0.f, FootHeight));
	};

	CurrentOffset = FMath::VInterpTo(CurrentOffset, TargetLocation, DeltaTime, 25.f);
}

void UIKFootComponent::UpdateFootRotation(FRotator& CurrentRotation, FRotator NormalRotation, float DeltaTime,
                                          float InterpSpeed)
{
	CurrentRotation = FMath::RInterpTo(CurrentRotation, NormalRotation, DeltaTime, InterpSpeed);
}

void UIKFootComponent::UpdateHipOffset(FVector& HipOffset, float DeltaTime, float InterpSpeed) const
{
	if (bLeftHit && bRightHit)
	{
		if (LeftHit.ImpactPoint.Z != RightHit.ImpactPoint.Z)
		{
			const float ClampedOffset = FMath::Clamp(FMath::Abs(LeftHit.ImpactPoint.Z - RightHit.ImpactPoint.Z),
			                                         0.f, Character->GetCapsuleComponent()->
			                                                         GetScaledCapsuleHalfHeight());

			HipOffset.Z = FMath::FInterpTo(HipOffset.Z, -ClampedOffset, DeltaTime, InterpSpeed);
		}
		else
		{
			HipOffset.Z = FMath::FInterpTo(HipOffset.Z, 0.f, DeltaTime, InterpSpeed);
		}
	}
	else
	{
		HipOffset.Z = 0.f;
	}
}

bool UIKFootComponent::UpdateFootTrace(FName IKFootSocket, FHitResult& HitResult) const
{
	const FVector IKFootLocation = MeshComponent->GetSocketLocation(IKFootSocket);

	const FVector Start = FVector(IKFootLocation.X, IKFootLocation.Y, Character->GetActorLocation().Z);
	const FVector End = FVector(IKFootLocation.X, IKFootLocation.Y, Character->GetActorLocation().Z -
	                            Character->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() - fTraceDistance);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	const EDrawDebugTrace::Type DebugTrace = bIsDebugging ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;

	return UKismetSystemLibrary::LineTraceSingle(GetWorld(), Start, End,
             UEngineTypes::ConvertToTraceType(ECC_Visibility), true,
             ActorsToIgnore, DebugTrace, HitResult, true);
}

FRotator UIKFootComponent::NormalToRotator(FHitResult HitResult) const
{
	FRotator TargetRotation = FRotator::ZeroRotator;

	const FVector ImpactNormal = HitResult.ImpactNormal;

	TargetRotation.Pitch = -FMath::RadiansToDegrees(FMath::Atan2(ImpactNormal.X, ImpactNormal.Z));
	TargetRotation.Roll = FMath::RadiansToDegrees(FMath::Atan2(ImpactNormal.Y, ImpactNormal.Z));

	return TargetRotation;
}
