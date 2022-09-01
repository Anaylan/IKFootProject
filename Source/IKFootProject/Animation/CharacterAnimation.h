// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IKFootProject/IKFootProjectCharacter.h"
#include "IKFootProject/Components/IKFootComponent.h"
#include "IKFootProject/Library/StructLibrary.h"
#include "CharacterAnimation.generated.h"

/**
 * 
 */
UCLASS()
class IKFOOTPROJECT_API UCharacterAnimation : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		TWeakObjectPtr<AIKFootProjectCharacter> CachedCharacter = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		UIKFootComponent* FootIKComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
		FIKFootValues IKFootValues;

	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void InitFootIK();
	void UpdateFootIK();
};
