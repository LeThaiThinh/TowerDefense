#pragma once
#include "Sprite2D.h"
#include "Animation2D.h"
#include "MoveAble.h"
#include "AttackAble.h"
#include "ShootAble.h"
#include "../GameManager/ResourceManagers.h"
#include "ObjectPool.h"

class MainCharacter :public MoveAble, public Animation2D, public AttackAble, public ShootAble
{
public:

	MainCharacter();
	~MainCharacter();
	void Init();
	void Move(GLfloat deltatime);
	void Move(GLfloat deltatime, Vector2 vector2);
	void Update(GLfloat deltatime);
private:
	ObjectPool<Bullet>* m_poolTarget;
};