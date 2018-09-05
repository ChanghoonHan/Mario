#include "Kuri.h"

USING_NS_CC;

bool Kuri::init()
{
	Monster::init();

	m_destroyTime = 0.5f;
	m_destroyDeltaTime = 0;

	m_score = 100;
	SetScoreLabel();

	return true;
}

void Kuri::UpdateMonster(float deltaTime)
{
	switch (m_curState)
	{
	case MONSTER_STATE::STAY:
		break;
	case MONSTER_STATE::IDLE:
		m_idleSpriteChangeDeltaTime += deltaTime;
		if (m_idleSpriteChangeTime < m_idleSpriteChangeDeltaTime)
		{
			m_idleSpriteChangeDeltaTime = 0;
			m_curKuriTextureIdx = (KURI_TEXTURE)((m_curKuriTextureIdx + 1) % 2);
			SetTexture();
		}
		break;
	case MONSTER_STATE::DESTROY:
		if (!m_instantaneousDestroy)
		{
			m_destroyDeltaTime += deltaTime;
			if (m_destroyTime < m_destroyDeltaTime)
			{
				m_destroyDeltaTime = 0;
				ChangeState(MONSTER_STATE::DESTROYED);
			}
		}
		break;
	case MONSTER_STATE::DESTROYED:
		break;
	default:
		break;
	}

	Monster::UpdateMonster(deltaTime);
}

void Kuri::ChangeState(MONSTER_STATE state, bool instantaneousDestroy)
{
	switch (state)
	{
	case Monster::STAY:
		m_curKuriTextureIdx = KURI_TEXTURE::IDLE1;
		break;
	case Monster::IDLE:
		m_curKuriTextureIdx = KURI_TEXTURE::IDLE2;
		break;
	case Monster::DESTROY:
		if (!instantaneousDestroy)
		{
			m_curKuriTextureIdx = KURI_TEXTURE::DIE;
		}
		break;
	default:
		break;
	}

	Monster::ChangeState(state, instantaneousDestroy);
}

void Kuri::SetTextureAndMonsterSize()
{
	m_monsterSize.width = 80;
	m_monsterSize.height = 80;

	m_textures.push_back(Director::getInstance()->getTextureCache()->addImage("Monsters/KuriIdle1.png"));
	m_textures.push_back(Director::getInstance()->getTextureCache()->addImage("Monsters/KuriIdle2.png"));
	m_textures.push_back(Director::getInstance()->getTextureCache()->addImage("Monsters/KuriDie.png"));
	m_curKuriTextureIdx = KURI_TEXTURE::IDLE1;
}

void Kuri::SetTexture()
{
	m_curTexture = m_textures[m_curKuriTextureIdx];
	Monster::SetTexture();
}

Kuri::Kuri()
{
}


Kuri::~Kuri()
{
}
