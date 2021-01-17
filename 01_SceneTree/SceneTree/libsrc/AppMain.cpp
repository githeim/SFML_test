#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <chrono>
#include <math.h>

#include "CSceneTree.h"
#include "CButton.h"
#include "CActor.h"

int OnEvtCoordCursor(sf::Event& evt,const sf::Transform& Tf,
    float& fTimeDelta,CSceneTree* pSceneNode) {
  if (evt.type == sf::Event::MouseMoved) {
    CSceneNode<sf::Text> *pTextCoord =(CSceneNode<sf::Text>*)pSceneNode;
    pTextCoord->m_Tf = sf::Transform::Identity;
    pTextCoord->m_Tf.translate(evt.mouseMove.x+10,evt.mouseMove.y);
    std::string strCoord ="x = "+std::to_string(evt.mouseMove.x)+ " y = "+
                          std::to_string(evt.mouseMove.y) ;
    pTextCoord->m_Entity.setString(strCoord);

  }

  return 0;
}

int OnClick_BTN_Test_Start(std::vector<CSceneTree*>& vecChildren, 
                           sf::Transform& Tf, float& fTimeDelta)
{
  printf("\033[1;36m[%s][%d] :x: Click \033[m\n",__FUNCTION__,__LINE__);
  SetCurrentScene (GetSceneTreeRoot()->FindNode("SCENE_SCENARIO_SELECT"));
  return 0;
}

int OnClick_BTN_Back(std::vector<CSceneTree*>& vecChildren, 
                           sf::Transform& Tf, float& fTimeDelta)
{
  printf("\033[1;36m[%s][%d] :x: Click \033[m\n",__FUNCTION__,__LINE__);

  SetCurrentScene (GetSceneTreeRoot()->FindNode("SCENE_MAIN_MENU"));
  return 0;
}

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

int CreateBtnMenu(CSceneTree* &pMenuNode, std::string strMenuName,
                  std::string strMenuTitle,
                  std::vector<std::string> &vecMenuItem,
                  sf::RenderWindow& window,
                  sf::Font &font,
                  int iWidth=200,int iPosX=-1,int iPosY=-1) {
  // interval between buttons
  int iHeightInterval = 40;

  // get window size
  sf::Vector2u vec2uWinSize;
  vec2uWinSize = window.getSize();

  pMenuNode  = new CSceneNode<CNullNode>("MENU_"+strMenuName); 

  CButton* pBtn;
  int iMenuPosX = iPosX;
  int iMenuPosY = iPosY;

  int iButtonWidth = iWidth - ((float)iWidth/4.0f);
  if (iMenuPosX == -1  && iMenuPosY == -1) {
    iMenuPosX =(int)( ((float)vec2uWinSize.x/2.0f) - ((float)iButtonWidth/2.0));
    iMenuPosY = 40;
  }

  // :x: Create Border
  CSceneNode<sf::RectangleShape> *pMenuBorder =
    new CSceneNode<sf::RectangleShape>(strMenuName+std::string("_BORDER"));

  sf::Vector2f vecBorderSize = 
                { (float)iWidth,(float)(iHeightInterval*(vecMenuItem.size()+2.0f))};
  pMenuBorder->m_Entity.setSize(vecBorderSize);
  pMenuBorder->m_Entity.setFillColor(sf::Color(0,40,0));
  pMenuBorder->m_Entity.setOutlineThickness(1.0f);
  pMenuBorder->m_Tf.translate(iMenuPosX,1);
  pMenuNode->AddChildNode(pMenuBorder);

  // :x: Create Text
  CSceneNode<sf::Text> *pMenuName = 
    new CSceneNode<sf::Text>(strMenuName+std::string("_TXT"));
  pMenuName->m_Entity.setFont(font);
  pMenuName->m_Entity.setString(strMenuTitle);
  pMenuName->m_Entity.setCharacterSize(20);
  pMenuName->m_Entity.setFillColor(sf::Color::Yellow);
  pMenuName->m_Tf.translate(0,5);

  pMenuBorder->AddChildNode(pMenuName);
  
  int cnt = 0;
  for ( auto strItem : vecMenuItem) {
    pBtn= new CButton("BTN_"+strItem,strItem,window,font,iButtonWidth);
    if (pBtn == nullptr) {
      printf("\033[1;31m[%s][%d] :x: ERR] Button creation  \033[m\n",
          __FUNCTION__,__LINE__);
      return -1;
    }
    //pBtn->m_Tf.translate(iMenuPosX,iMenuPosY+(cnt*iHeightInterval) );
    pBtn->m_Tf.translate(5,iMenuPosY+(cnt*iHeightInterval) );
    cnt++;
    pMenuBorder->AddChildNode(pBtn);
  }

  return 0;
}

