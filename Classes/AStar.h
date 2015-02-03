#ifndef __A_STAR_H__
#define __A_STAR_H__
#include <math.h>
#include <list>
#include <vector>
using namespace std;

struct AStarLocation
{
public:
	int m_x;
	int m_y;

public:
	AStarLocation(){}

	AStarLocation(int x, int y)
		: m_x(x),m_y(y)
	{

	}

	~AStarLocation(){}

	inline void operator =(const AStarLocation& asl)
	{
		this->m_x = asl.m_x;
		this->m_y = asl.m_y;
	}

	inline void operator +=(const AStarLocation& asl)
	{
		this->m_x += asl.m_x;
		this->m_y += asl.m_y;
	}

	inline bool operator ==(const AStarLocation& asl) const
	{
		return this->m_x == asl.m_x && this->m_y == asl.m_y;
	}

	inline bool operator !=(const AStarLocation& asl) const
	{
		return this->m_x != asl.m_x || this->m_y != asl.m_y;
	}
};

struct CellData 
{
public:
	int m_h;
	int m_g;
	AStarLocation m_location;
	AStarLocation m_parentLocation;
public:
	CellData(){}
	CellData(const AStarLocation& asl)
		: m_location(asl)
	{

	}
	~CellData(){}
};

class AStar
{
public:
	AStar();
	~AStar(){}
	bool setMapSize(int row, int col);//设置场景尺寸
	bool isHavePaths();//是否有路径
	bool findPaths(vector<AStarLocation>* arrs);//获取路径
	void setObstacle(const AStarLocation& asl);//设置阻隔点
	void clearObstacle(const AStarLocation& asl);//清除阻隔点
	void clearObstacles();//清除所有的阻隔点
	bool isObstacle(const AStarLocation& asl);//判断是否是阻隔点
	inline int getRow()
	{
		return m_row;
	}
	inline int getCol()
	{
		return m_col;
	}
	inline void setStart(const AStarLocation& asl){m_start = asl;}
	inline void setTarget(const AStarLocation& asl){m_target = asl;}
	inline const AStarLocation& getStart(){return m_start;}
	inline const AStarLocation& getTarget(){return m_target;}
	inline void setDiagonalEnable(bool isEnable){m_aroundNum = isEnable ? 8 : 4;}
private:
	unsigned char* m_map;
	int m_row;
	int m_col;
	AStarLocation m_start;
	AStarLocation m_target;
	int m_aroundNum;
	int m_gAdd[8];
	AStarLocation m_arounds[8];
	list<CellData> m_openList;
	list<CellData> m_closeList;
private:
	bool isBestCell(CellData* cd);
	bool isInList(list<CellData>& arr, const AStarLocation& asl);
	bool judgeAround(const AStarLocation& asl, const AStarLocation& pAsl, int g);
	CellData* getCellDataByList(list<CellData>& arr, const AStarLocation& asl);
	inline int getH(const AStarLocation& asl){
		return (abs(m_target.m_x - asl.m_x) + abs(m_target.m_y - asl.m_y)) * 10;
	}
};


#endif