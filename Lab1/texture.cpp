#include <iostream>
#include "texture.h"
#include "SOIL/SOIL.h"

/**
* ���캯��
*  TextureTarget  �����Ŀ�� GL_TEXTURE_2D֮���
*  FileName ����ͼƬ���ļ���
*/

Texture::Texture(GLenum TextureTarget, const std::string& FileName)
{
	m_textureTarget = TextureTarget;
	m_fileName = FileName;
	//m_pImage = NULL;
}

bool Texture::Load()
{
	//�����ļ�·����ͼƬ����һ��Magick��Image����,Ȼ��ʹ��MagickImage��write��ͼƬ����д��һ��Magick��blob����
	/*try {
		m_pImage = new Magick::Image(m_fileName);
		m_pImage->write(&m_blob, "RGBA");
	}
	catch (Magick::Error& Error) {
		std::cout << "Error loading texture '" << m_fileName << "': " << Error.what() << std::endl;
		return false;
	}*/
	int width, height,channels;
	unsigned char* image = SOIL_load_image(m_fileName.c_str(), &width, &height, &channels, SOIL_LOAD_RGBA);

	glGenTextures(1, &m_textureObj);  //����ͼƬ��blob���������������Ϊ1

	glBindTexture(m_textureTarget, m_textureObj); //�������������ض�������Ԫ��

	//glTexImage2D(m_textureTarget, 0, GL_RGB, m_pImage->columns(), m_pImage->rows(), -0.5, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data()); //���ظ���������������Ϣ����������2dĿ����
	
	if (channels == 3)
	{
		glTexImage2D(m_textureTarget, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	else if (channels == 4)
	{
		glTexImage2D(m_textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	
	glGenerateMipmap(m_textureTarget);

	//ʹ�����Բ�ֵ�������ˣ�������������С��ʱ�����ر���
	glTexParameterf(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(m_textureTarget, 0); //���������������Ŀ��֮��İ�
	return true;
}

//����ǰ��texture�����ں�����Ŀ���������󣩰󶨵��ض�������Ԫ��
void Texture::Bind(GLenum TextureUnit)
{
	glActiveTexture(TextureUnit);
	glBindTexture(m_textureTarget, m_textureObj);
}
