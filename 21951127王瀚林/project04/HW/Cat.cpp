#include "Cat.h"

Cat::Cat() {
	Status = stop;
	Power = 0;
	ChargeSpeed = 0;
}

void Cat::UpdateCat() {

	switch (Status)
	{
	case stop:
		if (ChargeSpeed != 0)
		{
			DoCharge();
		}
		break;
	case charge:
		if (ChargeSpeed == 0)
		{
			DoRelease();
		}
		else {
			DoCharge();
		}
		break;
	case jump:
		if (Safe) {
			JumpCat();
		}
		else {
			DropCat();
		}
		break;
	case drop:
		DropCat();
		break;
	case dead:
		//Reset();
		break;
	default:
		break;
	}

}

void Cat::JumpCat() {
	V = V - G;
	Height = Height + V;
	if (Height < 0) {
		//itemHeight = -itemHeight;
		//v = -v;
		Height = 0;
		V = 0;
		Status = stop;
	}
}

void Cat::DropCat() {
	V = V - G;
	Height = Height + V;
	if (Height < 0) {
		//itemHeight = -itemHeight;
		//v = -v;
		if (Height < -5)
		{
			Height = -5;
			V = 0;
			Status = dead;
		}
		else
		{
			Status = drop;
		}

	}
}

void Cat::DoCharge() {
	Power += ChargeSpeed;
	if (Power > 1.0) {
		Power = 1.0;
	}
	Status = charge;
}

void Cat::DoRelease() {
	Status = jump;
	V = Power * 0.2;
	Power = 0;
}

bool Cat::inAir() {
	return Status == jump || Status == drop;
}

void Cat::Reset() {
	Status = stop;
	Power = 0;
	ChargeSpeed = 0;
	Height = 0;
	V = 0;
}

bool Cat::isDead() {
	return Status == dead;
}

Cat::~Cat() {

}
