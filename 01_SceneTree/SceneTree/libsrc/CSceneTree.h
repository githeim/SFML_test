#pragma once
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

class CNullNode : public sf::Drawable 
{
  void draw(sf::RenderTarget& target, sf::RenderStates states) const {
  }
  int m_test;

};

class CSceneTree
{
  public:
    CSceneTree(std::string strID):m_strID(strID){
      printf("\033[1;36m[%s][%d] :x: Create[%s] \033[m\n",
          __FUNCTION__,__LINE__,m_strID.c_str());

    };

    virtual ~CSceneTree(){
       printf("\033[1;37m[%s][%d] :x: Destory[%s] \033[m\n",
          __FUNCTION__,__LINE__,m_strID.c_str());
       ClearChildNode();
       printf("\033[1;31m[%s][%d] :x: Destory Done[%s] \033[m\n",
          __FUNCTION__,__LINE__,m_strID.c_str());

    };

    // ... functions to transform the node

    // ... functions to manage the node's children

    void AddChildNode(CSceneTree* pChildNode) {
      m_pChildren.push_back(pChildNode);
    }

    
    /**
     * @brief recursively release heap memory in child nodes
     */
    void ClearChildNode() {
      for (CSceneTree* pItem : m_pChildren) {
        pItem->ClearChildNode();
        if ( pItem != nullptr) {
          delete pItem;
          pItem= nullptr;
        }
      }
      m_pChildren.clear();
    }

    void PrintNodeTree(int iLayerIdx) {
      int iCurIdx = iLayerIdx +1;
      std::string strNodeName;
      for (int i =0 ; i < iCurIdx ; i++ ) {
        strNodeName+="--)";
      }
      strNodeName+= m_strID;
      printf("\033[1;32m%s\033[m\n",strNodeName.c_str());
      for (auto child : m_pChildren) {
        child->PrintNodeTree(iCurIdx);
      }
    }


    void draw(sf::RenderTarget& target, const sf::Transform& parentTf) const
    {
      // combine the parent transform with the node's one
      sf::Transform combinedTransform = parentTf * m_Tf;

      // let the node draw itself
      onDraw(target, combinedTransform);

      // draw its children
      for (std::size_t i = 0; i < m_pChildren.size(); ++i)
        m_pChildren[i]->draw(target, combinedTransform);
    }
    void evt(sf::Event& evt,const sf::Transform& parentTf) {
      // combine the parent transform with the node's one
      sf::Transform combinedTf = parentTf * m_Tf;
      onEvt(evt,combinedTf);
      for (auto child : m_pChildren ) {
        child->evt(evt,combinedTf);
      }
    }

    sf::Transform m_Tf;
    std::vector<CSceneTree*> m_pChildren;
    std::string m_strID;
  private:

    virtual void onEvt(sf::Event& evt, sf::Transform& Tf)  =0;
    virtual void onDraw(sf::RenderTarget& target, const sf::Transform& Tf) const = 0;

};
template <typename T>
class CSceneNode : public CSceneTree
{

  public:
    CSceneNode() {};
    CSceneNode(std::string strID) : CSceneTree(strID){};

    virtual void onDraw(sf::RenderTarget& target, const sf::Transform& Tf) const
    {
      target.draw(m_Entity, Tf);
    }
    
    virtual void onEvt(sf::Event& evt, sf::Transform& Tf) {

    }


    // type T ; sf::RectangleShapeNode, sf::Text ...
    T m_Entity; 

};


class CButton : public CSceneTree
{
  public:
    CButton(std::string strID,std::string strText,
        sf::RenderWindow& window,sf::Font &font,int iBtnWidth = 100 ,
        int iFontSize = 20 ) 
      : CSceneTree(strID) {
        // :x: Create Text
        CSceneNode<sf::Text> *pText = 
          new CSceneNode<sf::Text>(strID+std::string("_TXT"));
        pText->m_Entity.setFont(font);
        pText->m_Entity.setString(strText);
        pText->m_Entity.setCharacterSize(iFontSize);
        pText->m_Entity.setFillColor(sf::Color::Green);
        pText->m_Tf.translate(0,0);
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
      }
    virtual void onDraw(sf::RenderTarget& target, const sf::Transform& Tf) const
    {

    }
    virtual void onEvt(sf::Event& evt, sf::Transform& Tf) {

      if (evt.type == sf::Event::MouseMoved) {
        sf::Transform tfLocal;
        tfLocal = sf::Transform::Identity;
        tfLocal = Tf.getInverse();

        sf::Vector2f vec2fMousePose = 
                               {(float)evt.mouseMove.x,(float)evt.mouseMove.y};
        sf::Vector2f vec2Localpoint = tfLocal.transformPoint(vec2fMousePose);

        CSceneNode<sf::RectangleShape>*pBorder=GetBorder();
        
        sf::FloatRect fRect = pBorder->m_Entity.getGlobalBounds();
        
        if (fRect.contains(vec2Localpoint)) {
          printf("\033[1;33m[%s][%d] :x: Touch ; ID [%s]\033[m\n",
                                        __FUNCTION__,__LINE__,m_strID.c_str());
          pBorder->m_Entity.setFillColor(sf::Color(0,80,0));
        }
        else {
          pBorder->m_Entity.setFillColor(sf::Color(0,40,0));
        }
      }

    }
    CSceneNode<sf::RectangleShape>* GetBorder() {
      return (CSceneNode<sf::RectangleShape>*)this->m_pChildren[0];
    }

    int m_iOutlineThickness = 1;

//    std::function<int(
//    [] (int first, int second) m_OnHover;
    
};
