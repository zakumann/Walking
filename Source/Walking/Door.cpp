// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "PlayerCharacter.h"

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
        SetDoorOnSameSide();
        Timeline.Play();
    }
    else {
        Timeline.Reverse();
    }

    bIsDoorClosed = !bIsDoorClosed; // Flip flop
}

void ADoor::OpenDoor(float Value)
{
    //float Angle = bDoorOnSameSide ? DoorRotateAngle : -DoorRotateAngle;
    float Angle = DoorRotateAngle;

    FRotator Rot = FRotator(0.0f, -Angle * Value, 0.0f);

    Door->SetRelativeRotation(Rot);
}

void ADoor::SetDoorOnSameSide()
{
    if (Character)
    {
        FVector CharacterFV = Character->GetActorForwardVector();
        FVector DoorFV = GetActorForwardVector();
        bDoorOnSameSide = (FVector::DotProduct(CharacterFV, DoorFV) >= 0);
    }
}