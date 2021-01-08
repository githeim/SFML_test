#include "CSceneTree.h"
CSceneTree::CSceneTree(std::string strID):m_strID(strID){
  printf("\033[1;36m[%s][%d] :x: Create[%s] \033[m\n",
      __FUNCTION__,__LINE__,m_strID.c_str());

}


CSceneTree::~CSceneTree(){
  printf("\033[1;37m[%s][%d] :x: Destroy[%s] \033[m\n",
      __FUNCTION__,__LINE__,m_strID.c_str());
  ClearChildNode();
  printf("\033[1;31m[%s][%d] :x: Destroy Done[%s] \033[m\n",
      __FUNCTION__,__LINE__,m_strID.c_str());
}


void CSceneTree::AddChildNode(CSceneTree* pChildNode) {
  m_pChildren.push_back(pChildNode);
}

/**
 * @brief recursively release heap memory in child nodes
 */
void CSceneTree::ClearChildNode() {
  for (CSceneTree* pItem : m_pChildren) {
    pItem->ClearChildNode();
    if ( pItem != nullptr) {
      delete pItem;
      pItem= nullptr;
    }
  }
  m_pChildren.clear();
}



void CSceneTree::PrintNodeTree(int iLayerIdx) {
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


void CSceneTree::draw(sf::RenderTarget& target, const sf::Transform& parentTf) const
{
  // combine the parent transform with the node's one
  sf::Transform combinedTransform = parentTf * m_Tf;

  // let the node draw itself
  onDraw(target, combinedTransform);

  // draw its children
  for (std::size_t i = 0; i < m_pChildren.size(); ++i)
    m_pChildren[i]->draw(target, combinedTransform);
}
void CSceneTree::evt(sf::Event& evt,const sf::Transform& parentTf,
    float& fTimeDelta) {
  // combine the parent transform with the node's one
  sf::Transform combinedTf = parentTf * m_Tf;
  onEvt(evt,combinedTf,fTimeDelta);
  for (auto child : m_pChildren ) {
    child->evt(evt,combinedTf,fTimeDelta);
  }
}



