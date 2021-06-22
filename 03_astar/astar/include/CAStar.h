#ifndef _CASTAR_H_
#define _CASTAR_H_ 
#include <vector>
#include <stdio.h>
#include <memory>
#include <queue>
#include <functional>
#include <algorithm>
#include <cmath>
struct AStar_Map {
  std::vector<int> vecData;
  int iStride;
  int iHeight;
};

struct AStar_NodeSet {
  int     iIdx;
  int     iParentIdx;
  double  dF_Score;
};

class CAStar {
  public:
  CAStar(); 
  ~CAStar(); 
  CAStar* SetMap(AStar_Map& Map);  

  CAStar*            SetStart(const std::pair<int,int>& Start);  
  std::pair<int,int> GetStart();  
  CAStar*            SetDest (const std::pair<int,int>& Dest);  
  std::pair<int,int> GetDest ();  

  void DisplayMap();

  void Exec(); 
  private:
  AStar_Map m_Map;
  std::pair<int,int> m_Start;
  std::pair<int,int> m_Dest;



  // To compare AStar_NodeSet's F score
  struct FScore_Compare {
    bool operator() (AStar_NodeSet left,AStar_NodeSet right) {
      return left.dF_Score > right.dF_Score;
    }
  };

  std::priority_queue<AStar_NodeSet,std::vector<AStar_NodeSet>,FScore_Compare>
    m_OpenSet;
  std::vector<AStar_NodeSet> m_CloseSet;

};
auto Lambda_DisplayMap = [](AStar_Map& map) {
  int iCnt =0;
  for ( int i : map.vecData ) {
    if (iCnt++ % map.iStride == 0) 
      printf("\n");
    printf("%d",i);
  }
  printf("\n");
};

auto Lambda_Idx2Coord =  [](int iIdx,int iStride) -> std::pair<int,int> {
  return std::make_pair(
      (int)iIdx % iStride,
      (int)(iIdx)/iStride
      );
};
auto Lambda_Coord2Idx =  
   [](const std::pair<int,int> &pairCoord,int iStride) -> int {
  return (iStride*pairCoord.second+pairCoord.first);
};



/**
 * @brief Find Adjacent node
 *
 * @param iCurIdx         [IN] index to find the adjacent node
 * @param iStride         [IN] stride of the map
 * @param iHeight         [IN] Height of the map
 * @param bDiagonalFlag   [IN] diagonal path flag if it true the diagonal path 
 *                             will be considered
 * @param lambdaIdx2Coord [IN] lambda func. for change format
 * @param lambdaCoord2Idx [IN] lambda func. for change format
 *
 * @return 
 */
auto Lambda_GetAdjacentNodes = 
  [] (
   const int& iCurIdx,const int& iStride, const int &iHeight,
   bool bDiagonalFlag = true,
   std::function<std::pair<int,int>(int,int)> lambdaIdx2Coord =Lambda_Idx2Coord, 
   std::function<int(const std::pair<int,int> &pairCoord,int iStride)> 
                                              lambdaCoord2Idx=Lambda_Coord2Idx) 
   -> std::vector<int> {
  std::vector<int> vecAdjacents={};
  std::pair<int,int> pairCurCoord = Lambda_Idx2Coord(iCurIdx,iStride); 
  int iCurPos_X = pairCurCoord.first;
  int iCurPos_Y = pairCurCoord.second;

  for (int y = -1 ; y < 2 ; y++ ) {
    for (int x = -1 ; x < 2 ; x++ ) {
      if (y == 0 && x == 0) 
        continue;
      if (bDiagonalFlag == false && 
          (y != 0 && x !=0 ))
        continue;
      int iAdjacent_X = x + iCurPos_X;
      int iAdjacent_Y = y + iCurPos_Y;
      // :x: filter out border condition coordinaions
      if ( iAdjacent_X < 0 || iAdjacent_Y < 0 ||
            iAdjacent_X >= iStride || iAdjacent_Y >= iHeight ) 
        continue;
      vecAdjacents.push_back(
          Lambda_Coord2Idx( 
            std::make_pair<int,int>((int)(iAdjacent_X),(int)iAdjacent_Y),iStride));
    }
  }
  return vecAdjacents;
};
auto Lambda_FilterObstacles = 
  [] (const std::vector<int>& vecIdx, 
      const std::vector<int>& vecFilterIdx)
  -> std::vector<int> 
{
  std::vector<int> vecFilteredIdx = vecIdx;
  for (int iIdx : vecFilterIdx) {
    vecFilteredIdx.erase(
        std::remove(vecFilteredIdx.begin(),vecFilteredIdx.end(),iIdx),
        vecFilteredIdx.end());
  }
  return vecFilteredIdx;
};

auto Lambda_GetDistanceBetweenIdx = [](
    std::pair<int,int> coord_A,
    std::pair<int,int> coord_B,
    int iStride
    ) 
  -> double {
    return (double) std::sqrt(
        pow(coord_A.first - coord_B.first, 2) +
        pow(coord_A.second - coord_B.second, 2)*1.0
        );
};


