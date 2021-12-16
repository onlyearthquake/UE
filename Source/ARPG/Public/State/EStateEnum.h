// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EStateEnum.generated.h"

UENUM(Blueprintable)
enum class EStateEnum:uint8
{
	IDLE,
	ATTACK,
	ATTACKED,
	DODGE
};

UENUM(Blueprintable)
enum class EAIStateEnum:uint8
{
	IDLE,
	ATTACK,
	ATTACKED,
	HEALING,
	DODGE
};
