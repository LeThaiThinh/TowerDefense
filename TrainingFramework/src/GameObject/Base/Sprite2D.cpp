#include "Sprite2D.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"
#include "Application.h"

Sprite2D::Sprite2D(GLint id, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
	: BaseObject(id, model, shader, texture), m_iWidth(100), m_iHeight(50), m_vboId(0)
{
	Init();
}

Sprite2D::Sprite2D(GLint id, std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, Vector4 color)
	: BaseObject(id, model, shader, color), m_iWidth(100), m_iHeight(50), m_vboId(0)
{
	Init();
}

Sprite2D::Sprite2D(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
	: BaseObject(-1, model, shader, texture), m_iWidth(100), m_iHeight(50), m_vboId(0)
{
	Init();
}

Sprite2D::Sprite2D(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, int iwidth, int iheight)
	: BaseObject(-1, model, shader, texture), m_iWidth(iwidth), m_iHeight(iheight), m_vboId(0)
{
	SetISize(iwidth, iheight);
	Init();
}

Sprite2D::Sprite2D(std::shared_ptr<Model> model, std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture, float x, float y, int iwidth, int iheight, int width, int height)
	: BaseObject(-1, model, shader, texture), m_iWidth(width), m_iHeight(height), m_vboId(0), m_width(width), m_height(height)
{
	Init();
	Set2DPosition(x, y);
	SetISize(iwidth, iheight);
	SetSize(width, height);
}

Sprite2D::~Sprite2D()
{
}

void Sprite2D::Init()
{
	SetCamera(Application::GetInstance()->GetCamera());
	CalculateWorldMatrix();
}

void Sprite2D::Draw()
{
	if (m_pCamera == nullptr) return;
	glUseProgram(m_pShader->m_program);
	glBindBuffer(GL_ARRAY_BUFFER, m_pModel->GetVertexObject());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pModel->GetIndiceObject());

	GLuint iTempShaderVaribleGLID = -1;
	Matrix wvpMatrix;

	wvpMatrix = m_worldMatrix * m_pCamera->GetLookAtCameraMatrix();

	if (m_pTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pTexture->Get2DTextureAdd());
		if (m_pShader->m_iTextureLoc[0] != -1)
			glUniform1i(m_pShader->m_iTextureLoc[0], 0);
	}
	else
	{
		iTempShaderVaribleGLID = -1;
		iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_color");
		if (iTempShaderVaribleGLID != -1)
			glUniform4f(iTempShaderVaribleGLID, m_color.x, m_color.y, m_color.z, m_color.w);
	}


	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetAttribLocation((char*)"a_posL");
	if (iTempShaderVaribleGLID != -1)
	{
		glEnableVertexAttribArray(iTempShaderVaribleGLID);
		glVertexAttribPointer(iTempShaderVaribleGLID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), VETEX_POSITION);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetAttribLocation((char*)"a_uv");
	if (iTempShaderVaribleGLID != -1)
	{
		glEnableVertexAttribArray(iTempShaderVaribleGLID);
		glVertexAttribPointer(iTempShaderVaribleGLID, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), VETEX_UV);
	}

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_alpha");
	if (iTempShaderVaribleGLID != -1)
		glUniform1f(iTempShaderVaribleGLID, 1.0);

	iTempShaderVaribleGLID = -1;
	iTempShaderVaribleGLID = m_pShader->GetUniformLocation((char*)"u_wvpMatrix");
	if (iTempShaderVaribleGLID != -1)
		glUniformMatrix4fv(iTempShaderVaribleGLID, 1, GL_FALSE, wvpMatrix.m[0]);



	glDrawElements(GL_TRIANGLES, m_pModel->GetNumIndiceObject(), GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Sprite2D::Update(GLfloat deltatime)
{
}

void Sprite2D::Set2DPosition(GLfloat x, GLfloat y)
{
	m_position = Vector3(x, y, 0.0f);
	m_centerPosition = Vector3(x - m_iWidth / 2 + m_width / 2, y + m_iHeight / 2 - m_height / 2, 0.0f);
	CalculateWorldMatrix();
}
void Sprite2D::Set3DPosition(GLfloat x, GLfloat y, GLfloat z)
{
	m_position = Vector3(x, y, z);
	m_centerPosition = Vector3(x , y , 0);
	CalculateWorldMatrix();
}
void Sprite2D::Set2DStaticPosition(GLfloat x, GLfloat y)
{
	m_static_x = x;
	m_static_y = y;
	m_position = Vector3(m_static_x + Application::GetInstance()->GetCamera()->GetPosition().x, m_static_y + Application::GetInstance()->GetCamera()->GetPosition().y, 0.0f);
	m_centerPosition = Vector3(m_static_x - m_iWidth / 2 + m_width / 2 + Application::GetInstance()->GetCamera()->GetPosition().x, m_static_y + m_iHeight / 2 - m_height / 2 + Application::GetInstance()->GetCamera()->GetPosition().y, 0.0f);
	CalculateWorldMatrix();
}
void Sprite2D::Set2DStaticPosition()
{
	m_position = Vector3(m_static_x + Application::GetInstance()->GetCamera()->GetPosition().x, m_static_y + Application::GetInstance()->GetCamera()->GetPosition().y, 0.0f);
	m_centerPosition = Vector3(m_static_x - m_iWidth / 2 + m_width / 2 + Application::GetInstance()->GetCamera()->GetPosition().x, m_static_y + m_iHeight / 2 - m_height / 2 + Application::GetInstance()->GetCamera()->GetPosition().y, 0.0f);
	CalculateWorldMatrix();
}
void Sprite2D::Set2DPosition(Vector2 position)
{
	m_position = Vector3(position.x, position.y, 0.0f);
	m_centerPosition = Vector3(m_position.x - m_iWidth / 2 + m_width / 2, m_position.y + m_iHeight / 2 - m_height / 2, 0.0f);
	CalculateWorldMatrix();
}

void Sprite2D::SetISize(GLint width, GLint height)
{
	m_iWidth = width;
	m_iHeight = height;
	m_scale = Vector3((GLfloat)m_iWidth, (GLfloat)m_iHeight, 1.0f);
	CalculateWorldMatrix();
}

void Sprite2D::SetSize(GLint width, GLint height)
{
	m_width = width;
	m_height = height;
	m_centerPosition = m_position; //Vector3(m_position.x - m_iWidth / 2 + m_width/2, m_position.y + m_iHeight / 2 - m_height/2, 0);//
}
