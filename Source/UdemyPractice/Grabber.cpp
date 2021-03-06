// Copyright Soohwan Park 2019.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"

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
	
	//Look for attached component
	physicsHandle = owner->FindComponentByClass<UPhysicsHandleComponent>();
	if (!physicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing Handle component"), *(owner->GetName()))
	}
	
	inputComponent = owner->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{ 
		UE_LOG(LogTemp, Error, TEXT("%s missing Input component"), *(owner->GetName()))
	}
	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//GetReachLineEnd();

	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineStart() const
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation);

	return playerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(
		OUT playerViewPointLocation,
		OUT playerViewPointRotation);

	return playerViewPointLocation + playerViewPointRotation.Vector() * reach;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"))

	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* grabbed = hitResult.GetComponent();
	AActor* actorHit = hitResult.GetActor();

	if (actorHit)
	{
		physicsHandle->GrabComponent(grabbed, NAME_None, grabbed->GetOwner()->GetActorLocation(), true);
	}

}

void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released"))
		physicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{

	FCollisionQueryParams traceParams(FName(TEXT("")), false, owner);

	FHitResult lineTraceHit;

	GetWorld()->LineTraceSingleByObjectType(
		lineTraceHit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams
	);
	return lineTraceHit;
}
