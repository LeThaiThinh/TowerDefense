#include "TowerTwo.h"
#include "DefensiveManager.h"
#include <Enemy/EnemyManager.h>
#include "Resource/Coin.h"
#include <Others/TowerOption.h>
#define TowerTwoCost 70

TowerTwo::TowerTwo() :UnMoveThroughAbleTower() {}

TowerTwo::TowerTwo(float x, float y) : UnMoveThroughAbleTower(ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg"),
	ResourceManagers::GetInstance()->GetShader("TextureShader"),
	ResourceManagers::GetInstance()->GetTexture("TowerTwolvl1.tga"),
	x, y, 120, 100, 120, 100, 500, 0.3f, 40, 0, Vector3(0, 0, 0), nullptr, nullptr, 1000, 0, TowerType::Two, 3)
{
	SetName("White Tower");
	LocateOption();
	Upgrade();
	LocateOption();
}

void TowerTwo::Attack()
{
	UnMoveThroughAbleTower::Attack();
	BulletPoolManager::GetInstance()->AddBullet(
		BulletType::Target_Tower_Two, std::dynamic_pointer_cast<BaseObject>(m_target),
		Vector3(0, 0, 0), shared_from_this());
}

void TowerTwo::Reset()
{
	UnMoveThroughAbleTower::Reset();
	Upgrade();
}

void TowerTwo::LocateOption()
{
	EnemyPoolManager::ResetFindPath();
	m_towerOption = std::make_shared<TowerOption>();
	m_towerOption->SetMainOptionPosition(m_position.x, m_position.y);
	m_towerOption->SetMainOptionSize(m_iWidth , m_iHeight );

	std::shared_ptr<GameButton> secondButton1 = std::make_shared<GameButton>(ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg"),
		ResourceManagers::GetInstance()->GetShader("TextureShader"),
		ResourceManagers::GetInstance()->GetTexture("Tower/remove_tower.tga"));
	secondButton1->Set2DPosition(m_position.x - m_iWidth / 2.f - AdjustTowerOption, m_position.y - m_iHeight / 2.f - AdjustTowerOption);
	secondButton1->SetISize(m_iWidth / 2, m_iHeight / 2);
	secondButton1->SetOnClickTower([](std::shared_ptr<BaseDefensive> tower) {
		Coin::GetInstance()->Refund(tower->GetTotalCost() * RefundRatio);
		DefensivePoolManager::GetInstance()->RemoveUnmove(tower);
		DefensivePoolManager::GetInstance()->Add(tower->GetPosition().x, tower->GetPosition().y, TowerType::Spot);
		EnemyPoolManager::ResetFindPath();
		});
	m_towerOption->AddSecondOption(secondButton1);

	std::shared_ptr<GameButton> secondButton2 = std::make_shared<GameButton>(ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg"),
		ResourceManagers::GetInstance()->GetShader("TextureShader"),
		ResourceManagers::GetInstance()->GetTexture("Tower/upgrade_tower.tga"));
	secondButton2->Set2DPosition(m_position.x + m_iWidth / 2.f + AdjustTowerOption, m_position.y - m_iHeight / 2.f - AdjustTowerOption);
	secondButton2->SetISize(m_iWidth / 2, m_iHeight / 2);
	secondButton2->SetOnClickTower([](std::shared_ptr<BaseDefensive> tower) {
		if (Coin::GetInstance()->Purchase(tower->GetNextCost()))
			tower->Upgrade();
		});
	m_towerOption->AddSecondOption(secondButton2);

	auto shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Triangle.ttf");
	auto 	text = std::make_shared< Text>(shader, font, std::to_string((int)(TowerTwoCost * RefundRatio)), Vector4(1.0f, 0.6f, 0.f, 1.0f), 0.6f);
	text->Set2DPositionDynamic(Vector2(m_position.x - m_iWidth / 2.f , m_position.y - m_iHeight / 4.f - AdjustTowerOption));
	m_towerOption->AddCostText(text);
	text = std::make_shared< Text>(shader, font, std::to_string(TowerTwoCost), Vector4(1.0f, 0.6f, 0.f, 1.0f), 0.6f);
	text->Set2DPositionDynamic(Vector2(m_position.x + m_iWidth / 2.f + AdjustTowerOption, m_position.y - m_iHeight / 4.f - AdjustTowerOption));
	m_towerOption->AddCostText(text);
}

void TowerTwo::Update(GLfloat deltatime)
{
	UnMoveThroughAbleTower::Update(deltatime);
}

void TowerTwo::Upgrade()
{
	UnMoveThroughAbleTower::Upgrade();
	float previosMaxHitpoint = m_maxHitPoint;
	switch (m_level)
	{
	case 1:
		SetTexture(ResourceManagers::GetInstance()->GetTexture("TowerTwolvl1.tga"));
		SetAvatar(ResourceManagers::GetInstance()->GetTexture("TowerTwolvl1.tga"));
		SetISize(120, 100);
		m_totalCost = TowerTwoCost;
		m_costUpgrade = 100;
		m_range = 175;
		m_attackSpeed = 1;
		m_hitpoint = 1000.f;
		m_maxHitPoint = 1000.f;
		break;
	case 2:
		SetTexture(ResourceManagers::GetInstance()->GetTexture("TowerTwolvl2.tga"));
		SetAvatar(ResourceManagers::GetInstance()->GetTexture("TowerTwolvl2.tga"));
		SetISize(126, 105);
		m_totalCost += m_costUpgrade;
		m_costUpgrade = 140;
		m_range = 225;
		m_attackSpeed = 1;
		m_maxHitPoint = m_maxHitPoint + 200;
		break;
	case 3:
		SetTexture(ResourceManagers::GetInstance()->GetTexture("TowerTwolvl3.tga"));
		SetAvatar(ResourceManagers::GetInstance()->GetTexture("TowerTwolvl3.tga"));
		SetISize(132, 110);
		m_totalCost += m_costUpgrade;
		m_costUpgrade = 0;
		m_range = 250;
		m_attackSpeed = 1;
		m_maxHitPoint = m_maxHitPoint + 300;
		break;
	default:
		break;
	}
	m_towerOption->GetCostTextList().front()->SetText(std::to_string((int)(m_totalCost*RefundRatio)));
	if (m_level < m_maxlevel)
		m_towerOption->GetCostTextList().back()->SetText(std::to_string(m_costUpgrade));
	else
		m_towerOption->GetCostTextList().back()->SetText("max");

	m_currentTimeAttack = 1 / m_attackSpeed;
	if (m_level >= 2) {
		m_hitpoint = m_hitpoint / previosMaxHitpoint * m_maxHitPoint;
	}

}
