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

	glBindBuffer(GL_ARRAY_BUFFER, VBO);	//传入VBO（顶点缓冲对象）

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);	//顶点的空间坐标

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	//传入IBO（索引缓冲对象）

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);	//顶点的纹理坐标

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20); //顶点的法向量

	glDrawArrays(GL_TRIANGLES, 0, objLoader.getAllVertexBuffer().size());

	//在不使用顶点和法向量还有纹理的时候，短时间禁用他们会比较好
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
