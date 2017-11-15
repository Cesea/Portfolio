#ifndef VIDEO_DEFINES_H
#define VIDEO_DEFINES_H

#define VIDEO_CHECK( expression )  \
	{ \
		if (FAILED(expression)) \
		{ \
			Assert(false); \
		} \
	}

#define VIDEO_CHECK_TRACE(expression, text) \
	{ \
		if (FAILED(expression)) \
		{ \
			Console::Log(text); \
			Assert(false); \
		} \
	}
#define VIDEO_CONFIG_MAX_NUM_VERTEX_ELEMENTS 16
#define INVALID_INDEX  0xffff

#define VIDEO_CONFIG_VERTEXBUFFER_MAX_NUM 1024
#define VIDEO_CONFIG_INDEXBUFFER_MAX_NUM 1024
#define VIDEO_CONFIG_TEXTURE_MAX_NUM 512
#define VIDEO_CONFIG_EFFECT_MAX_NUM 128
#define VIDEO_CONFIG_DECL_MAX_NUM 128
#define VIDEO_CONFIG_RENDER_VIEW_MAX_NUM 8 
#define VIDEO_CONFIG_MATERIAL_MAX_NUM 256 
#define VIDEO_CONFIG_RENDER_GROUP_MAX_NUM 512

#define VIDEO_CONFIG_STATIC_XMESH_MAX_NUM 256
#define VIDEO_CONFIG_SKINNED_XMESH_MAX_NUM 256

#define VIDEO_CONFIG_RESOURCE_INDEX_BIT 16 
#define VIDEO_CONFIG_RESOURCE_COUNT_BIT 16 

#define VIDEO_CONFIG_MATERIAL_TEXTURE_MAX_NUM 8

#define VIDEO_CONFIG_MAX_MATRIX_CACHE 2048
#define VIDEO_CONFIG_MAX_COMMAND_BUFFER_SIZE (64 << 10)

#define VIDEO_TEXTURE0 0
#define VIDEO_TEXTURE1 1
#define VIDEO_TEXTURE2 2
#define VIDEO_TEXTURE3 3
#define VIDEO_TEXTURE4 4
#define VIDEO_TEXTURE5 5
#define VIDEO_TEXTURE6 6
#define VIDEO_TEXTURE7 7

#define VIDEO_TEXTURE_DIFFUSE (VIDEO_TEXTURE0 )
#define VIDEO_TEXTURE_NORMAL (VIDEO_TEXTURE2) 
#define VIDEO_TEXTURE_SPECULAR (VIDEO_TEXTURE1) 
#define VIDEO_TEXTURE_EMISSIVE (VIDEO_TEXTURE3)

#define VIDEO_TEXTURE_TERRAIN0 (VIDEO_TEXTURE0)
#define VIDEO_TEXTURE_TERRAIN1 (VIDEO_TEXTURE1)
#define VIDEO_TEXTURE_TERRAIN2 (VIDEO_TEXTURE2)
#define VIDEO_TEXTURE_TERRAIN_BLEND (VIDEO_TEXTURE3)

#endif