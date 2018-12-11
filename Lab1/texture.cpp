#include <iostream>
#include "texture.h"
#include "SOIL/SOIL.h"

/**
* 构造函数
*  TextureTarget  纹理的目标 GL_TEXTURE_2D之类的
*  FileName 纹理图片的文件名
*/

Texture::Texture(GLenum TextureTarget, const std::string& FileName, GLenum TextureUnit, unsigned int textureUnitNumber_)
{
	textureTarget = TextureTarget;
	fileName = FileName;
	textureUnit = TextureUnit;
	textureUnitNumber = textureUnitNumber_;
	//m_pImage = NULL;
}

bool Texture::Load()
{
	//利用文件路径将图片读入一个Magick的Image对象,然后使用MagickImage的write将图片数据写入一个Magick的blob对象
	/*try {
		m_pImage = new Magick::Image(m_fileName);
		m_pImage->write(&m_blob, "RGBA");
	}
	catch (Magick::Error& Error) {
		std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
		return false;
	}*/
	glActiveTexture(textureUnit);

	int width, height,channels;
	unsigned char* image = SOIL_load_image(fileName.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

	glGenTextures(1, &textureObj);  //利用图片的blob产生纹理对象，数量为1

	glBindTexture(textureTarget, textureObj); //将纹理对象绑定至特定的纹理单元上

	//glTexImage2D(m_textureTarget, 0, GL_RGB, m_pImage->columns(), m_pImage->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data()); //加载该纹理对象的数据信息到纹理对象的2d目标上
	
	if (channels == 3)
	{
		glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	else if (channels == 4)
	{
		glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	
	glGenerateMipmap(textureTarget);

	//使用线性插值来做过滤，避免物体过大过小的时候，纹素变形
	glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glBindTexture(textureTarget, 0); //解除纹理对象和纹理目标之间的绑定
	return true;
}

//将当前的texture对象（内含纹理目标和纹理对象）绑定到特定的纹理单元上
void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(textureTarget, textureObj);
}

unsigned int Texture::getTextureUnitNumber()
{
	return textureUnitNumber;
}
