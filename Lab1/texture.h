#pragma once
#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
//#include <Magick++/Magick++.h>

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName, GLenum TextureUnit, unsigned int textureUnitNumber);

	bool Load();

	void Bind(GLenum TextureUnit);
	
	unsigned int getTextureUnitNumber();
private:
	std::string fileName;
	GLenum textureTarget; //纹理目标
	GLuint textureObj; //纹理对象
	GLenum textureUnit; //纹理单元
	unsigned int textureUnitNumber;
	//Magick::Image* m_pImage;
	//Magick::Blob m_blob;
};


#endif	/* TEXTURE_H */