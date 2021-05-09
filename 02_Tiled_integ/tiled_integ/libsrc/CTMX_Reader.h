#ifndef _CTMX_READER_H_
#define _CTMX_READER_H_
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

typedef struct {
  int iID;
  int iWidth;
  int iHeight;
  std::string strName;
  std::vector<int> vecTileIdx;
  std::string strEncoding;
  sf::Sprite Sprite;
  std::shared_ptr<sf::RenderTexture> pRenderTexture;
} MapLayer;

typedef struct {
  std::string strImgSourceFile;
  std::string strName;
  int iTileWidth;
  int iTileHeight;
  int iSpacing;
  int iMargin;
  int iTilecount;
  int iColumns;
  int iImgWidth;
  int iImgHeight;
  sf::Texture Texture;
  std::map<int,sf::Sprite> mapSprites;
} TileSet;

class CTMX_Reader {
  public:
    CTMX_Reader();
    CTMX_Reader(std::string strTMX_Path,std::string strTSXPath);
    ~CTMX_Reader();

    bool Read_Ctx(std::string strTMX_Path,std::string strTSX_Path);
    bool Read_TileSet(std::string strTSX_Path);
    bool Create_Tiles(std::map<int,sf::Sprite> &mapSprites,
                        sf::Texture &Texture,
                        int iTilewidth, int iTileheight, int iSpacing, 
                        int iMargin, int iTilecount, int iColumns, 
                        int iImgWidth, int iImgHeight );
    int Create_Layers();
    int Update_Layer(
        MapLayer &Layer,
        std::map<std::string,TileSet> &mapTileSets);

//  private:
    std::string m_strTMX_path;
    std::string m_strTSX_path;

    std::string m_strTileSetSource;
    int m_iMapWidth;
    int m_iMapHeight;

    int m_iTileWidth;
    int m_iTileHeight;
    std::map<std::string,MapLayer> m_mapLayers;

    std::string m_strCurTileSetName;
    std::map<std::string,TileSet> m_mapTileSets;
};
#endif /* ifndef _CTMX_READER_H_ */
