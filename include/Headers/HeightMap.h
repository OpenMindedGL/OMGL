#ifndef Map_H
#define Map_H

#include <glm/glm.hpp>
#include "Texture.h"
#include "Material.h"

class Map : public Texture {

  private :
    unsigned int channels;
    float m_Precision;
    int m_Size;
    int m_HalfSize;
    Object lods_obj[NB_LEVELS];   // Future Objects
    LODLevel * lods[NB_LEVELS];
    Material * m_Material;


  public :
    
    NoiseGen m_Noise;
    Map(glm::vec2 spawn, float p = PRECISION, unsigned int s = SIZE, unsigned int n = NB_LEVELS);
    inline int GetSize(){ return m_Size; }
    inline float GetPrecision(){ return m_Precision; }
    inline unsigned int GetNbLevel(){ return m_NbLevels; }
    inline Object& GetLevel(unsigned int i){ return lods_obj[i]; }
    void Update(glm::i32vec2& center);


};



#endif

