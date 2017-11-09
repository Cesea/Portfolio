#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

//struct RasterizerState
//{
//
//};

//struct DepthState
//{
//};

struct RenderContext
{
	//EffectHandle _effectHandle;
	//SurfaceMaterialHandle _surfaceMaterialHandle;
	//VertexDeclHandle _vertexDeclHandle;
	//VertexBufferHandle _vertexBufferHandle;
	//IndexBufferHandle _indexBufferHandle;

	Matrix _viewMatrix;
	Matrix _projectionMatrix;
	//CommandBucket bucket;
};

#endif