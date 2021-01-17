#ifndef _CACTOR_H_
#define _CACTOR_H_ 

#include "CSceneTree.h"
#include "CSteeringBehavior.h"
class CActor : public CSceneTree {
  public:
  CActor(std::string strID):CSceneTree(strID) {
    // Create Body
    CSceneNode<sf::RectangleShape>* pBody;  

    pBody = new CSceneNode<sf::RectangleShape>(strID+"CENTER_BODY");
    sf::Vector2f vecSize =  {20.0f,10.0f};
    pBody->m_Entity.setSize(vecSize);
    pBody->m_Entity.setFillColor(sf::Color(0,0,180));
    pBody->m_Entity.setOutlineThickness(1.0f);
    pBody->m_Entity.setPosition(-(vecSize.x/2.0f),-(vecSize.y/2.0f));
    AddChildNode(pBody);

    pBody = 
      new CSceneNode<sf::RectangleShape>(strID+"LEFT_BODY");
    pBody->m_Entity.setSize(vecSize);
    pBody->m_Entity.setFillColor(sf::Color(0,80,0));
    pBody->m_Entity.setOutlineThickness(1.0f);
    pBody->m_Entity.setPosition(-(vecSize.x/2.0f)*2,-(vecSize.y)*1.5-1);
    AddChildNode(pBody);

    pBody = 
      new CSceneNode<sf::RectangleShape>(strID+"RIGHT_BODY");
    pBody->m_Entity.setSize(vecSize);
    pBody->m_Entity.setFillColor(sf::Color(0,80,0));
    pBody->m_Entity.setOutlineThickness(1.0f);
    pBody->m_Entity.setPosition(-(vecSize.x/2.0f)*2,(vecSize.y)*0.5+1);
    AddChildNode(pBody);




    m_vec2Pos = {300,400};

    m_Tf.translate(m_vec2Pos.x,m_vec2Pos.y);
#if 1 // :x: for test
    printf("\033[1;33m[%s][%d] :x: Matrix \033[m\n",__FUNCTION__,__LINE__);
    const float* pMat = m_Tf.getMatrix();
    for (int i =0 ; i< 4 ; i++) {
    printf("\033[1;36m[%s][%d] :x: %f %f %f %f \033[m\n",
        __FUNCTION__,__LINE__,pMat[0+4*i],pMat[1+4*i],pMat[2+4*i],pMat[3+4*i]);
    }
#endif // :x: for test
    m_Tf.rotate(90);
#if 1 // :x: for test
    printf("\033[1;33m[%s][%d] :x: Matrix \033[m\n",__FUNCTION__,__LINE__);
    pMat = m_Tf.getMatrix();
    for (int i =0 ; i< 4 ; i++) {
    printf("\033[1;36m[%s][%d] :x: %f %f %f %f \033[m\n",
        __FUNCTION__,__LINE__,pMat[0+4*i],pMat[1+4*i],pMat[2+4*i],pMat[3+4*i]);
    }
#endif // :x: for test
    m_Tf.rotate(-90);


    sf::Vector2f point = m_Tf.transformPoint(0,0);
    printf("\033[1;36m[%s][%d] :x: point ( %f %f ) \033[m\n",
        __FUNCTION__,__LINE__,point.x,point.y);


    m_vec2Target.x= 300;
    m_vec2Target.y= 400;
  }
  void onEvt(sf::Event& evt, sf::Transform& Tf,float& fTimeDelta) {
    if (evt.type == sf::Event::MouseButtonPressed)
    {
      if (evt.mouseButton.button == sf::Mouse::Right)
      {
        printf("\033[1;36m[%s][%d] :x: %d %d \033[m\n",__FUNCTION__,__LINE__,evt.mouseButton.x,evt.mouseButton.y);
        m_vec2Target.x = evt.mouseButton.x;
        m_vec2Target.y = evt.mouseButton.y;
      }
    }
    if (evt.type == sf::Event::KeyReleased)
    {
      if (evt.key.code == sf::Keyboard::G)
      {
        if (m_bFlag_Seek == true) {
          m_bFlag_Seek = false;
          m_vec2Velocity = {0,0};
        } else
          m_bFlag_Seek = true;
      }


    }

  }
  void onTick(const sf::Transform& Tf,float& fTimeDelta)  {
    static int iCnt=0;
    iCnt++;
    static float fTvel = 0.0f;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      m_fAngle-=50*fTimeDelta;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      m_fAngle+=50*fTimeDelta;
    }



    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      //m_Tf.translate(50*fTimeDelta,0);
      fTvel+=5.0;
      m_vec2Velocity = { fTvel*fTimeDelta,0};
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      //m_Tf.translate(50*fTimeDelta,0);
      fTvel=0;
      m_vec2Velocity = { fTvel*fTimeDelta,0};
    }

    

    m_vec2Pos = m_Tf.transformPoint(0,0);
    if (m_bFlag_Seek == true) {
      m_vec2SteeringForce = CSteeringBehavior::Seek(
          m_vec2Pos,m_vec2Target,m_vec2Velocity,m_fMax_velocity,m_fMax_force);
      m_vec2Acceleration = m_vec2SteeringForce/m_fMass;
      m_vec2Velocity = m_vec2Velocity + m_vec2Acceleration;

      m_fAngle = atan2(m_vec2Velocity.x, m_vec2Velocity.y)*180/M_PI-90;
    }
    m_Tf = sf::Transform::Identity;
    
    if (iCnt%6 ==0 ) {
      printf("\033[1;33m[%s][%d] :x: vel (%f %f) %f \033[m\n",
          __FUNCTION__,__LINE__,m_vec2Velocity.x,m_vec2Velocity.y,m_fAngle);
    }
    m_Tf.translate(m_vec2Pos+m_vec2Velocity);
    m_Tf.rotate(m_fAngle);



  }

  void onDraw(sf::RenderTarget& target, const sf::Transform& Tf) const {

  }
 
  sf::Vector2f m_vec2Target;
  
  bool m_bFlag_Seek =false;
	float m_fOrientation;
	float m_fMax_force= 5.0;
	float m_fMax_velocity=5.0;
  float m_fMass=50.0;
  float m_fAngle=0;

  sf::Vector2f m_vec2Velocity = {0,0};
  sf::Vector2f m_vec2SteeringForce = {0,0};
  sf::Vector2f m_vec2Acceleration = {0,0};
  sf::Vector2f m_vec2Pos = {0,0};


};
#endif /* ifndef _CACTOR_H_ */
