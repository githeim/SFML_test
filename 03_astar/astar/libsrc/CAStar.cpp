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
  m_OpenSet = 
    std::priority_queue<AStar_NodeSet,std::vector<AStar_NodeSet>,FScore_Compare>
    ();
  m_CloseSet.clear();
  // register the initial point to open set
  int iStartIdx = Lambda_Coord2Idx(m_Start,m_Map.iStride);
  int iDestIdx = Lambda_Coord2Idx(m_Dest,m_Map.iStride);
  int iStride = m_Map.iStride;

  m_OpenSet.push( (AStar_NodeSet){iStartIdx,ASTAR_INIT_POS,0});
  while (!m_OpenSet.empty()) {
    AStar_NodeSet CurrentNode = m_OpenSet.top();

    m_OpenSet.pop();
    m_CloseSet.push_back(CurrentNode);
    if (CurrentNode.iIdx == iDestIdx) {
      printf("\033[1;31m[%s][%d] :x: Arrive iIdx = %d \033[m\n",
          __FUNCTION__,__LINE__,CurrentNode.iIdx);

      break;
    }
    // Find available adjacent points
    auto vecAdjacents =
      Lambda_FilterObstacles(
          Lambda_GetAdjacentNodes(CurrentNode.iIdx, m_Map.iStride,m_Map.iHeight), 
          Lambda_GetFilterIdxFromMap(m_Map.vecData));
    // remove closed items 
    std::vector<int> vecCloseSetIdx;
    for ( AStar_NodeSet Node : m_CloseSet ) {
      vecCloseSetIdx.push_back(Node.iIdx);
    }
    vecAdjacents =Lambda_FilterObstacles(vecAdjacents,vecCloseSetIdx);

    // push adjacents nodes
    for ( auto iIdxAdjacent : vecAdjacents) {
      double dH = Lambda_GetDistanceBetweenIdx(
          Lambda_Idx2Coord(iIdxAdjacent,iStride),
          Lambda_Idx2Coord(iDestIdx,iStride),iStride);
      double dG = Lambda_GetDistanceBetweenIdx(
          Lambda_Idx2Coord(iIdxAdjacent,iStride),
          Lambda_Idx2Coord(iStartIdx,iStride),iStride);

      double dF = dH+ dG;
      m_OpenSet.push( (AStar_NodeSet){iIdxAdjacent,CurrentNode.iIdx,dF});
    }
  }

  AStar_NodeSet CloseNode= m_CloseSet.back();
  std::vector<std::pair<int,int>> m_vecPath;
  while (CloseNode.iParentIdx != -1 ) {
    m_vecPath.push_back(Lambda_Idx2Coord(CloseNode.iIdx,iStride));
    auto itr = std::find_if(m_CloseSet.begin(), m_CloseSet.end(),
        [CloseNode](AStar_NodeSet item)->bool {
          if (item.iIdx == CloseNode.iParentIdx) 
            return true;
          return false;
        }
        );
    CloseNode = (*itr);
  }
  m_vecPath.push_back(Lambda_Idx2Coord(CloseNode.iIdx,iStride));
  

  for ( std::vector<std::pair<int,int>>::reverse_iterator itr = m_vecPath.rbegin() ; 
      itr != m_vecPath.rend(); ++itr) {
    printf("\033[1;36m[%s][%d] :x: x : %d y : %d \033[m\n",
        __FUNCTION__,__LINE__,(*itr).first,(*itr).second);
  }

  
printf("\033[1;33m[%s][%d] :x: End \033[m\n",__FUNCTION__,__LINE__);

}
