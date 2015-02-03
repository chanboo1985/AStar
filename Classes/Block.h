#ifndef __BLOCK_H__
#define __BLOCK_H__
#include "cocos2d.h"
USING_NS_CC;

class Block : public Node
{
public:
	static Block* createBlock(int wh);

	virtual bool init(int wh);

	void setNum(int num);

	inline int getNum(){return m_num;}

	inline int getWidth() {return m_width;}

	inline int getHeight() {return m_height;}
private:
	int m_width;
	int m_height;
	int m_num;
	Label* m_text;
	LayerColor* m_layer;
};

#endif