int CreateBaseScene(std::string  strSceneName,
                    CSceneTree* &pRootScene) {
  pRootScene  = new CSceneNode<CNullNode>(strSceneName); 
  CSceneNode<CNullNode>* pSceneNode;

  pSceneNode = new CSceneNode<CNullNode>("UI_LAYER");
  pRootScene->AddChildNode(pSceneNode);

  pSceneNode = new CSceneNode<CNullNode>("STAGE_LAYER");
  pRootScene->AddChildNode(pSceneNode);
 
  return 0;
}

/**
 * @brief Create select menu template scene
 *
 * @param pScene
 * @param strSceneName
 * @param strMenuTitle
 * @param strMenuID
 * @param vecMenuItem
 * @param window
 * @param 
 *
 * @return 
 */
int CreateTmplScene_Select_Menu(
    CSceneTree* & pScene,
    std::string strSceneName,
    std::string strMenuTitle,
    std::string strMenuID,
    std::vector<std::string> &vecMenuItem,
    sf::RenderWindow& window,sf::Font &font
    ) {
  CSceneTree* pUI_LAYER = nullptr;
  CSceneTree* pMenuNode = nullptr;
  // Create Scene Node ; MAIN_MENU
  CreateBaseScene(strSceneName, pScene);

  // Get UI_Layer from pScene
  pUI_LAYER = pScene->FindNode("UI_LAYER",pScene);
  if (pUI_LAYER == nullptr) {
    printf("\033[1;31m[%s][%d] :x: Error \033[m\n",__FUNCTION__,__LINE__);
    return -1;
  }

  // Create Button Menu
  if ( 
      CreateBtnMenu(pMenuNode,
                    strMenuID,
                    strMenuTitle,
                    vecMenuItem,
                    window,font)) {
    printf("\033[1;36m[%s][%d] :x: ERR ] Create BtnMenu \033[m\n",
        __FUNCTION__,__LINE__);
    return -1;
  }
  // Attach Button Menu to the UI Layer of SCENE_MAIN_MENU
  pUI_LAYER->AddChildNode(pMenuNode);

  return 0;
}
CSceneNode<sf::Text>* CreateCoordCursor(sf::Font &font){
  // Add Coordination cursor(show coordination)
  CSceneNode<sf::Text> *pTextCoord =new CSceneNode<sf::Text>("TEXT_COORD_CURSOR");
  pTextCoord->m_Entity.setFont(font);
  pTextCoord->m_Entity.setString("COORD");
  pTextCoord->m_Entity.setCharacterSize(18);
  pTextCoord->m_Entity.setFillColor(sf::Color::Cyan);
  pTextCoord->m_Tf.translate(200,300);
  pTextCoord->m_OnEvt = OnEvtCoordCursor;

  return pTextCoord;
}

CSceneTree* CreateScene_SCENARIO_SELECT(sf::RenderWindow& window,sf::Font &font) 
{
  CSceneTree* pScene = nullptr;
  std::vector<std::string> vecMenuItem = 
                       {"SCENARIO1","SCENARIO2","SCENARIO3","SCENARIO4","Back"};
  CreateTmplScene_Select_Menu(
      pScene,
      "SCENE_SCENARIO_SELECT",
      "Scenario Select",
      "SCENARIO_SELECT",
      vecMenuItem,
      window,font);

  auto BTN_Back = (CButton*)pScene->FindNode("BTN_Back",pScene);
  BTN_Back->m_OnClick = OnClick_BTN_Back;
  

  CActor* pActor00 = new CActor("Actor00");

  pScene->FindNode("STAGE_LAYER",pScene)->AddChildNode(pActor00);

  // Add Coordination cursor(show coordination)
  CSceneNode<sf::Text> *pTextCoord =CreateCoordCursor(font);
  pScene->FindNode("STAGE_LAYER", pScene)->AddChildNode(pTextCoord);


  return pScene;
}
CSceneTree* CreateScene_MAIN_MENU(sf::RenderWindow& window,sf::Font &font) {
  CSceneTree* pScene = nullptr;
  std::vector<std::string> vecMenuItem;
  vecMenuItem = {"Test Start","Option","Exit"};
  CreateTmplScene_Select_Menu(
      pScene,
      "SCENE_MAIN_MENU",
      "Automation test",
      "MainMenu",
      vecMenuItem,
      window,font);

  // Add Button callback
  auto BTN_Test_Start = (CButton*)pScene->FindNode("BTN_Test_Start",pScene);
  if (!BTN_Test_Start) {
    printf("\033[1;31m[%s][%d] :x: Err Can't Find BTN_Test_Start\033[m\n",
        __FUNCTION__,__LINE__);
  }
  BTN_Test_Start->m_OnClick = OnClick_BTN_Test_Start;

  // Add Coordination cursor(show coordination)
  CSceneNode<sf::Text> *pTextCoord =CreateCoordCursor(font);
  pScene->FindNode("STAGE_LAYER", pScene)->AddChildNode(pTextCoord);


  return pScene;
}
/**
 * @brief create scene tree
 *
 * @param pRootNode[OUT]
 * @param window[IN]
 * @param font[IN]
 *
 * @return 
 */
