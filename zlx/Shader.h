#pragma once

#include "zlx.h"
#include "Utils.h"
#include "Font.h"
// Todo
namespace ZLX {
	class Shader
	{
	public:
		// Vertex
		ZLXVertexShader * loadVertexShader(const char * file);
		ZLXVertexShader * loadVertexShader(unsigned char * memory);

		// Pixel
		ZLXPixelShader * loadPixelShader(const char * file);
		ZLXPixelShader * loadPixelShader(unsigned char * memory);
		
		// Free memory
		void freeShader(ZLXPixelShader * shader);
#ifndef LIBXENON
		void freeShader(ZLXVertexShader * shader);
#endif
	};
}
