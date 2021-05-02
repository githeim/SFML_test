#include "CTMX_Reader.h"
#include <stdio.h>
#include "tinyxml2/tinyxml2.h"
#include <sstream>
CTMX_Reader::CTMX_Reader() {
printf("\033[1;36m[%s][%d] :x: chk \033[m\n",__FUNCTION__,__LINE__);

}
CTMX_Reader::CTMX_Reader(std::string strTMX_Path,std::string strTSX_Path):
  m_strTMX_path(strTMX_Path),m_strTSX_path(strTSX_Path)
{
printf("\033[1;36m[%s][%d] :x: chk \033[m\n",__FUNCTION__,__LINE__);
  Read_Ctx(strTMX_Path,strTSX_Path);
  Read_TileSet(strTSX_Path);
}


CTMX_Reader::~CTMX_Reader() {
printf("\033[1;33m[%s][%d] :x: chk \033[m\n",__FUNCTION__,__LINE__);

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

  while (pLayer) {
    MapLayer Layer;
    Layer.iID     = std::stoi(pLayer->Attribute("id"));
    Layer.strName = pLayer->Attribute("name");
    Layer.iWidth  = std::stoi(pLayer->Attribute("width"));
    Layer.iHeight = std::stoi(pLayer->Attribute("height"));
    auto pData = pLayer->FirstChildElement("data");

    std::stringstream ssData(pData->GetText());

    int iIdx;
    while (ssData >> iIdx) {
      if (ssData.peek() == ',') {
        ssData.ignore();
      }
      Layer.vecTileIdx.push_back(iIdx);
    }
    m_mapLayers[Layer.strName] = Layer;

    
    printf("\033[1;33m[%s][%d] :x: chk  %s\033[m\n",
        __FUNCTION__,__LINE__,pLayer->Attribute("name"));
    pLayer = pLayer->NextSiblingElement("layer");
  }
  
  return true;
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
  m_mapTileSets[strTileSetName].Texture.loadFromFile(strImgSourceFile);


  return true;
}

bool CTMX_Reader::Create_Sprites(std::map<int,sf::Sprite> &mapSprites,
                        sf::Texture &texture,
                        int iTilewidth, int iTileheight, int iSpacing, 
                        int iMargin, int iTilecount, int iColumns, 
                        int iImgWidth, int iImgHeight ) {
  int iHeight = 0;
  int iIdx = 0;
  while (iHeight < iImgHeight ) {

  }


  return true;
}

