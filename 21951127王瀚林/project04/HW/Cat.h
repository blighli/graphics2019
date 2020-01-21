#pragma once
class Cat
{
enum STATUS{stop=1,charge,jump,drop,dead};
public:
	STATUS Status; //猫状态
	float Power; //猫跳跃力量
	float ChargeSpeed; //猫蓄力速度
	float Height = 0.0f; //猫高度
	float V = 0.0f; //猫速度
	float G = 0.005f; //猫重力加速度
	bool Safe = true; //猫平台检测

	Cat();
	~Cat();

	void UpdateCat(); //更新猫状态

	void JumpCat(); //猫安全跳跃

	void DropCat(); //猫不安全跳跃

	void DoCharge(); //猫充能

	void DoRelease(); //猫起跳

	bool inAir(); //检查猫是否在空中

	void Reset(); //重置猫

	bool isDead(); //检查猫是否死亡
};

