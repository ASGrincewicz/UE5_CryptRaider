#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Actor = GetAcceptableActor();

	if(Actor != nullptr && !IsUnlocked)
	{
		UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if(Component != nullptr)
		{
			Component->SetSimulatePhysics(false);
			Actor->AttachToComponent(this,FAttachmentTransformRules::KeepWorldTransform);
		}
		IsUnlocked = true;
		Mover->SetShouldMove(true);
		UE_LOG(LogTemp,Display,TEXT("Unlocking"));
		
	}
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
	Mover = NewMover;
	if(NewMover != nullptr)
		UE_LOG(LogTemp,Display,TEXT("Mover Set"));
}


AActor* UTriggerComponent::GetAcceptableActor() const
{
	TArray<AActor*> Actors;
	GetOverlappingActors(Actors);
	
	for(AActor* Actor: Actors)
	{
		bool HasAcceptableTag = Actor->ActorHasTag(UnlockTag);
		bool IsGrabbed = Actor->ActorHasTag("Grabbed");
		if(HasAcceptableTag && !IsGrabbed)
		{
			return Actor;
		}
	}
	return nullptr;
}