int CreateSceneTree(CSceneTree* &pRootNode,
                    sf::RenderWindow& window,sf::Font &font) {

  sf::Vector2u vec2uWinSize;
  vec2uWinSize = window.getSize();
  CSceneTree* pScene = nullptr;
 
  std::string strMenuTitle;
  std::string strMenuID;
  std::vector<std::string> vecMainMenuItem;

  // Create Root Node
  pRootNode  = new CSceneNode<CNullNode>("Root"); 
  SetSceneTreeRoot(pRootNode);
  
  if (pRootNode->FindNode("Root") == nullptr) {
    printf("\033[1;31m[%s][%d] :x: Err \033[m\n",__FUNCTION__,__LINE__);
    return -1;
  }
  std::vector<std::string> vecMenuItem;

  pScene = CreateScene_MAIN_MENU(window, font);
  pRootNode->AddChildNode(pScene);

  pScene = CreateScene_SCENARIO_SELECT(window,font);
  pRootNode->AddChildNode(pScene);

  return 0;
}

int AppMain(int argc, char *argv[]) {
  int ret = 0;
  sf::RenderWindow& window = SetupWindow();
  sf::Font& font = SetupFont();

  sf::Event evt;

  bool bWindowFocused = false;
  CSceneTree* pRootNode = nullptr;
  CSceneTree* pCurrentScene = nullptr;
  CreateSceneTree(pRootNode,window, font);
  pRootNode->PrintNodeTree(0);
#if 1 // :x: for test
   
  printf("\033[1;32m[%s][%d] :x: %f \033[m\n",__FUNCTION__,__LINE__,atan2(-0.5, -0.5)*180/M_PI);
#endif // :x: for test

  SetCurrentScene (GetSceneTreeRoot()->FindNode("SCENE_MAIN_MENU"));
  pCurrentScene = GetCurrentScene();
  auto now  = std::chrono::system_clock::now();
  auto prev = std::chrono::system_clock::now();
  std::chrono::duration<float> DeltaTime;
  float fTimeDelta;

  // :x: main loop
  while (window.isOpen())
  { 
    if (GetCurrentScene() != pCurrentScene) {
      pCurrentScene = GetCurrentScene();
    }
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
      if (evt.type == sf::Event::KeyReleased &&
          evt.key.code == sf::Keyboard::Num1 ) {
        printf("\033[1;32m[%s][%d] :x: SceneChange \033[m\n",
            __FUNCTION__,__LINE__);
        SetCurrentScene(
            pRootNode->FindNode("SCENE_MAIN_MENU",pRootNode));
      }
      if (evt.type == sf::Event::KeyReleased &&
          evt.key.code == sf::Keyboard::Num2 ) {
        printf("\033[1;32m[%s][%d] :x: SceneChange \033[m\n",
            __FUNCTION__,__LINE__);
        SetCurrentScene(
            pRootNode->FindNode("SCENE_SCENARIO_SELECT",pRootNode));
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
       pCurrentScene->evt(evt,sf::Transform::Identity,fTimeDelta);
    }
    // :x: Event
    //if (bWindowFocused)
    //  pCurrentScene->evt(evt,sf::Transform::Identity,fTimeDelta);

    // :x: Tick
    pCurrentScene->tick(sf::Transform::Identity,fTimeDelta);

    // :x: Draw
    window.clear();
    pCurrentScene->draw(window,sf::Transform::Identity);
    window.display();

  }
  delete pRootNode;
  return ret;
}
