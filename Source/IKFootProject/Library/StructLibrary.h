#pragma once

#include "CoreMinimal.h"
#include "StructLibrary.generated.h"

USTRUCT(BlueprintType)
struct FIKFootValues
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector LeftFootOffset = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector RightFootOffset = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FRotator LeftFootRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FRotator RightFootRotation = FRotator::ZeroRotator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FVector HipOffset = FVector::ZeroVector;
};
