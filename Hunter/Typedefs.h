#ifndef TYPE_DEFS_H
#define TYPE_DEFS_H

#include <stdint.h>
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef int32 bool32;

typedef uint32 SurfaceID;
typedef uint32 TextureID;
typedef uint32 EffectID;
typedef uint32 FontID;


typedef uint32 ComponentID;
typedef uint32 ComponentFamilyID;
typedef uint32 EntityID;
typedef uint32 SurfaceID;

typedef uint32 UIID;

typedef uint16 ResourceHandle;
typedef ResourceHandle VertexBufferHandle;
typedef ResourceHandle IndexBufferHandle;
typedef ResourceHandle EffectHandle;
typedef ResourceHandle TextureHandle;

#endif