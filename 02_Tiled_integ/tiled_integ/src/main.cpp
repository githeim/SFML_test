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

  bool bWindowFocused = false;
  sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
  window.setFramerateLimit(60);

  sf::Texture texture1;

  texture1.loadFromFile("resources/medieval_tilesheet.png"
      ,sf::IntRect(32+(32+64)*0,32+(32+64)*0,64,64)
//      ,sf::IntRect(32+(32+64)*10,32+(32+64)*2,64,64)
      );
  sf::Sprite sprite1(texture1);
  sf::Texture texture2;
  texture2.loadFromFile("resources/medieval_tilesheet.png"
      ,sf::IntRect(32+(32+64)*10,32+(32+64)*3,64,64)
      );
  sf::Sprite sprite2(texture2);
  sprite2.move(sf::Vector2f(64,64));


  while (window.isOpen())
  {
    sf::Event event;
     while (window.pollEvent(event))
    {
       if (event.type == sf::Event::Closed)
        window.close();
       if (event.type == sf::Event::GainedFocus) {
         bWindowFocused = true;
         printf("\033[1;33m[%s][%d] :x: focus in\033[m\n",__FUNCTION__,__LINE__);
       } 
       if (event.type == sf::Event::LostFocus) {
         bWindowFocused = false;
         printf("\033[1;33m[%s][%d] :x: focus out\033[m\n",__FUNCTION__,__LINE__);
       }

    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
      printf("\033[1;36m[%s][%d] :x: Quit Key \033[m\n",__FUNCTION__,__LINE__);
      window.close();
    }


    window.clear();
    window.draw(sprite1);
    window.draw(sprite2);
    window.display();
  }

  return 0;
}
