// Copyright Soohwan Park 2019.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	reach = 100.0f;
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();
	

	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation);

	//UE_LOG(LogTemp, Warning, TEXT("Position : %s, Rotation : %s"), 
	//	*playerViewPointLocation.ToString(),
	//	*playerViewPointRotation.ToString())

	FVector lineTraceEnd = playerViewPointLocation + playerViewPointRotation.Vector() * reach;

	DrawDebugLine(
		GetWorld(),
		playerViewPointLocation,
		lineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f
	);

	FCollisionQueryParams traceParams(FName(TEXT("")), false, owner);

	FHitResult lineTraceHit;

	GetWorld()->LineTraceSingleByObjectType(
		lineTraceHit,
		playerViewPointLocation,
		lineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);

	AActor* actorHit = lineTraceHit.GetActor();
	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit %s"), *(actorHit->GetName()))
	}

}

