#include "HelloWorldScene.h"
#include "Block.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
	m_row = 15;
	m_col = 15;
	m_astar.setMapSize(m_row, m_col);
	m_astar.clearObstacles();
	m_astar.setDiagonalEnable(false);

	Vec2 start_pos(100.0f, 100.0f);
	m_astar.setStart(getLocation(start_pos));

	m_people = Sprite::create("people.png");
	m_people->setAnchorPoint(Vec2(0.0f,0.0f));
	m_people->setPosition(correctPosition(start_pos));
	this->addChild(m_people);

	Vec2 end_pos(300.0f, 300.0f);
	m_astar.setTarget(getLocation(end_pos));

	m_target = Sprite::create("CloseNormal.png");
	m_target->setAnchorPoint(Vec2(0.0f,0.0f));
	m_target->setPosition(correctPosition(end_pos));
	this->addChild(m_target);

	m_obstacles = Layer::create();
	m_obstacles->setPosition(Vec2(0.0f,0.0f));
	this->addChild(m_obstacles);

	for (int i = 0; i < m_col; i++)
	{
		Block* block = Block::createBlock(CELL_SIZE);
		block->setAnchorPoint(Vec2(0.0f,0.0f));
		block->setPosition(i*CELL_SIZE,m_row*CELL_SIZE);
		this->addChild(block);
	}

	for (int i = 0; i < m_col; i++)
	{
		Block* block = Block::createBlock(CELL_SIZE);
		block->setAnchorPoint(Vec2(0.0f,0.0f));
		block->setPosition(m_col*CELL_SIZE,i*CELL_SIZE);
		this->addChild(block);
	}

	MenuItemFont* goItem = MenuItemFont::create("GO",CC_CALLBACK_1(HelloWorld::goCallback,this));
	goItem->setFontSize(30);
	goItem->setAnchorPoint(Vec2(0.0f,0.0f));
	goItem->setPosition(Vec2((m_col + 2)*CELL_SIZE, 300));

	MenuItemFont* resetItem = MenuItemFont::create("RESET",CC_CALLBACK_1(HelloWorld::clearCallback,this));
	resetItem->setFontSize(30);
	resetItem->setAnchorPoint(Vec2(0.0f,0.0f));
	resetItem->setPosition(Vec2((m_col + 2)*CELL_SIZE, 200));

	Menu* menu = Menu::create(goItem,resetItem,NULL);
	menu->setPosition(Vec2(0.0f,0.0f));
	this->addChild(menu);

	EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan,this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    return true;
}

void HelloWorld::goCallback(cocos2d::Ref* pSender)
{
	if (m_astar.isHavePaths())
	{
		m_route.clear();
		m_routeStep = 0;
		m_astar.findPaths(&m_route);

		m_people->runAction(
			Sequence::create(
				CCMoveTo::create(.2f,Vec2(m_route[m_routeStep].m_x*CELL_SIZE,m_route[m_routeStep].m_y*CELL_SIZE)),
				CallFunc::create(CC_CALLBACK_0(HelloWorld::stepCallback,this)),
				NULL
			)
		);
		m_routeStep++;
	} 
	else
	{
		MessageBox("NO PATH!","NO PATH!!");
	}
}

void HelloWorld::clearCallback(cocos2d::Ref* pSender)
{
	m_astar.clearObstacles();
	m_people->stopAllActions();

	Vec2 start_pos(100.0f,100.0f);
	m_astar.setStart(getLocation(start_pos));
	m_people->setPosition(correctPosition(start_pos));

	this->removeChild(m_obstacles);
	m_obstacles = Layer::create();
	m_obstacles->setPosition(Vec2(0.0f,0.0f));
	this->addChild(m_obstacles);
}

void HelloWorld::stepCallback()
{
	if (m_routeStep < m_route.size() - 1)
	{
		m_people->runAction(
			Sequence::create(
				CCMoveTo::create(.2f,Vec2(m_route[m_routeStep].m_x*CELL_SIZE,m_route[m_routeStep].m_y*CELL_SIZE)),
				CallFunc::create(CC_CALLBACK_0(HelloWorld::stepCallback,this)),
				NULL
			)
		);
		m_routeStep++;
	}
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* e)
{
	AStarLocation asl = getLocation(touch->getLocation());
	int index = asl.m_y * m_col + asl.m_x;
	if (asl.m_x < m_col && asl.m_y < m_row && asl != m_astar.getStart() && asl != m_astar.getTarget())
	{
		if (m_astar.isObstacle(asl))
		{
			m_astar.clearObstacle(asl);
			m_obstacles->removeChildByTag(index);
		}
		else
		{
			Vec2 pos = correctPosition(touch->getLocation());
			Block* block = Block::createBlock(CELL_SIZE);
			block->setAnchorPoint(Vec2(0.0f,0.0f));
			block->setPosition(pos);
			block->setTag(index);
			block->setNum(index);
			m_obstacles->addChild(block);
			m_astar.setObstacle(asl);
		}
	}
	return false;
}