/**
 * @brief Get filter index vector from map data
 *        0 0 1 0 
 *        1 0 1 0
 *        0 1 1 1 --> 2,4,6,9,10,11  ; get six items from map data
 *
 * @param vecMapIdx[IN]
 *
 * @return vecFilterIdx
 */
auto Lambda_GetFilterIdxFromMap = [] (
    std::vector<int> vecMapIdx)-> std::vector<int>  {
  int iCnt = 0;
  std::vector<int> vecFilterIdx ={};
  for (int iIdx : vecMapIdx) {
    if (iIdx) {
      vecFilterIdx.push_back(iCnt);
    }
    iCnt++;
  }
  return vecFilterIdx;
};
// input : start idx, dest idx, map data, map stride
// output : path idx

/**
 * @brief Get the path through A* algorithm
 *
 * @param iStartIdx [IN]
 * @param iDestIdx  [IN]
 * @param vecMapData[IN]
 * @param iMapStride[IN]
 *
 * @return the index of path if it is failed to find path the size of 
 *         vector should be zero
 */
auto Lambda_GetAStarPath = [] ( int iStartIdx, int iDestIdx,
    std::vector<int> vecMapData, int iMapStride) -> std::vector<int> {
  std::vector<int> vecPath;
  int iMapHeight = vecMapData.size()/iMapStride;
  const int ASTAR_INIT_POS = (-1);
  // To compare AStar_NodeSet's F score
  struct FScore_Compare {
    bool operator() (AStar_NodeSet left,AStar_NodeSet right) {
      return left.dF_Score > right.dF_Score;
    }
  };
  std::priority_queue<AStar_NodeSet,std::vector<AStar_NodeSet>,FScore_Compare>
    OpenSet;
  std::vector<AStar_NodeSet> CloseSet;

  OpenSet.push( (AStar_NodeSet){iStartIdx,ASTAR_INIT_POS,0});
  bool bFindDestFlag = false;
  int iCnt =0;
  while (!OpenSet.empty()) {
    iCnt++;
    AStar_NodeSet CurrentNode = OpenSet.top();
    OpenSet.pop();
    CloseSet.push_back(CurrentNode);
    if (CurrentNode.iIdx == iDestIdx) {
      printf("\033[1;31m[%s][%d] :x: Arrive iIdx = %d \033[m\n",
          __FUNCTION__,__LINE__,CurrentNode.iIdx);
      bFindDestFlag = true;
      break;
    }
    // Find available adjacent points
    auto vecAdjacents =
      Lambda_FilterObstacles(
          Lambda_GetAdjacentNodes(CurrentNode.iIdx, iMapStride,iMapHeight), 
          Lambda_GetFilterIdxFromMap(vecMapData));
    // remove closed items 
    std::vector<int> vecCloseSetIdx;
    for ( AStar_NodeSet Node : CloseSet ) {
      vecCloseSetIdx.push_back(Node.iIdx);
    }
    vecAdjacents = Lambda_FilterObstacles(vecAdjacents,vecCloseSetIdx);
    // push adjacents nodes
    for ( auto iIdxAdjacent : vecAdjacents) {
      double dH = Lambda_GetDistanceBetweenIdx(
          Lambda_Idx2Coord(iIdxAdjacent,iMapStride),
          Lambda_Idx2Coord(iDestIdx,iMapStride),iMapStride);
      double dG = Lambda_GetDistanceBetweenIdx(
          Lambda_Idx2Coord(iIdxAdjacent,iMapStride),
          Lambda_Idx2Coord(iStartIdx,iMapStride),iMapStride);

      double dF = dH + dG;
      OpenSet.push( (AStar_NodeSet){iIdxAdjacent,CurrentNode.iIdx,dF});
    }
  }
printf("\033[1;33m[%s][%d] :x: chk %d\033[m\n",__FUNCTION__,__LINE__,iCnt);
  if (bFindDestFlag == false) {

    return vecPath;
  }

  AStar_NodeSet CloseNode= CloseSet.back();
  while (CloseNode.iParentIdx != -1 ) {
    vecPath.push_back(CloseNode.iIdx);
    auto itr = std::find_if(CloseSet.begin(), CloseSet.end(),
        [CloseNode](AStar_NodeSet item)->bool {
          if (item.iIdx == CloseNode.iParentIdx) 
            return true;
          return false;
        }
        );
    CloseNode = (*itr);
  }
  vecPath.push_back(CloseNode.iIdx);

  std::reverse(vecPath.begin(),vecPath.end());

//  for ( int iItem : vecPath) {
//    printf("\033[1;36m[%s][%d] :x: x : %d y : %d \033[m\n",
//        __FUNCTION__,__LINE__,
//        Lambda_Idx2Coord(iItem,iMapStride).first,
//        Lambda_Idx2Coord(iItem,iMapStride).second
//        );
//  }




  return vecPath;
};

#endif /* ifndef _CASTA */

