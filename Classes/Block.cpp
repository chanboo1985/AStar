#include "Block.h"

Block* Block::createBlock(int wh)
{
	Block* block = new Block();
	if (block && block->init(wh))
	{
		block->autorelease();
		return block;
	}
	CC_SAFE_DELETE(block);
	return nullptr;
}

bool Block::init(int wh)
{
	if (!Node::init())
	{
		return false;
	}
	m_width = m_height = wh;
	m_text = Label::create("","Arial",20);
	m_text->setPosition(Vec2(wh>>1,wh>>1));
	m_layer = LayerColor::create(Color4B(200,190,180,255));
	m_layer->setContentSize(Size(wh,wh));
	m_layer->setPosition(Vec2::ZERO);
	this->addChild(m_layer,0);
	this->addChild(m_text,1);
	m_num = -1;
	return true;
}

void Block::setNum(int num)
{
	if (num < 0 || num == m_num)
	{
		return;
	}
	m_num = num;
	char txt[20];
	sprintf(txt,"%d",num);
	m_text->setString(txt);
}