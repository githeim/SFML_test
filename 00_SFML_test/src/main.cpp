#include <string>
#include <SFML/Graphics.hpp>
#include <thread>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "CSceneTree.h"


int main()
{

#if 1 // :x: for test
        sf::Transform tfBase;
        sf::Transform tfTrans;
        sf::Transform tfRotate;
        sf::Transform tfTestScale;
        sf::Vector2f testpoint;
        sf::Vector2f vec2Point = {3,0};
        sf::Vector2f outputPoint;
        tfTrans.translate(2,0);
        tfRotate.rotate(90);
        tfTestScale.scale(2.0,1.0);
        tfBase *=tfTestScale; 
        tfBase *=tfTrans; 
        tfBase *=tfRotate; 

        testpoint = tfBase.transformPoint(vec2Point);
        printf("\033[1;37m[%s][%d] :x: chk %f %f \033[m\n",
                                 __FUNCTION__,__LINE__,testpoint.x,testpoint.y);

        tfBase = tfBase.getInverse();

        //tfBase = sf::Transform::Identity;
        //tfBase *=tfRotate.getInverse(); 
        //tfBase *=tfTrans.getInverse(); 

        testpoint = tfBase.transformPoint(testpoint);
        printf("\033[1;37m[%s][%d] :x: chk %f %f \033[m\n",
                                 __FUNCTION__,__LINE__,testpoint.x,testpoint.y);

#endif // :x: for test




  sf::RenderWindow window(sf::VideoMode(400, 200), "SFML works!");

  // setup font
  sf::Font font;
  if (!font.loadFromFile("arial.ttf")){
    printf("\033[1;31m[%s][%d] :x: Font Load Error \033[m\n",
        __FUNCTION__,__LINE__);
    exit(1);
  }
  int iFontSize = 20;

  // Get window size
  sf::Vector2u vec2uWinSize;
  vec2uWinSize = window.getSize();

  float fLayoutWidth, fLayoutHeight;
  fLayoutWidth =  (float)(vec2uWinSize.x/2.0);
  fLayoutHeight = (float)(vec2uWinSize.y/2.0);




  CSceneTree * pRoot;

  CSceneNode<sf::Text> *pTxtTestStart = 
    new CSceneNode<sf::Text>(std::string("TXT_TESTSTART"));
  pTxtTestStart->m_Entity.setFont(font);
  pTxtTestStart->m_Entity.setString("Test start");
  pTxtTestStart->m_Entity.setCharacterSize(iFontSize);
  pTxtTestStart->m_Entity.setFillColor(sf::Color::Green);
  sf::FloatRect txtRect = pTxtTestStart->m_Entity.getGlobalBounds();


  CSceneNode<sf::RectangleShape> *pRectTxtBorder = new CSceneNode<sf::RectangleShape>("RECT_TXTBORDER");
  //pRectTxtBorder->m_Entity.setSize(sf::Vector2f(89,14+20) );
  pRectTxtBorder->m_Entity.setSize(sf::Vector2f(txtRect.width,txtRect.height+(iFontSize/2.0f)) );
  pRectTxtBorder->m_Entity.setFillColor(sf::Color(0,20,0));
  pRectTxtBorder->m_Entity.setOutlineThickness(1.0f);
  pRectTxtBorder->m_Tf.translate(50,5);
  pRectTxtBorder->m_Tf.rotate(30);

  CSceneNode<sf::RectangleShape> *pRectMenuBoundary = 
                            new CSceneNode<sf::RectangleShape>("RECT_MENU");
  pRectMenuBoundary->m_Entity.setSize(sf::Vector2f(110,80));
  pRectMenuBoundary->m_Entity.setFillColor(sf::Color(0,80,0));
  pRectMenuBoundary->m_Entity.setOutlineThickness(1.0f);
  pRectMenuBoundary->m_Tf.translate(10.0f,10.0f);
  pRectMenuBoundary->m_Tf.rotate(0.0f);



  CSceneNode<sf::RectangleShape> *pRectBoundary = 
                            new CSceneNode<sf::RectangleShape>("RECT_BOUNDARY");
  pRectBoundary->m_Entity.setSize(
                                  sf::Vector2f(fLayoutWidth, fLayoutHeight) );
  pRectBoundary->m_Entity.setFillColor(sf::Color(0,0,0));
  pRectBoundary->m_Entity.setOutlineThickness(1.0f);
  pRectBoundary->m_Tf.translate(fLayoutWidth/2.0f,fLayoutHeight/2.0f);


  pRoot = pRectBoundary;
  pRectTxtBorder->AddChildNode(pTxtTestStart);
  pRectMenuBoundary->AddChildNode(pRectTxtBorder);
  pRoot->AddChildNode(pRectMenuBoundary);



  

  static int cnt = 0;
  // Set framerate 
  window.setFramerateLimit(60);
  

  
  bool bWindowFocused = false;
  bool bMousePressed = false;
  sf::Text text;
  text.setFont(font);
  text.setString("Test Start");
  text.setCharacterSize(24);
  sf::Vector2u vec2BaseSize = window.getSize();
  sf::Vector2u vec2CurrentSize = window.getSize();
  printf("\033[1;32m[%s][%d] :x: Base Size(%u %u) \033[m\n",
      __FUNCTION__,__LINE__,vec2BaseSize.x,vec2BaseSize.y);

  sf::Transform tfScale= sf::Transform::Identity; 
  
  sf::Vector2i vec2iLocalPos;
  while (window.isOpen())
  {
    cnt++;
    sf::Event event;
    // window event
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Resized) {
        printf("\033[1;36m[%s][%d] :x: Resize (%d,%d) \033[m\n",
            __FUNCTION__,__LINE__,
            event.size.width,
            event.size.height);
        vec2CurrentSize.x = event.size.width;
        vec2CurrentSize.y = event.size.height;
        float fScaleX =(float)((float)vec2CurrentSize.x/(float)vec2BaseSize.x);
        float fScaleY =(float)((float)vec2CurrentSize.y/(float)vec2BaseSize.y);
        printf("\033[1;36m[%s][%d] :x: Resize (%f,%f) \033[m\n",
            __FUNCTION__,__LINE__,
            fScaleX,
            fScaleY
            );
        // :x: init is needed!!!
        tfScale= sf::Transform::Identity; 
        tfScale.scale(fScaleX,fScaleY);

      }
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


    // key event in the focus 
    if (bWindowFocused == true ) {
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        bMousePressed = true;
      }

      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)== false && 
          bMousePressed == true) {
        bMousePressed = false;
        vec2iLocalPos = sf::Mouse::getPosition(window);
        sf::Vector2f vec2fMousePose = {(float)vec2iLocalPos.x,(float)vec2iLocalPos.y};
        printf("\033[1;36m[%s][%d] :x: click %f %f \033[m\n",
            __FUNCTION__,__LINE__,vec2fMousePose.x,vec2fMousePose.y);

        vec2fMousePose = tfScale.getInverse().transformPoint(vec2fMousePose);

        printf("\033[1;36m[%s][%d] :x: click- Scale %f %f \033[m\n",
            __FUNCTION__,__LINE__,vec2fMousePose.x,vec2fMousePose.y);


        sf::Transform tf ;
        tf = sf::Transform::Identity;

        sf::FloatRect fRect = pRectMenuBoundary->m_Entity.getGlobalBounds();
        printf("\033[1;33m[%s][%d] :x: Target Rect %f %f %f %f \033[m\n",
            __FUNCTION__,__LINE__,fRect.left,fRect.top,fRect.width,fRect.height);

        tf *=pRoot->m_Tf;
        tf *=pRectMenuBoundary->m_Tf;
        tf *=pRectTxtBorder->m_Tf;

        tf = tf.getInverse();
             
        sf::Vector2f point = tf.transformPoint(vec2fMousePose);
        printf("\033[1;36m[%s][%d] :x: Point %f %f \033[m\n",__FUNCTION__,__LINE__,point.x,point.y);
        sf::FloatRect bound = pRectTxtBorder->m_Entity.getGlobalBounds();
        bound = tfScale.transformRect(bound);
        if (pRectTxtBorder->m_Entity.getGlobalBounds().contains(point)) {
        //if (bound.contains(point)) {
          printf("\033[1;31m[%s][%d] :x: Hit \033[m\n",__FUNCTION__,__LINE__);

        }


        
      }

      // Quit Key ( Q )
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
      {
        printf("\033[1;36m[%s][%d] :x: Quit Key \033[m\n",__FUNCTION__,__LINE__);
        window.close();
      }
    }


    

    window.clear();
    pRoot->draw(window,sf::Transform::Identity);
    //pRootNode->draw(window,sf::Transform::Identity);
    //pTextTestStart->draw(window,sf::Transform::Identity);
    window.display();
  }


  return 0;
}
