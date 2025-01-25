// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/TimelineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"

// Sets default values
ADoor::ADoor()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Components setup
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(RootComponent);

    DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);

    bIsOpen = false;
    OverlappingPlayer = nullptr;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
    Super::BeginPlay();

    ClosedRotation = GetActorRotation();
    OpenRotation = ClosedRotation + FRotator(0.0f, 90.0f, 0.0f);

    if (DoorCurve)
    {
        InterpFunction.BindUFunction(this, FName("HandleDoorProgress"));
        TimelineFinished.BindUFunction(this, FName("OnTimelineFinished"));

        DoorTimeline->AddInterpFloat(DoorCurve, InterpFunction);
        DoorTimeline->SetTimelineFinishedFunc(TimelineFinished);
    }
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnTriggerBoxBeginOverlap);
    TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnTriggerBoxEndOverlap);
}

void ADoor::OnTriggerBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA<APlayerCharacter>())
    {
        OverlappingPlayer = OtherActor;
        ToggleDoor();
    }
}

void ADoor::OnTriggerBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor == OverlappingPlayer)
    {
        OverlappingPlayer = nullptr;
        ToggleDoor();
    }
}
void ADoor::HandleDoorProgress(float Value)
{
    FRotator NewRotation = FMath::Lerp(ClosedRotation, OpenRotation, Value);
    SetActorRotation(NewRotation);
}

void ADoor::OnTimelineFinished()
{
    bIsOpen = !bIsOpen;
}

void ADoor::ToggleDoor()
{
    if (DoorTimeline->IsPlaying())
    {
        return;
    }

    if (bIsOpen)
    {
        DoorTimeline->Reverse();
    }
    else
    {
        DoorTimeline->Play();
    }
}