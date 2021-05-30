#include  "CAStar.h"
#define ASTAR_INIT_POS (-1)

CAStar::CAStar() {
  printf("\033[1;33m[%s][%d] :x: Constructor \033[m\n",
      __FUNCTION__,__LINE__);
}
CAStar::~CAStar() {
  printf("\033[1;32m[%s][%d] :x: Destructor \033[m\n",
      __FUNCTION__,__LINE__);
}

CAStar* CAStar::SetMap(AStar_Map& Map) {
  m_Map = Map;
  return this;
}
CAStar* CAStar::SetStart(const std::pair<int,int>& Start) {
  m_Start = Start;
  return this;
}

std::pair<int,int> CAStar::GetStart()  {
  return m_Start;
}
CAStar* CAStar::SetDest (const std::pair<int,int>& Dest) {
  m_Dest = Dest;
  return this;
}
std::pair<int,int> CAStar::GetDest()  {
  return m_Dest;
}

void CAStar::DisplayMap() {
  Lambda_DisplayMap(m_Map);
}
void CAStar::Exec() {
  // register the initial point to open set

//  int iStartIdx = Lambda_Coord2Idx(m_Start,m_Map.iStride);
//  int iDestIdx = Lambda_Coord2Idx(m_Dest,m_Map.iStride);
//
//  m_OpenSet.push( (AStar_OpenSet){iStartIdx,ASTAR_INIT_POS,0});
//  while (!m_OpenSet.empty()) {
//    AStar_OpenSet item = m_OpenSet.top();
//    m_OpenSet.pop();
//    // Add available adjacent points
//
//  }
}
