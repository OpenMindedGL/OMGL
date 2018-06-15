#include "DynamicHeightMap.h"
#include "Debug.h"

void DynamicHeightMap::Upload(glm::i32vec2 offset, glm::i32vec2 size){
  GLCall(glTexSubImage2D(
        GL_TEXTURE_2D,          // target
        0,                       // level
        offset.x,          // offsets in the texture
        offset.y,
        size.x,          // size of the sub image
        size.y,
        GL_RGBA,          // GL_RGBA etc
        0,            // GL_UNSIGNED_BYTES etc
        (const GLvoid *) &offset));

}



