#ifndef _CSCENETREE_H_
#define _CSCENETREE_H_ 

#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

class CNullNode : public sf::Drawable 
{
  void draw(sf::RenderTarget& target, sf::RenderStates states) const {
  }

};
class CSceneTree;
CSceneTree* GetSceneTreeRoot(); 
class CSceneTree
{
  public:
    CSceneTree(std::string strID);

    virtual ~CSceneTree();


    int AddChildNode(CSceneTree* pChildNode);

    /**
     * @brief recursively release heap memory in child nodes
     */
    void ClearChildNode();

    void PrintNodeTree(int iLayerIdx);

    CSceneTree* FindNode(std::string strID,
                         CSceneTree* baseNode= GetSceneTreeRoot());

    void draw(sf::RenderTarget& target, const sf::Transform& parentTf) const;
    
    void evt(sf::Event& evt,const sf::Transform& parentTf,float& fTimeDelta);
    sf::Transform m_Tf;
    std::vector<CSceneTree*> m_pChildren;
    std::string m_strID;
  private:

    virtual void onEvt(sf::Event& evt, sf::Transform& Tf,float& fTimeDelta)  =0;
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
    
    virtual void onEvt(sf::Event& evt, sf::Transform& Tf,float& fTimeDelta) {

    }

    // type T ; sf::RectangleShapeNode, sf::Text ...
    T m_Entity; 

};

void SetSceneTreeRoot(CSceneTree* pRoot); 



#endif /* ifndef _CSCENETREE_H_ */
