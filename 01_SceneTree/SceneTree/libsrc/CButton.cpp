#include "CButton.h"

CButton::CButton(std::string strID,std::string strText,
    sf::RenderWindow& window,sf::Font &font,int iBtnWidth ,
    int iFontSize ): CSceneTree(strID) {
  // :x: Create Text
  CSceneNode<sf::Text> *pText = 
    new CSceneNode<sf::Text>(strID+std::string("_TXT"));
  pText->m_Entity.setFont(font);
  pText->m_Entity.setString(strText);
  pText->m_Entity.setCharacterSize(iFontSize);
  pText->m_Entity.setFillColor(sf::Color::Green);
  sf::FloatRect txtRect = pText->m_Entity.getGlobalBounds();

  // :x: Create Border
  CSceneNode<sf::RectangleShape> *pBorder = 
    new CSceneNode<sf::RectangleShape>(strID+std::string("_BORDER"));
  float fTxtWidth =txtRect.width+(m_iOutlineThickness*2); 
  float fWidth =  fTxtWidth;
  if (fWidth < iBtnWidth ) {
    fWidth = iBtnWidth;
    pText->m_Tf.translate(fWidth/2.0-fTxtWidth/2.0,0);
  }
  float fHeight = txtRect.height+txtRect.top*2+(m_iOutlineThickness*2);
  pBorder->m_Entity.setSize(sf::Vector2f(fWidth, fHeight));
  pBorder->m_Entity.setFillColor(sf::Color(0,80,0));
  pBorder->m_Entity.setOutlineThickness(1.0f);
  pBorder->AddChildNode(pText);
  this->AddChildNode(pBorder);
  sf::FloatRect borderRect= pBorder->m_Entity.getGlobalBounds();
  borderRect= pBorder->m_Entity.getGlobalBounds();
}

bool CButton::IsHover(sf::Vector2f& vec2fMousePose,sf::Transform& Tf,
    CSceneNode<sf::RectangleShape>*pBorder) {
  sf::Transform tfLocal;                                               
  tfLocal = sf::Transform::Identity;                                   
  tfLocal = Tf.getInverse();                                           

  //sf::Vector2f vec2fMousePose = {(float)evt.mouseMove.x,(float)evt.mouseMove.y};                     
  sf::Vector2f vec2Localpoint = tfLocal.transformPoint(vec2fMousePose);
  sf::FloatRect fRect = pBorder->m_Entity.getGlobalBounds();           

  if (fRect.contains(vec2Localpoint)) {                                
    return true;                                                       
  }                                                                    
  return false;                                                        
}

void CButton::onEvt(sf::Event& evt, sf::Transform& Tf,float& fTimeDelta) {
  CSceneNode<sf::RectangleShape>*pBorder=GetBorder();
  sf::Vector2f vec2fMousePose;

  if (evt.type == sf::Event::MouseMoved) {
    vec2fMousePose = {(float)evt.mouseMove.x,(float)evt.mouseMove.y};                     
    // is The cursor in the border? 
    if (IsHover(vec2fMousePose,Tf,pBorder)) {


      if (m_bIsCursorOn == false) {
        pBorder->m_Entity.setFillColor(sf::Color(0,160,0));
      }
      m_bIsCursorOn = true;

    } else{
      if (m_bIsCursorOn == true) {
        pBorder->m_Entity.setFillColor(sf::Color(0,80,0));
      }
      m_bIsCursorOn = false;
    }
  }
  if (evt.type == sf::Event::MouseButtonPressed) {
    vec2fMousePose = {(float)evt.mouseButton.x,(float)evt.mouseButton.y};                     
    if (IsHover(vec2fMousePose,Tf,pBorder)) {
      if (m_OnClick != nullptr) {
          m_OnClick(m_pChildren,m_Tf,fTimeDelta);
      }
    }

  }

}


void CButton::onTick(const sf::Transform& Tf,float& fTimeDelta) {

}
