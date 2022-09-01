// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimation.h"

void UCharacterAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	CachedCharacter = StaticCast<AIKFootProjectCharacter*>(TryGetPawnOwner());
	
	InitFootIK();
}

void UCharacterAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateFootIK();
}

void UCharacterAnimation::InitFootIK()
{
	APawn* Owner = TryGetPawnOwner();

	if (!Owner) { return; }

	UActorComponent* ActorComponent = Owner->GetComponentByClass(UIKFootComponent::StaticClass());

	if (!ActorComponent) { return; }

	FootIKComponent = Cast<UIKFootComponent>(ActorComponent);
}

void UCharacterAnimation::UpdateFootIK()
{
	if (!FootIKComponent) { return; }

	IKFootValues = FootIKComponent->GetIKFootValues();
}
