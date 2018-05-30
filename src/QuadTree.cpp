#include "QuadTree.h"
#include <stdio.h>
#include <stdlib.h>

QuadTree::QuadTree(unsigned int w) :
  m_Root(NodeType::Root, NULL, 0, 0, WIDTH, glm::i32vec2(0,0)),
  m_Width(w),
  m_Depth(std::log2(w))
{
  if(w%2 != 0){
    printf("[ERROR] Tried to make an odd sized QuadTree\n\
            [ERROR] Exiting before it explodes\n\
        ");
    exit(69);
  }
}

Node::Node(NodeType t, Node* p, unsigned int i, unsigned int d, unsigned int s, glm::i32vec2 pos) :
  m_Type(t),
  m_Parent(p),
  m_Index(i),
  m_Depth(d),
  m_Size(s),
  m_Position(pos)
{
  AddVertices();
  AddChildren();
  AddNeighbors();
}

void Node::AddMiddleVertices(){
      m_Vertices[Bottom] = new NodeVertex(m_Index+m_Size/2,false);
      m_Vertices[Left] = new NodeVertex(m_Index+(m_Size/2)*(m_Size+1),false);
      m_Vertices[Right] = new NodeVertex(m_Index+(m_Size/2) * (m_Size+3),false);
      m_Vertices[Top] = new NodeVertex(m_Index+(m_Size)*(m_Size+1)+m_Size/2,false);
      m_Vertices[Center] = new NodeVertex(m_Index+(m_Size/2) * ((m_Size+2)/(m_Depth+1),false);

}
void RootNode::AddVertices(){
      m_Vertices[Center] = new NodeVertex((WIDTH/2) * (WIDTH+2),false);
      m_Vertices[BottomLeft] = new NodeVertex(0,true);
      m_Vertices[Bottom] = new NodeVertex(WIDTH/2,false);
      m_Vertices[BottomRight] = new NodeVertex(WIDTH+1,true);
      m_Vertices[Right] = new NodeVertex((WIDTH/2) * (WIDTH+3),false);
      m_Vertices[TopRight] = new NodeVertex((WIDTH+1)*(WIDTH+1)-1,true);
      m_Vertices[Top] = new NodeVertex((WIDTH)*(WIDTH+1)+WIDTH/2,false);
      m_Vertices[TopLeft] = new NodeVertex((WIDTH)*(WIDTH+1),true);
      m_Vertices[Left] = new NodeVertex((WIDTH/2)*(WIDTH+1),false);
}

void BottomLeftNode::AddVertices(){
      m_Vertices[Center] = new NodeVertex(m_Index+(m_Size/2) * (m_Size+2),false);
      m_Vertices[BottomLeft] = m_Parent->m_Vertices[BottomLeft];
      m_Vertices[Bottom] = new NodeVertex(m_Index+(m_Size/2),false);
      m_Vertices[BottomRight] = m_Parent->m_Vertices[BottomRight];
      m_Vertices[Right] = new NodeVertex((WIDTH/2) * (WIDTH+3),false);
      m_Vertices[TopRight] = new NodeVertex((WIDTH+1)*(WIDTH+1)-1,true);
      m_Vertices[Top] = new NodeVertex((WIDTH)*(WIDTH+1)+WIDTH/2,false);
      m_Vertices[TopLeft] = new NodeVertex((WIDTH)*(WIDTH+1),true);
      m_Vertices[Left] = new NodeVertex((WIDTH/2)*(WIDTH+1),false);
      
      break;
    case NodeType::BottomRight:

      break;
    case NodeType::TopRight:

      break;
    case NodeType::TopLeft:

      break;
  
  }

}
