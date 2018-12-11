#pragma once
#ifndef TEXTURE_H
#define	TEXTURE_H

#include <string>

#include <GL/glew.h>
//#include <Magick++/Magick++.h>

class Texture
{
public:
	Texture(GLenum TextureTarget, const std::string& FileName, GLenum TextureUnit);

	bool Load();

	void Bind(GLenum TextureUnit);

private:
	std::string fileName;
	GLenum textureTarget; //����Ŀ��
	GLuint textureObj; //�������
	GLenum textureUnit; //����Ԫ
	//Magick::Image* m_pImage;
	//Magick::Blob m_blob;
};


#endif	/* TEXTURE_H */