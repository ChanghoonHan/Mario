#include "MapAnchor.h"
#include "Block.h"

bool MapAnchor::init()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2(0, 40));
	m_blockArrayVector.clear();
	m_drawNode = DrawNode::create();
	this->addChild(m_drawNode);
	BackgroundUpdate();

	return true;
}

void MapAnchor::UpdateAnchor(float deltaTime)
{

}

void MapAnchor::AddBlock(Vec2 mousePos, BLOCK_TYPE blockType, int stackNum, bool isEntranceStack)
{
	Vec2 thisPos = this->getPosition();
	Vec2 addPos = Vec2(mousePos.x + (-thisPos.x), mousePos.y - 40);
	
	Vec2 addIndex = Vec2((int)addPos.x / 80, ((int)addPos.y + 80) / 80);

	if (addIndex.y > MAX_BLOCK_HEIGHT - 1)
	{
		return;
	}

	int vectorSize =  m_blockArrayVector.size();
	if (vectorSize < addIndex.x + 1)
	{
		if (vectorSize != addIndex.x + 1)
		{
			int vectorAddCount = addIndex.x + 1 - vectorSize;

			std::vector<Block*> newVector;
			for (size_t i = 0; i < MAX_BLOCK_HEIGHT; i++)
			{
				newVector.push_back(NULL);
			}

			for (size_t i = 0; i < vectorAddCount; i++)
			{
				m_blockArrayVector.push_back(newVector);
			}
		}
	}

	if (m_blockArrayVector[(int)addIndex.x][(int)addIndex.y] == NULL)
	{
		m_blockArrayVector[(int)addIndex.x][(int)addIndex.y] = Block::create();
		m_blockArrayVector[(int)addIndex.x][(int)addIndex.y]->setPosition(Vec2(addIndex.x * 80, -80 + (addIndex.y * 80)));
		this->addChild(m_blockArrayVector[(int)addIndex.x][(int)addIndex.y]);
	}

	m_blockArrayVector[(int)addIndex.x][(int)addIndex.y]->SetBlockType(blockType, stackNum, isEntranceStack);
	
	BackgroundUpdate();
}

void MapAnchor::SetItemInBlock(Vec2 mousePos, ITEM_TYPE itemType)
{
	Vec2 thisPos = this->getPosition();
	Vec2 addPos = Vec2(mousePos.x + (-thisPos.x), mousePos.y - 40);

	Vec2 setItemBlockIndex = Vec2((int)addPos.x / 80, ((int)addPos.y + 80) / 80);

	int vectorSize = m_blockArrayVector.size();
	if (vectorSize < setItemBlockIndex.x + 1)
	{
		return;
	}

	if (m_blockArrayVector[(int)setItemBlockIndex.x][(int)setItemBlockIndex.y] == NULL)
	{
		return;
	}
	else
	{
		m_blockArrayVector[(int)setItemBlockIndex.x][(int)setItemBlockIndex.y]->SetItemType(itemType);
	}
}

void MapAnchor::SetMonster(Vec2 mousePos, MONSTER_TYPE monsterType)
{
	Vec2 thisPos = this->getPosition();
	Vec2 addPos = Vec2(mousePos.x + (-thisPos.x), mousePos.y - 40);

	Vec2 setMonsterBlockIndex = Vec2((int)addPos.x / 80, ((int)addPos.y + 80) / 80);

	int vectorSize = m_blockArrayVector.size();
	if (vectorSize < setMonsterBlockIndex.x + 1)
	{
		return;
	}

	if (monsterType == MONSTER_TYPE::FLOWER)
	{
		if (m_blockArrayVector[(int)setMonsterBlockIndex.x][(int)setMonsterBlockIndex.y - 1] == NULL)
		{
			return;
		}

		if (m_blockArrayVector[(int)setMonsterBlockIndex.x][(int)setMonsterBlockIndex.y - 1]->getBlockType() != BLOCK_TYPE::STACK_TOP)
		{
			return;
		}
	}

	if (m_blockArrayVector[(int)setMonsterBlockIndex.x][(int)setMonsterBlockIndex.y] == NULL)
	{
		m_blockArrayVector[(int)setMonsterBlockIndex.x][(int)setMonsterBlockIndex.y] = Block::create();
		m_blockArrayVector[(int)setMonsterBlockIndex.x][(int)setMonsterBlockIndex.y]->setPosition(Vec2(setMonsterBlockIndex.x * 80, -80 + (setMonsterBlockIndex.y * 80)));
		this->addChild(m_blockArrayVector[(int)setMonsterBlockIndex.x][(int)setMonsterBlockIndex.y]);
	}
	
	m_blockArrayVector[(int)setMonsterBlockIndex.x][(int)setMonsterBlockIndex.y]->SetMonsterType(monsterType);
}

