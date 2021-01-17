#include "CSceneTree.h"
#include <algorithm>

CSceneTree* g_pSceneTreeRoot = nullptr;


CSceneTree* GetSceneTreeRoot() {
  return g_pSceneTreeRoot;
}

void SetSceneTreeRoot(CSceneTree* pRoot) {
  g_pSceneTreeRoot = pRoot;
}

CSceneTree* g_pCurrentScene = nullptr;

CSceneTree* GetCurrentScene() {
  return g_pCurrentScene;
}

void SetCurrentScene(CSceneTree* pCurrentScene) {
  g_pCurrentScene = pCurrentScene;
}




CSceneTree::CSceneTree(std::string strID):m_strID(strID){
  // ID sholud not have ' '(space)
  // replace to underbar'_' from the space 
  std::replace(m_strID.begin(),m_strID.end(),' ','_');
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


int CSceneTree::AddChildNode(CSceneTree* pChildNode) {
  // check the duplication
  if ( FindNode(pChildNode->m_strID,this) != nullptr) {
    printf("\033[1;31m[%s][%d] :x: Err) Node ID = [%s] is duplicated "
        "Node ID should be unique\033[m\n",
        __FUNCTION__,__LINE__,pChildNode->m_strID.c_str());
    return -1;
  }
  m_pChildren.push_back(pChildNode);
  return 0;
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
  if (m_OnEvt != nullptr) 
    m_OnEvt(evt,combinedTf,fTimeDelta,this);
  for (auto child : m_pChildren ) {
    child->evt(evt,combinedTf,fTimeDelta);
  }
}
void CSceneTree::tick(const sf::Transform& parentTf,float& fTimeDelta) {
  // combine the parent transform with the node's one
  sf::Transform combinedTf = parentTf * m_Tf;
  onTick(combinedTf,fTimeDelta);
  if (m_OnTick != nullptr) 
    m_OnTick(combinedTf,fTimeDelta,this);
  for (auto child : m_pChildren ) {
    child->tick(combinedTf,fTimeDelta);
  }
}


CSceneTree* CSceneTree::FindNode(std::string strID,CSceneTree* pBaseNode) {
  CSceneTree* pNode;
  if (pBaseNode->m_strID == strID) {
    return pBaseNode;
  }
  for ( auto pChild : pBaseNode->m_pChildren) {
    pNode =pChild->FindNode(strID,pChild);
    if ( pNode != nullptr) 
      return pNode;
  }
  return nullptr;
}
