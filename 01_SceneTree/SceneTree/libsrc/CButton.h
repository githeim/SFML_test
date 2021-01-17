#ifndef _CBUTTON_H_
#define _CBUTTON_H_
#include "CSceneTree.h"
class CButton : public CSceneTree
{
  public:

    CButton(std::string strID,std::string strText,
        sf::RenderWindow& window,sf::Font &font,int iBtnWidth = 100 ,
        int iFontSize = 20 ) ;

    virtual void onDraw(sf::RenderTarget& target, const sf::Transform& Tf) const
    { 
    }

    bool IsHover(sf::Vector2f& vec2fMousePose,sf::Transform& Tf,
                 CSceneNode<sf::RectangleShape>*pBorder);

    virtual void onEvt(sf::Event& evt, sf::Transform& Tf,float& fTimeDelta);
    void onTick(const sf::Transform& Tf,float& fTimeDelta);

    /**
     * @brief Get border of the button 
     *
     * @return border instance pointer
     */
    CSceneNode<sf::RectangleShape>* GetBorder() {
      return (CSceneNode<sf::RectangleShape>*)this->m_pChildren[0];
    }

    // the flag , Is the curson on the button?
    bool m_bIsCursorOn = false;
    int m_iOutlineThickness = 1;
   
    std::function <int(std::vector<CSceneTree*>&, sf::Transform&, float& )> 
                                                            m_OnClick = nullptr;

};


#endif // :x: end _CBUTTON_H_
