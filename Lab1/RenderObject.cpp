#include "RenderObject.h"

RenderObject::RenderObject(const char * vshader, const char * fshader)
{
	pVSFileName = string(vshader);
	pFSFileName = string(fshader);
}

void RenderObject::render()
{
	glUseProgram(ShaderProgram);

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)(objectTransformController.GetWorldTrans()->matrix));
	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)(objectTransformController.GetCameraTrans()->matrix));
	glUniform3f(gEyePosition, camera->GetPos().x, camera->GetPos().y, camera->GetPos().z);

	glUniform1i(gSampler, Texture->getTextureUnitNumber());

	glBindBuffer(GL_ARRAY_BUFFER, VBO);	//����VBO�����㻺�����

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//����Ŀռ�����

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//����IBO�������������

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//�������������

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //����ķ�����

	glDrawArrays(GL_TRIANGLES, 0, objLoader.getAllVertexBuffer().size());

	//�ڲ�ʹ�ö���ͷ��������������ʱ�򣬶�ʱ��������ǻ�ȽϺ�
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
