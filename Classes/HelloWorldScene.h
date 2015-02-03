#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "AStar.h"
using namespace cocos2d;

class HelloWorld : public Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void goCallback(Ref* pSender);
    
	void clearCallback(Ref* pSender);

	void stepCallback();

	bool onTouchBegan(Touch*, Event*);

    CREATE_FUNC(HelloWorld);

private:
	static const int CELL_SIZE = 40;
	int m_row;
	int m_col;
	Layer* m_obstacles;
	AStar m_astar;
	Sprite* m_people;
	Sprite* m_target;
	int m_routeStep;
	std::vector<AStarLocation> m_route;
	inline Vec2 correctPosition(const Vec2& pos) {return Vec2(((int)pos.x/40)*40.0f,((int)pos.y/40)*40.0f);}
	AStarLocation getLocation(const Vec2& pos) {return AStarLocation((int)(pos.x/40),(int)(pos.y/40));}
};

#endif // __HELLOWORLD_SCENE_H__
