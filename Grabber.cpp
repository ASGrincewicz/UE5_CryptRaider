// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;

		GetPhysicsHandle()->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("Grabbing"));
	
	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if(HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		HitResult.GetActor()->Tags.Add("Grabbed");
		GetPhysicsHandle()->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
		AActor* ActorHit = HitResult.GetActor();

		FString HitName = ActorHit->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Sweep hit: %s "),*HitName);
		//DrawDebugSphere(GetWorld(),HitResult.ImpactPoint, 10,10, FColor::Blue, false, 5);
		//DrawDebugSphere(GetWorld(),HitResult.Location, 10,10, FColor::Green, false, 5);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Sweep hit: Nothing"));
	}
}
void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("Grabber requires a UPhysicsHandleComponent!"));
	}
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);
}
