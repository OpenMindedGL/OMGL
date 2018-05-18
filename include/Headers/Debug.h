#ifndef DEBUG_H
#define DEBUG_H

#define ASSERT(x) if (!(x)) __builtin_trap()
#define GLCall(x) GLClearError();\
  x;\
  ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#endif
