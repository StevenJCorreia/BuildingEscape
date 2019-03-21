// Fill out your copyright notice in the Description page of Project Settings.

#define OUT

#include "OpenDoor.h"
#include "Engine/TriggerVolume.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing pressure plate!"), *GetOwner()->GetName())
	}
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(NULL, OpenAngle, NULL));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(NULL, NULL, NULL));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Poll trigger volume
	if (GetTotalMassOfActorsOnPlate() > MassThreshold)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	/// Check if its time to close door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	// Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Iterate through array, adding up each element's mass to total
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}
