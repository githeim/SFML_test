#ifndef _CASTAR_H_
#define _CASTAR_H_ 
#include <vector>
#include <stdio.h>
#include <memory>
#include <queue>
#include <functional>
#include <algorithm>
struct AStar_Map {
  std::vector<int> vecData;
  int iStride;
};

struct AStar_OpenSet {
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



  // To compare AStar_OpenSet's F score
  struct FScore_Compare {
    bool operator() (AStar_OpenSet left,AStar_OpenSet right) {
      return left.dF_Score > right.dF_Score;
    }
  };

  std::priority_queue<AStar_OpenSet,std::vector<AStar_OpenSet>,FScore_Compare>
    m_OpenSet;
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



auto Lambda_GetAdjacentNodes = 
  [] (
   const int& iCurIdx,const int& iStride,
   std::function<std::pair<int,int>(int,int)> lambdaIdx2Coord =Lambda_Idx2Coord, 
   std::function<int(const std::pair<int,int> &pairCoord,int iStride)> 
                                              lambdaCoord2Idx=Lambda_Coord2Idx) 
   -> std::vector<int> {
  bool bDiagonalFlag = true;
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
      if ( (iAdjacent_X < 0 || iAdjacent_Y < 0) ) 
        continue;
      vecAdjacents.push_back(
          Lambda_Coord2Idx( 
            std::make_pair<int,int>((int)(iAdjacent_X),(int)iAdjacent_Y),iStride));
    }
  }
  return vecAdjacents;
};
auto Lambda_FilterObstacles = 
  [] (std::vector<int> vecIdx, std::vector<int> vecObstacles)
  -> std::vector<int> 
{
//  vecIdx.erase(
//      std::remove(vecIdx.begin(), vecIdx.end(), iObstacle), vecIdx.end());

};



#endif /* ifndef _CASTA */

