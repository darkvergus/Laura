#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "lrpch.h"

class ITexture
{
public:
	static std::shared_ptr<ITexture> Create(const std::string& filepath, int bindingPoint);
	static std::shared_ptr<ITexture> Create(int width, int height, int channels, int bindingPoint);

	virtual ~ITexture() {}

	virtual void Bind() = 0;
	virtual void Unbind() = 0;
	virtual void ChangeBindingPoint(int bindingPoint) = 0;

	/// GETTERS ///
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual int GetID() = 0;
};

#endif // ITEXTURE_H