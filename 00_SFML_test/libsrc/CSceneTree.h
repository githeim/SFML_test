#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>


class CSceneTree
{
  public:
    CSceneTree(std::string strID):m_strID(strID){};
    virtual ~CSceneTree(){};

    // ... functions to transform the node

    // ... functions to manage the node's children

    void AddChildNode(CSceneTree* pChildNode) {
      m_pChildren.push_back(pChildNode);
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

    sf::Transform m_Tf;
  private:

    virtual void onDraw(sf::RenderTarget& target, const sf::Transform& Tf) const = 0;

    std::vector<CSceneTree*> m_pChildren;
    std::string m_strID;
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


    // type T ; sf::RectangleShapeNode, sf::Text ...
    T m_Entity; 
};
