// Copyright Soohwan Park 2019.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	APawn* myPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	//owner->SetActorRotation(FRotator(0.0f, openAngle, 0.0f));
	owner->SetActorRelativeRotation(FRotator(0.0f, openAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	//owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
	owner->SetActorRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (pressurePlate && GetTotalMassOfActorsOnPlate()>50.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%f"), GetTotalMassOfActorsOnPlate())
		OpenDoor();
		doorLastOpenTime = GetWorld()->GetTimeSeconds();
	}

	if (GetWorld()->GetTimeSeconds() - doorLastOpenTime > doorCloseDelay)
	{
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0;

	TArray<AActor*> actors;

	pressurePlate->GetOverlappingActors(actors, AActor::StaticClass());

	for (const AActor* actor : actors)
	{
		//if(actor) totalMass += ((UPrimitiveComponent*)actor)->GetMass();
		totalMass = actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}
