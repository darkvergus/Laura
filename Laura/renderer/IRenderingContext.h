#ifndef IRENDERING_CONTEXT_H
#define IRENDERING_CONTEXT_H

namespace Laura {

	class IRenderingContext
	{
	public:
		virtual void init() = 0;
		virtual void swapBuffers() = 0;
	};

}

#endif // IRENDERING_CONTEXT_H