#include <stdio.h>
#include <memory>
#include "libmodule.h"
#include "tinyxml2.h"
#include "CTMX_Reader.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Transformable.hpp>

int main(int argc, char *argv[]) {
  printf("Project tiled_integ\n");
  printf("libmodule() = %d\n", testmodule_Test());

  std::string strTMX_Path = "resources/test00.tmx";
  std::string strTSX_Path = "resources/medieval_tilesheet.tsx";
  
  std::shared_ptr<CTMX_Reader> pReader = 
    std::make_shared<CTMX_Reader>(strTMX_Path ,
                                  strTSX_Path );

  sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
  window.setFramerateLimit(60);

  while (window.isOpen())
  {
    sf::Event event;
     while (window.pollEvent(event))
    {
       if (event.type == sf::Event::Closed)
        window.close();
       if (event.type == sf::Event::GainedFocus) {
         printf("\033[1;33m[%s][%d] :x: focus in\033[m\n",__FUNCTION__,__LINE__);
       } 
       if (event.type == sf::Event::LostFocus) {
         printf("\033[1;33m[%s][%d] :x: focus out\033[m\n",__FUNCTION__,__LINE__);
       }

    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
      printf("\033[1;36m[%s][%d] :x: Quit Key \033[m\n",__FUNCTION__,__LINE__);
      window.close();
    }


    window.clear();
    sf::Sprite &Terrain = pReader->m_mapLayers["Terrain"].Sprite;
    sf::Sprite &Road = pReader->m_mapLayers["road"].Sprite;
    sf::Sprite &Building = pReader->m_mapLayers["building"].Sprite;
    Terrain.setScale(0.25,0.25);
    Road.setScale(0.25,0.25);
    Building.setScale(0.25,0.25);
    window.draw(Terrain);
    window.draw(Road);
    window.draw(Building);
    window.display();
  }

  return 0;
}
