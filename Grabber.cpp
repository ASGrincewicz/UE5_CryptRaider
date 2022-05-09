// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"



// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	

	if(GetPhysicsHandle() != nullptr)
	{
		FString HandleName = GetPhysicsHandle()->GetName();
		UE_LOG(LogTemp, Display, TEXT("%s"),*HandleName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Physics Handle Found!"));
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;

	GetPhysicsHandle()->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released"));
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Display, TEXT("Grabbing"));
	
	FVector Start = GetComponentLocation();
	
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	
	FHitResult HitResult;

	bool HasHIt;
	HasHIt = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);

	if(HasHIt)
	{
		GetPhysicsHandle()->GrabComponentAtLocationWithRotation(HitResult.GetComponent(), NAME_None, HitResult.ImpactPoint, GetComponentRotation());
		AActor* ActorHit = HitResult.GetActor();

		FString HitName = ActorHit->GetActorNameOrLabel();
		UE_LOG(LogTemp, Display, TEXT("Sweep hit: %s "),*HitName);
		DrawDebugSphere(GetWorld(),HitResult.ImpactPoint, 10,10, FColor::Blue, false, 5);
		DrawDebugSphere(GetWorld(),HitResult.Location, 10,10, FColor::Green, false, 5);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Sweep hit: Nothing"));
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("No Physics Handle!"));
	}
	return PhysicsHandle;
}





