#ifndef BACKEND_DISPATCH_H
#define BACKEND_DISPATCH_H

typedef void(*BackendDispatchFunction)(const void *);

namespace backenddispatch
{
	void Draw(const void *pCommand);
	void DrawIndexed(const void *pCommand);
}

#endif