void MapAnchor::RemoveBlock(Vec2 mousePos)
{
	Vec2 thisPos = this->getPosition();
	Vec2 addPos = Vec2(mousePos.x + (-thisPos.x), mousePos.y - 40);

	Vec2 removeIndex = Vec2((int)addPos.x / 80, ((int)addPos.y + 80) / 80);

	int vectorSize = m_blockArrayVector.size();
	if (vectorSize < removeIndex.x + 1)
	{
		return;
	}

	if (m_blockArrayVector[(int)removeIndex.x][(int)removeIndex.y] == NULL)
	{
		return;
	}
	else
	{
		if (m_blockArrayVector[(int)removeIndex.x][(int)removeIndex.y]->getBlockType() == BLOCK_TYPE::STACK_TOP &&
			m_blockArrayVector[(int)removeIndex.x][(int)removeIndex.y+1] != NULL &&
			m_blockArrayVector[(int)removeIndex.x][(int)removeIndex.y+1]->getMosnterType() == MONSTER_TYPE::FLOWER)
		{
			this->removeChild(m_blockArrayVector[(int)removeIndex.x][(int)removeIndex.y-1]);
			m_blockArrayVector[(int)removeIndex.x][(int)removeIndex.y+1] = NULL;
		}

		this->removeChild(m_blockArrayVector[(int)removeIndex.x][(int)removeIndex.y]);
		m_blockArrayVector[(int)removeIndex.x][(int)removeIndex.y] = NULL;
		UpdateMapVector();
	}

	BackgroundUpdate();
}

void MapAnchor::BackgroundUpdate()
{
	int vecSize = m_blockArrayVector.size();
	m_drawNode->clear();
	if (vecSize == 0)
	{
		m_drawNode->drawSolidRect(Vec2::ZERO, Vec2(80, m_visibleSize.height - 80), Color4F(107 / 255.0f, 140 / 255.0f, 255 / 255.0f, 1.0f));
	}
	else
	{
		m_drawNode->drawSolidRect(Vec2::ZERO, Vec2(80 * vecSize, m_visibleSize.height - 80), Color4F(107 / 255.0f, 140 / 255.0f, 255 / 255.0f, 1.0f));
	}
}

void MapAnchor::UpdateMapVector()
{
	bool lastLineEmpty = true;
	for (size_t i = 0; i < MAX_BLOCK_HEIGHT; i++)
	{
		if (m_blockArrayVector[m_blockArrayVector.size() - 1][i] != NULL)
		{
			lastLineEmpty = false;
			break;
		}
	}

	if (lastLineEmpty)
	{
		m_blockArrayVector[m_blockArrayVector.size() - 1].clear();
		m_blockArrayVector.pop_back();
		if (m_blockArrayVector.size() != 0)
		{
			UpdateMapVector();
		}
	}

	return;
}

std::string MapAnchor::GetMapToString()
{
	std::string mapString = "";

	for (auto blockArray : m_blockArrayVector)
	{
		for (auto block : blockArray)
		{
			if (block == NULL)
			{
				mapString += "N ";
			}
			else
			{
				mapString += block->GetBlockString();
			}
		}
		mapString += "\n";
	}

	return mapString;
}

void MapAnchor::Clear()
{
	for (auto blockArray : m_blockArrayVector)
	{
		for (auto block : blockArray)
		{
			if (block != NULL)
			{
				this->removeChild(block);
			}
		}
	}

	m_blockArrayVector.clear();
	BackgroundUpdate();
	this->setPosition(0, 40);
}

MapAnchor::MapAnchor()
{
}


MapAnchor::~MapAnchor()
{
}
