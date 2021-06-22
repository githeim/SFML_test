#include <gtest/gtest.h>
#include "libmodule.h"
#include "CAStar.h"


// Test template    

class Test_AStar : public testing::Test {
  public:
    AStar_Map TestMap;
    int k=3;
  protected:
    virtual void SetUp() {
      TestMap.iStride = 10;
      TestMap.iHeight = 6;
      TestMap.vecData = 
      { 
         0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 
         0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 
         0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 
         0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 
         0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 
         0, 0, 0, 0, 0, 1, 0, 1, 0, 0 
      };

    }
    virtual void TearDown() {
    }

};

#if 0 // :x: for test
TEST_F(Test_AStar, Test_GetAdjacentNodes) {
  auto vecExpectAdjacents = std::vector<int> {7,8,9,17,19,27,28,29};
  auto vecExpectAvailableAdjacents = std::vector<int> {8,9,19,28,29};

  EXPECT_EQ(vecExpectAdjacents,
            Lambda_GetAdjacentNodes(18, TestMap.iStride,TestMap.iHeight));

  // get the vector for filtering
  std::vector<int> vecFilterIdx = Lambda_GetFilterIdxFromMap(TestMap.vecData);

  EXPECT_EQ(vecExpectAvailableAdjacents,
      Lambda_FilterObstacles(
        Lambda_GetAdjacentNodes(18, TestMap.iStride,TestMap.iHeight), vecFilterIdx)
      );
}
#endif // :x: for test

#if 0 // :x: for test

struct TT {
  int a,b,c;
};
TEST_F(Test_AStar, Test_Queue) {
  auto cmp = [](TT left, TT right) {
    return left.c > right.c;
  };

  struct compare {
    bool operator() (TT& left,TT& right) {
      return left.c > right.c;
    }
  };
  std::vector<TT> Test;

  //std::priority_queue<TT, std::vector<TT>,decltype(cmp)> MyQ(cmp);
  std::priority_queue<TT, std::vector<TT>,compare> MyQ;

  TT A ={1,1,3};
  TT B ={1,1,5};
  TT C ={1,1,2};
  TT D ={1,1,9};
  MyQ.push(A);
  MyQ.push(B);
  MyQ.push(C);
  MyQ.push(D);

  //auto con = MyQ.c;
  int qsize = MyQ.size();
  for (int i=0 ; i < qsize ; i++) {

    TT item = MyQ.top();
    printf("\033[1;32m[%s][%d] :x: %d %d %d \033[m\n",__FUNCTION__,__LINE__,item.a,item.b,item.c);
    MyQ.pop();
  }

}
#endif // :x: for test

#if 0 // :x: for test
TEST_F(Test_AStar, Test_Idx_Coord_Exchange) {
  // stride 10
  EXPECT_EQ(std::make_pair(2,1), Lambda_Idx2Coord(12,10));
  EXPECT_EQ(std::make_pair(1,2), Lambda_Idx2Coord(21,10));
  EXPECT_EQ(12, Lambda_Coord2Idx(std::make_pair(2,1),10));
  EXPECT_EQ(21, Lambda_Coord2Idx(std::make_pair(1,2),10));

  // stride 4
  EXPECT_EQ(std::make_pair(2,2), Lambda_Idx2Coord(10,4));
  EXPECT_EQ(std::make_pair(3,1), Lambda_Idx2Coord( 7,4));
  EXPECT_EQ(10, Lambda_Coord2Idx(std::make_pair(2,2),4));
  EXPECT_EQ(7,  Lambda_Coord2Idx(std::make_pair(3,1),4));
}
#endif // :x: for test

#if 0// :x: for test
TEST_F(Test_AStar, Test_MapInit) {

  std::shared_ptr<CAStar> pAStar =std::make_shared<CAStar> ();

  pAStar->SetMap(TestMap);

  std::pair<int,int> Start; 
  std::pair<int,int> Dest ; 
  Start= std::make_pair(1,4);
  Dest = std::make_pair(8,1);
  pAStar->SetStart(Start)->SetDest(Dest);
  EXPECT_EQ(std::make_pair(1,4), pAStar->GetStart());
  EXPECT_EQ(std::make_pair(8,1), pAStar->GetDest());
  pAStar->Exec();

  Start= std::make_pair(3,5);
  Dest = std::make_pair(9,5);

  pAStar->SetStart(Start)->SetDest(Dest);
  pAStar->Exec();

  Start= std::make_pair(0,0);
  Dest = std::make_pair(6,5);

  pAStar->SetStart(Start)->SetDest(Dest);
  pAStar->Exec();


}
#endif // :x: for test

#if 0 // :x: for test

TEST_F(Test_AStar, Test_InputMap) {
  EXPECT_EQ(60, TestMap.vecData.size() );

  Lambda_DisplayMap(TestMap);
}
#endif // :x: for test

TEST_F(Test_AStar, Test_GetAStarPath) {
  int iStartIdx;
  int iDestIdx;
  std::vector<int> vecPath;

  iStartIdx = Lambda_Coord2Idx(std::make_pair(1,4),TestMap.iStride);
  iDestIdx  = Lambda_Coord2Idx(std::make_pair(8,1), TestMap.iStride);

  vecPath = Lambda_GetAStarPath(
      iStartIdx,iDestIdx,TestMap.vecData,TestMap.iStride);
  EXPECT_TRUE( vecPath.size() != 0 );
  EXPECT_TRUE( vecPath.back() == iDestIdx );

  vecPath.clear();
  iStartIdx = Lambda_Coord2Idx(std::make_pair(3,5),TestMap.iStride);
  iDestIdx  = Lambda_Coord2Idx(std::make_pair(9,5), TestMap.iStride);

  vecPath = Lambda_GetAStarPath(
      iStartIdx,iDestIdx,TestMap.vecData,TestMap.iStride);
  EXPECT_TRUE( vecPath.size() != 0 );
  EXPECT_TRUE( vecPath.back() == iDestIdx );

  // can not reached path case
  vecPath.clear();
  iStartIdx = Lambda_Coord2Idx(std::make_pair(0,5),TestMap.iStride);
  iDestIdx  = Lambda_Coord2Idx(std::make_pair(6,5), TestMap.iStride);

  vecPath = Lambda_GetAStarPath(
      iStartIdx,iDestIdx,TestMap.vecData,TestMap.iStride);
  EXPECT_TRUE( vecPath.size() == 0 );


  
}

