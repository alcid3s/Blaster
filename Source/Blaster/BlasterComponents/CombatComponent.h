// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

// Lecture 74
#define TRACE_LENGTH 80000.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	friend class ABlasterCharacter;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void EquipWeapon(class AWeapon* WeaponToEquip);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAimingRPC(bool bIsAiming);

	UFUNCTION()
	void OnRep_EquippedWeapon();

	void FireButtonPressed(bool bPressed);

	UFUNCTION(Server, Reliable)
	void ServerRPCFire(const FVector_NetQuantize& TraceHitTarget);

	UFUNCTION(NetMultiCast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget);

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	// lecture 83
	void SetHUDCrosshairs(float DeltaTime);

private:
	class ABlasterCharacter* Character;

	// lecture 83
	class ABlasterPlayerController* Controller;
	class ABlasterHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon);
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated);
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	/*
		HUD and crosshairs
	*/
	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;

	FVector HitTarget;

	// L88
	float CrosshairAimFactor;
	float CrosshairShootingFactor;

	/*
		Aiming and FOV L87
	*/

	// field of view when not aiming; set to the camera's base FOV in BeginPlay
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomedFOV = 30.f;

	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category = Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

public:	
	

};
