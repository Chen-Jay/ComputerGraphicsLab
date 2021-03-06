#pragma once
#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
#include <Magick++/Magick++.h>

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName);

	bool Load();

	void Bind(GLenum TextureUnit);

private:
	std::string m_fileName;
	GLenum m_textureTarget; //纹理单元
	GLuint m_textureObj; //纹理对象
	Magick::Image* m_pImage;
	Magick::Blob m_blob;
};


#endif	/* TEXTURE_H */