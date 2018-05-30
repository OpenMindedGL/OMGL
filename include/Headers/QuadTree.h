#ifndef QUADTREE_H
#define QUADTREE_H

#include <glm/glm.hpp>

#define WIDTH   256

enum struct NodeType {
  Root = 0,
  BottomLeft = 1,
  BottomRight = 2,
  TopRight = 3,
  TopLeft = 4
};

enum NodeVertexType {
  Center = 0,
  BottomLeft = 1,
  Bottom = 2,
  BottomRight = 3,
  Right = 4,
  TopRight = 5,
  Top = 6,
  TopLeft = 7,
  Left = 8
};


class NodeVertex{
  public:
    unsigned int m_Index;
    bool m_IsOn;
    inline NodeVertex(unsigned int i, bool io) : m_Index(i), m_IsOn(io) {};
};

class Node{
  
  public:
  
    Node* m_Parent;
    
    Node* m_Children[4];        // 0 is BottomLeft counter cw
    Node* m_Neighbors[4];       // 0 is Bottom counter cw
    NodeVertex* m_Vertices[9];

    // position of bottom left vertex
    glm::i32vec2& m_Position;    

    // index of bottom left vertex
    unsigned int m_Index; 

    unsigned int m_Depth;

    unsigned int m_Size;

    Node(NodeType t, Node* p, unsigned int i, unsigned int d, unsigned int s, glm::i32vec2 pos);

    void AddChildren();

    void AddNeighbors();

    void AddVertices();
    void AddMiddleVertices();
};

class RootNode : public Node {
    void AddVertices();
}

class BottomLeftNode : public Node {
    void AddVertices();
}

class BottomRightNode : public Node {
    void AddVertices();
}

class TopRightNode : public Node {
    void AddVertices();
}

class TopLeftNode : public Node {
    void AddVertices();
}

class QuadTree
{

  public:

    Node m_Root;
    unsigned int m_Width; // in quads not points (+1)
    unsigned int m_Depth; // root is depth 0

    QuadTree(unsigned int w);
};

#endif
