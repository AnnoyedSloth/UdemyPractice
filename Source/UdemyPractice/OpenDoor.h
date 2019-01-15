// Copyright Soohwan Park 2019.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

class ATriggerVolume;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDEMYPRACTICE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void OpenDoor();
	void CloseDoor();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = Movement)
		FDoorEvent onOpen;
	UPROPERTY(BlueprintAssignable, Category = Movement)
		FDoorEvent onClose;

private:
	UPROPERTY(VisibleAnywhere,Category = Movement)
	float openAngle = 90.0f;

	UPROPERTY(EditAnywhere, Category = Movement)
	ATriggerVolume* pressurePlate;

	UPROPERTY(EditAnywhere, Category = Movement)
		float doorCloseDelay = 1.0f;

	UPROPERTY(EditAnywhere, Category = Movement)
		float triggerMass = 30.0f;

	float doorLastOpenTime;

	AActor* owner;

	float GetTotalMassOfActorsOnPlate();

		
};
