#pragma once
#include <Windows.h>
#include <iostream>
#include "HelperFunctions.hpp"

class BoneStructure
{
public:
	char pad_0000[120]; //0x0000
	char* humanoidValidation; //0x0078
	char pad_0080[3800]; //0x0080
	vec3 head; //0x0F58
};

class ClassStructure
{
public:
	char pad_0000[304]; //0x0000
	BoneStructure* BoneStructurePtr; //0x0130
};

class ClassAttrib
{
public:
	char pad_0000[8]; //0x0000
	ClassStructure* bonePath2; //0x0008
};

class AccelerationBase
{
public:
	char pad_0000[176]; //0x0000
	float Right; //0x00B0
	float Forwards; //0x00B4
	char pad_00B8[1872]; //0x00B8
}; //Size: 0x0808

class PlayerCoordinates
{
public:
	char pad_0000[288]; //0x0000
	vec3 xyz; //0x0120
};

class LocationName
{
public:
	char pad_0000[40];
	char* locationName;
};

class Name
{
public:
	char pad_0000[40];
	char* name;
};

class Entity
{
public:
	char pad_0000[84]; //0x0000
	vec3 xyz; //0x0054
	LocationName* locationPtr; //0x0060
	void* validationPtr1; //0x0068
	void* validationPtr2; //0x0070
	void* validationPtr3; //0x0078
	char pad_0068[368]; //0x0068
	Name* namePtr; //0x01F0
	char pad_01F8[104]; //0x01F8
	ClassAttrib* bonePath1; //0x0260
	char pad_0268[3048]; //0x0268
	class AccelerationBase* accelerationBase; //0x0E50
}; //Size: 0x0850



	//char pad_01F8[2848]; //0x01F8
	//void *BonePtr; //0x0D18
	//char pad_0D20[304]; //0x0D20