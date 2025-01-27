// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values
ADoor::ADoor()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
    DoorFrame->SetupAttachment(RootComponent);

    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
    Door->SetupAttachment(DoorFrame);
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
    Super::BeginPlay();

    if (CurveFloat)
    {
        FOnTimelineFloat TimelineProgress;
        TimelineProgress.BindDynamic(this, &ADoor::OpenDoor);
        Timeline.AddInterpFloat(CurveFloat, TimelineProgress);
    }
}

void ADoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    Timeline.TickTimeline(DeltaTime);
}

void ADoor::OnInteract()
{
    if (bIsDoorClosed)
    {
        Timeline.Play();
    }
    else {
        Timeline.Reverse();
    }

    bIsDoorClosed = !bIsDoorClosed; // Flip flop
}

void ADoor::OpenDoor(float Value)
{
    FRotator Rot = FRotator(0.f, DoorRotateAngle * Value, 0.f);

    Door->SetRelativeRotation(Rot);
}