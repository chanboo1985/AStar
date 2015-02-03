#include "AStar.h"
#include "cocos2d.h"

AStar::AStar():
	m_aroundNum(8),
	m_map(0),
	m_row(0),
	m_col(0)
{
	m_arounds[0] = AStarLocation(0,-1);
	m_arounds[1] = AStarLocation(0,1);
	m_arounds[2] = AStarLocation(-1,0);
	m_arounds[3] = AStarLocation(1,0);
	m_arounds[4] = AStarLocation(-1,-1);
	m_arounds[5] = AStarLocation(1,-1);
	m_arounds[6] = AStarLocation(-1,1);
	m_arounds[7] = AStarLocation(1,1);

	for (int i = 0; i < 8; i++)
	{
		if (i < 4)
		{
			m_gAdd[i] = 10;
		} 
		else
		{
			m_gAdd[i] = 14;
		}
	}

	clearObstacles();
}

bool AStar::setMapSize(int row, int col)
{
	if (row <= 0 || col <= 0)
	{
		return false;
	}
	CC_SAFE_DELETE_ARRAY(m_map);
	int size = row*col/8+1;
	m_map = new unsigned char[size];
	memset(m_map,0,size);
	m_row = row;
	m_col = col;
	return true;
}

bool AStar::isHavePaths()
{
	m_openList.clear();
	m_closeList.clear();

	if (m_row <= 0 || m_col <= 0)
	{
		return false;
	}
	CellData cd(m_target);
	cd.m_g = 0;
	cd.m_h = getH(m_target);
	m_openList.push_back(cd);
	while (!m_openList.empty())
	{
		isBestCell(&cd);
		m_closeList.push_back(cd);

		AStarLocation asl;
		for (int i = 0; i < m_aroundNum; i++)
		{
			asl = cd.m_location;
			asl += m_arounds[i];
			if (judgeAround(asl,cd.m_location,cd.m_g+m_gAdd[i]))
			{
				return true;
			}
		}
	}
	return false;
}

bool AStar::findPaths(vector<AStarLocation>* arrs)
{
	arrs->clear();

	CellData* cd = getCellDataByList(m_openList,m_start);
	int size = m_openList.size();
	if (cd != nullptr)
	{
		arrs->push_back(cd->m_location);

		cd = getCellDataByList(m_closeList,cd->m_parentLocation);
		while (cd->m_location != m_target)
		{
			arrs->push_back(cd->m_location);
			cd = getCellDataByList(m_closeList,cd->m_parentLocation);
		}
		arrs->push_back(cd->m_location);
		return true;
	}
	return false;
}

void AStar::setObstacle(const AStarLocation& asl)
{
	if (asl.m_x >= 0 && asl.m_x < m_col && asl.m_y >= 0 && asl.m_y < m_row)
	{
		int index = asl.m_y * m_col + asl.m_x;
		m_map[index/8] |= 1 << (index%8);
	}
}

void AStar::clearObstacle(const AStarLocation& asl)
{
	if (asl.m_x >= 0 && asl.m_x < m_col && asl.m_y >= 0 && asl.m_y < m_row)
	{
		int index = asl.m_y * m_col + asl.m_x;
		m_map[index/8] &= ~(1 << (index%8));
	}
}

void AStar::clearObstacles()
{
	if (m_map)
	{
		int size = m_col*m_row/8 + 1;
		memset(m_map,0,size);
	}
	m_openList.clear();
	m_closeList.clear();
}

bool AStar::isObstacle(const AStarLocation& asl)
{
	if (asl.m_x >= 0 && asl.m_x < m_col && asl.m_y >= 0 && asl.m_y < m_row)
	{
		int index = asl.m_y * m_col + asl.m_x;
		return m_map[index/8] & (1 << (index%8));
	}
}

bool AStar::isBestCell(CellData* cd)
{
	list<CellData>::iterator begin = m_openList.begin();
	list<CellData>::const_iterator end = m_openList.end();
	if (begin != end)
	{
		cd->m_g = begin->m_g;
		cd->m_h = begin->m_h;
		cd->m_location = begin->m_location;
		cd->m_parentLocation = begin->m_parentLocation;

		begin ++;

		while (begin != end)
		{
			if (cd->m_g + cd->m_h > begin->m_g + begin->m_h)
			{
				cd->m_g = begin->m_g;
				cd->m_h = begin->m_h;
				cd->m_location = begin->m_location;
				cd->m_parentLocation = begin->m_parentLocation;
			}
			begin++;
		}

		begin = m_openList.begin();
		while (begin->m_location != cd->m_location)
		{
			begin++;
		}

		m_openList.erase(begin);
		return true;
	}
	return false;
}

bool AStar::isInList(list<CellData>& arr, const AStarLocation& asl)
{
	list<CellData>::iterator begin = arr.begin();
	list<CellData>::const_iterator end = arr.end();
	while (begin != end)
	{
		if (asl == begin->m_location)
		{
			return true;
		}
		begin++;
	}
	return false;
}

bool AStar::judgeAround(const AStarLocation& asl, const AStarLocation& pAsl, int g)
{
	if (asl.m_x >= 0 && asl.m_x < m_col && asl.m_y >= 0 && asl.m_y < m_row && !isInList(m_closeList,asl) && !isObstacle(asl))
	{
		CellData* cd = getCellDataByList(m_openList, asl);
		if (cd && cd->m_g > g)
		{
			cd->m_g = g;
			cd->m_parentLocation = pAsl;
		}else if (!cd)
		{
			CellData newCd(asl);
			newCd.m_g = g;
			newCd.m_h = getH(asl);
			newCd.m_parentLocation = pAsl;

			m_openList.push_back(newCd);

			if (newCd.m_location == m_start)
			{
				return true;
			}
		}
	}
	return false;
}

CellData* AStar::getCellDataByList(list<CellData>& arr, const AStarLocation& asl)
{
	list<CellData>::iterator begin = arr.begin();
	list<CellData>::const_iterator end = arr.end();
	while (begin != end)
	{
		if (asl	== begin->m_location)
		{
			return &(*begin);
		}
		begin++;
	}
	return nullptr;
}