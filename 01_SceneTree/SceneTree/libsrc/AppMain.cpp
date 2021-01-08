#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <chrono>

#include "CSceneTree.h"
#include "CButton.h"

sf::RenderWindow& SetupWindow() {
  static sf::RenderWindow window(sf::VideoMode(640, 480), "Scene Tree");
  // Set framerate - 60HZ
  window.setFramerateLimit(60);


  return window;
}
sf::Font& SetupFont() {
  // setup font
  static sf::Font font;
  if (!font.loadFromFile("arial.ttf")){
    printf("\033[1;31m[%s][%d] :x: Font Load Error \033[m\n",
        __FUNCTION__,__LINE__);
    exit(1);
  }
  return font;
}
int Rotate(std::vector<CSceneTree*>& pChildren, sf::Transform& Tf,
           float &fTimeDelta) {
  Tf.rotate(10*fTimeDelta);
  return 0;
}

int CreateSceneTree(CSceneTree* &pRootNode,
                    sf::RenderWindow& window,sf::Font &font) {
  sf::Vector2u vec2uWinSize;
  vec2uWinSize = window.getSize();
 
  pRootNode  = new CSceneNode<CNullNode>("Root"); 
  CSceneNode<CNullNode>* pUILayer;

  pUILayer= new CSceneNode<CNullNode>("UI_LAYER");
  pRootNode->AddChildNode(pUILayer);

  CSceneNode<CNullNode>* pDispLayer;
  pDispLayer = new CSceneNode<CNullNode>("DISP_LAYER");
  pRootNode->AddChildNode(pDispLayer);
 
  CButton* pBtn;
  int iButtonWidth = 150;
  int iCenterPos = (vec2uWinSize.x/2.0) - (iButtonWidth/2.0);
  pBtn= new CButton("BTN_Start","Test Start",window,font,iButtonWidth);
  pBtn->m_Tf.translate(iCenterPos,110);
  pBtn->m_OnClick = Rotate;
 
  pUILayer->AddChildNode(pBtn);
 
  pBtn= new CButton("BTN_Option","Option",window,font,iButtonWidth);
  pBtn->m_Tf.translate(iCenterPos,150);
  pUILayer->AddChildNode(pBtn);
 
  pBtn= new CButton("BTN_Exit","Exit",window,font,iButtonWidth);
  pBtn->m_Tf.translate(iCenterPos,190);
  pUILayer->AddChildNode(pBtn);

  printf("\033[1;33m[%s][%d] Tree Structure  \033[m\n",__FUNCTION__,__LINE__);
  pRootNode->PrintNodeTree(0);
 
  return 0;
}

int AppMain(int argc, char *argv[]) {
  int ret = 0;
  sf::RenderWindow& window = SetupWindow();
  sf::Font& font = SetupFont();

  sf::Event evt;

  bool bWindowFocused = false;
  CSceneTree* pRootNode = nullptr;
  CreateSceneTree(pRootNode,window, font);

  auto now  = std::chrono::system_clock::now();
  auto prev = std::chrono::system_clock::now();
  std::chrono::duration<float> DeltaTime;
  float fTimeDelta;

  // :x: main loop
  while (window.isOpen())
  { 
    now= std::chrono::system_clock::now();
    DeltaTime = now - prev;
    fTimeDelta = DeltaTime.count();
    prev = now;

    while (window.pollEvent(evt)) {
      // window event
      if (evt.type == sf::Event::Closed)
        window.close();

      if (evt.type == sf::Event::KeyReleased &&
          evt.key.code == sf::Keyboard::Q ) {
        printf("\033[1;32m[%s][%d] :x: Quit \033[m\n",__FUNCTION__,__LINE__);
        window.close();
      }
      if (evt.type == sf::Event::GainedFocus) {
        bWindowFocused = true;
        printf("\033[1;33m[%s][%d] :x: focus in\033[m\n",__FUNCTION__,__LINE__);
      } 
      if (evt.type == sf::Event::LostFocus) {
        bWindowFocused = false;
        printf("\033[1;33m[%s][%d] :x: focus out\033[m\n",__FUNCTION__,__LINE__);
      }

      // :x: Event
      if (bWindowFocused)
        pRootNode->evt(evt,sf::Transform::Identity,fTimeDelta);
    }
    // :x: Tick
    //pRootNode->tick(deltaTime,sf::Transform::Identity,fTimeDelta)

    // :x: Draw
    window.clear();
    pRootNode->draw(window,sf::Transform::Identity);
    window.display();

  }
  delete pRootNode;
  return ret;
}
