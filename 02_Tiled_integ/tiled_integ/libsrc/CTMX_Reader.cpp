#include "CTMX_Reader.h"
#include <stdio.h>
#include "tinyxml2/tinyxml2.h"
#include <sstream>
CTMX_Reader::CTMX_Reader() {

}
CTMX_Reader::CTMX_Reader(std::string strTMX_Path,std::string strTSX_Path):
  m_strTMX_path(strTMX_Path),m_strTSX_path(strTSX_Path)
{
  Read_TileSet(strTSX_Path);
  Read_Ctx(strTMX_Path,strTSX_Path);
}

int CTMX_Reader::Create_Layers() {
  sf::RenderTexture renderTexture;
  return 0;
}


CTMX_Reader::~CTMX_Reader() {

}

bool CTMX_Reader::Read_Ctx(std::string strTMX_Path,std::string strTSX_Path) {
  printf("\033[1;33m[%s][%d] :x: chk %s %s\033[m\n",
      __FUNCTION__,__LINE__,m_strTMX_path.c_str(),m_strTSX_path.c_str());

  tinyxml2::XMLDocument xmldoc;
  if ( xmldoc.LoadFile(m_strTMX_path.c_str()))  {
    printf("\033[1;31m[%s][%d] :x: err \033[m\n",__FUNCTION__,__LINE__);
    return false;
  }


  tinyxml2::XMLElement * pRootElement = xmldoc.RootElement();
  printf("\033[1;36m[%s][%d] :x: chk %s\033[m\n",__FUNCTION__,__LINE__,pRootElement->Attribute("version"));
  m_iMapWidth   = std::stoi(pRootElement->Attribute("width"));
  m_iMapHeight  = std::stoi(pRootElement->Attribute("height"));
  printf("\033[1;33m[%s][%d] :x: chk  %d %d\033[m\n",__FUNCTION__,__LINE__,m_iMapWidth,m_iMapHeight);
  m_iTileWidth  = std::stoi(pRootElement->Attribute("tilewidth"));
  m_iTileHeight = std::stoi(pRootElement->Attribute("tileheight"));

  printf("\033[1;33m[%s][%d] :x: chk  %d %d\033[m\n",__FUNCTION__,__LINE__,m_iTileWidth,m_iTileHeight);
  
  m_strTileSetSource = pRootElement->FirstChildElement("tileset")->Attribute("source");
  printf("\033[1;32m[%s][%d] :x: chk [%s]\033[m\n",__FUNCTION__,__LINE__,m_strTileSetSource.c_str());

  tinyxml2::XMLElement* pLayer = pRootElement->FirstChildElement("layer");
  if (pLayer == nullptr) {
    printf("\033[1;31m[%s][%d] :x: ERR,No layer \033[m\n",__FUNCTION__,__LINE__);
    return false;
  }

  while (pLayer) {
    MapLayer Layer;
    Layer.iID     = std::stoi(pLayer->Attribute("id"));
    Layer.strName = pLayer->Attribute("name");
    Layer.iWidth  = std::stoi(pLayer->Attribute("width"));
    Layer.iHeight = std::stoi(pLayer->Attribute("height"));
    auto pData = pLayer->FirstChildElement("data");

    // Create RenderTexture for layer
    auto pRenderTexture = std::make_shared<sf::RenderTexture>();
    pRenderTexture->create(
        Layer.iWidth  * m_iTileWidth,
        Layer.iHeight * m_iTileHeight);
    pRenderTexture->clear(sf::Color(0,0,0,0));
    Layer.pRenderTexture = pRenderTexture;

    std::stringstream ssData(pData->GetText());
    int iIdx;
    while (ssData >> iIdx) {
      if (ssData.peek() == ',') {
        ssData.ignore();
      }
      Layer.vecTileIdx.push_back(iIdx);
    }

    // update the texture to display
    Update_Layer(Layer,m_mapTileSets);
    m_mapLayers[Layer.strName] = Layer;

    
    printf("\033[1;33m[%s][%d] :x: chk  %s\033[m\n",
        __FUNCTION__,__LINE__,pLayer->Attribute("name"));
    pLayer = pLayer->NextSiblingElement("layer");
  }
  
  return true;
}

int CTMX_Reader::Update_Layer(
        MapLayer &Layer,
        std::map<std::string,TileSet> &mapTileSets
        ) {

  std::vector<int> &vecTileIdx = Layer.vecTileIdx;
  auto &pRenderTexture = Layer.pRenderTexture;
  auto &strTileSetName = m_strCurTileSetName;
  int iCnt =0;
  sf::Vector2f vecCoordinate;
  for (int iIdx : vecTileIdx ) {
    int iIdx_X;
    int iIdx_Y;
    iIdx_X = iCnt % m_iMapWidth;
    iIdx_Y = iCnt / m_iMapWidth;

    vecCoordinate.x = m_iTileWidth*iIdx_X;
    vecCoordinate.y = m_iTileHeight*iIdx_Y;
    //printf("\033[1;36m[%s][%d] :x: chk x %d y %d idx %d (%f %f)\033[m\n",
    //    __FUNCTION__,__LINE__,iIdx_X,iIdx_Y,iIdx,
    //    vecCoordinate.x,vecCoordinate.y);

    // iIdx == 0 is blank tile
    if (iIdx) {
      sf::Sprite Sprite = mapTileSets[strTileSetName].mapSprites[iIdx-1];
      Sprite.move(vecCoordinate);
      pRenderTexture->draw(Sprite);
      pRenderTexture->display();
    }
    iCnt++;
  }
  Layer.Sprite.setTexture(pRenderTexture->getTexture());
  Layer.Sprite.move(sf::Vector2f(0,0));
  
  return 0;
}


bool CTMX_Reader::Read_TileSet(std::string strTSX_Path ) 
{
  TileSet tileset;
  tinyxml2::XMLDocument xmldoc;
  if ( xmldoc.LoadFile(m_strTSX_path.c_str()))  {
    printf("\033[1;31m[%s][%d] :x: err \033[m\n",__FUNCTION__,__LINE__);
    return false;
  }
  tinyxml2::XMLElement * pRootElement = xmldoc.RootElement();

  std::string strTileSetName;  
  if (!pRootElement->Attribute("name") ) {
    printf("\033[1;32m[%s][%d] :x: Err can not get tileset name from"
        " [%s] \033[m\n",
        __FUNCTION__,__LINE__,strTSX_Path.c_str());
    return false;
  } else {
    strTileSetName = pRootElement->Attribute("name");
    m_strCurTileSetName = strTileSetName;
  }
  m_mapTileSets[strTileSetName].iTileWidth = 
    std::stoi(pRootElement->Attribute("tilewidth"));
  m_mapTileSets[strTileSetName].iTileHeight = 
    std::stoi(pRootElement->Attribute("tileheight"));
  m_mapTileSets[strTileSetName].iSpacing = 
    std::stoi(pRootElement->Attribute("spacing"));
  m_mapTileSets[strTileSetName].iMargin = 
    std::stoi(pRootElement->Attribute("margin"));
  m_mapTileSets[strTileSetName].iTilecount = 
    std::stoi(pRootElement->Attribute("tilecount"));
  m_mapTileSets[strTileSetName].iColumns = 
    std::stoi(pRootElement->Attribute("columns"));

  auto pImage = pRootElement->FirstChildElement("image");
  m_mapTileSets[strTileSetName].strImgSourceFile = pImage->Attribute("source"); 
  m_mapTileSets[strTileSetName].iImgWidth = 
    std::stoi(pImage->Attribute("width")); 
  m_mapTileSets[strTileSetName].iImgHeight = 
    std::stoi(pImage->Attribute("height")); 

  std::string strImgSourceFile =
    std::string("resources/")+m_mapTileSets[strTileSetName].strImgSourceFile ;
  printf("\033[1;33m[%s][%d] :x: chk %s \033[m\n",__FUNCTION__,__LINE__,strImgSourceFile.c_str());

  if (m_mapTileSets[strTileSetName].Texture.loadFromFile(strImgSourceFile) 
      != true) {
    printf("\033[1;31m[%s][%d] :x: Err \033[m\n",__FUNCTION__,__LINE__);
    return false;
  }
  if (Create_Tiles(m_mapTileSets[strTileSetName].mapSprites,
        m_mapTileSets[strTileSetName].Texture,
        m_mapTileSets[strTileSetName].iTileWidth,
        m_mapTileSets[strTileSetName].iTileHeight,
        m_mapTileSets[strTileSetName].iSpacing,
        m_mapTileSets[strTileSetName].iMargin,
        m_mapTileSets[strTileSetName].iTilecount,
        m_mapTileSets[strTileSetName].iColumns,
        m_mapTileSets[strTileSetName].iImgWidth,
        m_mapTileSets[strTileSetName].iImgHeight) != true) {
    return false;
  }

  return true;
}

bool CTMX_Reader::Create_Tiles(std::map<int,sf::Sprite> &mapSprites,
                        sf::Texture &Texture,
                        int iTileWidth, int iTileHeight, int iSpacing, 
                        int iMargin, int iTilecount, int iColumns, 
                        int iImgWidth, int iImgHeight ) {
  int iHeight = 0;
  int iIdx = 0;
  int iIdx_X = 0;
  int iIdx_Y = 0;
  while (iHeight < iImgHeight ) {

    iIdx_X = iIdx % 18;
    iIdx_Y = iIdx / 18;
    iHeight =iSpacing+(iMargin+iTileHeight)*iIdx_Y;

    if (iHeight >= iImgHeight )
      break;
   // printf("\033[1;36m[%s][%d] :x: chk %d %d %d %d \033[m\n",__FUNCTION__,__LINE__,iHeight,iIdx_X,iIdx_Y,iIdx);

    mapSprites[iIdx] =
    sf::Sprite(
        Texture,sf::IntRect(
          iSpacing+(iMargin+iTileWidth)*iIdx_X,
          iSpacing+(iMargin+iTileHeight)*iIdx_Y,
          iTileWidth,
          iTileHeight
          ));
    iIdx++;
  }



  return true;
}

