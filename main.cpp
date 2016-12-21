/*
	Name		: Vatsal Gopani (N17368916)
	NetID		: vbg221
	Description	: Assignment 4
				  I have applied gaussian blur effect and then applied Gama Correction on the Object.

*/


#include "glsupport.h"
#include <glut.h>
#include "matrix4.h"
#include <iostream>
#include "quat.h"
#include <vector>
#include "geometrymaker.h"
#include <string>
#include <cassert>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;

GLint program;
GLuint vertPostionVBO;
GLuint normalPositionVBO;
GLuint positionAttribute;

GLuint texCoordAttribute;

GLuint vertColorVBO;
GLuint colorAttribute;

GLuint modelViewMatrixUniformLocation;
GLuint projectionMatrixUniformLocation;
GLuint positionUniform;

GLuint normalAttribute;
GLuint normalMatrixUniformLocation;


GLuint vertexBO1, vertexBO2, vertexBO3, vertexBO4;
GLuint indexBO1, indexBO2, indexBO3, indexBO4;

GLuint diffuseTexture;
GLuint diffuseTextureUniformLocation;

GLuint frameBuffer;
GLuint frameBufferTexture;
GLuint depthBufferTexture;
//FrameBuffer2
GLuint frameBuffer2;
GLuint frameBufferTexture2;
GLuint depthBufferTexture2;
//FrameBuffer3
GLuint frameBuffer3;
GLuint frameBufferTexture3;
GLuint depthBufferTexture3;


// 2nd Program
GLint screenTrianglesProgram;
GLuint screenTrianglesPositionAttribute;
GLuint screenTrianglesPositionBuffer;
GLuint screenTrianglesTexCoordAttribute;
GLuint screenTrianglesUVBuffer;
GLuint screenFramebufferUniform;

// 3rd Program
GLint screenTrianglesProgram2;
GLuint screenTrianglesPositionAttribute2;
GLuint screenTrianglesPositionBuffer2;
GLuint screenTrianglesTexCoordAttribute2;
GLuint screenTrianglesUVBuffer2;
GLuint screenFramebufferUniform2;

// 4th Program
GLint screenTrianglesProgram3;
GLuint screenTrianglesPositionAttribute3;
GLuint screenTrianglesPositionBuffer3;
GLuint screenTrianglesTexCoordAttribute3;
GLuint screenTrianglesUVBuffer3;
GLuint screenFramebufferUniform3;


int timeStart;
typedef struct Entity Entity;



struct VertexPN {
	Cvec3f p;
	Cvec3f n;
	Cvec2f t;
	
	VertexPN() {}
	VertexPN(float x, float y, float z, float nx, float ny, float nz) : p(x, y, z), n(nx, ny, nz) {}

	VertexPN& operator = (const GenericVertex& v) {
		p = v.pos;
		n = v.normal;
		t = v.tex;
		return *this;
	}
};

struct Geometry {
	int numIndices;
	void Draw(string type) {
	if (type == "Sphere") {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO1);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
		glEnableVertexAttribArray(positionAttribute);

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
		glEnableVertexAttribArray(normalAttribute);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO1);
	}
	if (type == "Plane") {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO2);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
		glEnableVertexAttribArray(positionAttribute);

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
		glEnableVertexAttribArray(normalAttribute);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO2);
	}
	if (type == "Cube") {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO3);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
		glEnableVertexAttribArray(positionAttribute);

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
		glEnableVertexAttribArray(normalAttribute);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO3);

	}
	if (type == "Object3D") {
		
		
		glUniform1i(diffuseTextureUniformLocation, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexBO4);
		glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, p));
		glEnableVertexAttribArray(positionAttribute);

		glEnableVertexAttribArray(texCoordAttribute);
		glVertexAttribPointer(texCoordAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, t));

		glVertexAttribPointer(normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, n));
		glEnableVertexAttribArray(normalAttribute);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO4);

	}
		
	}
};

class Entity {
private:
	Quat q1, q2, q3;
	Quat combined;

public:
	Geometry geometry;
	Cvec3 t;
	Cvec3 r;
	Cvec3 s;

	Entity *parent;
	Matrix4 modelMatrix;

	Entity() {
		s = (1.0, 1.0, 1.0);
	}


	void createMatrix() {

		q1 = Quat::makeXRotation(r[0]);
		q2 = Quat::makeYRotation(r[1]);
		q3 = Quat::makeZRotation(r[2]);

		combined = q1 * q2 * q3;

		Matrix4 tempRMatrix;
		tempRMatrix = quatToMatrix(combined);

		Matrix4 tempTMatrix;
		tempTMatrix = tempTMatrix.makeTranslation(t);

		Matrix4 tempSMatrix;
		tempSMatrix = tempSMatrix.makeScale(s);
		if (this->parent == NULL) {
			modelMatrix = tempTMatrix * tempRMatrix * tempSMatrix;
		}
		else {
			modelMatrix = parent->modelMatrix * tempTMatrix * tempRMatrix * tempSMatrix;
		}
	}
	
	void Draw(Matrix4 eyeMatrix, Matrix4 projectionMatrix, GLuint modelViewMatrixUniformLocation, GLuint projectionMatrixUniformLocation, GLuint normalMatrixUniformLocation, string type)
	{
		createMatrix();
		
		Matrix4 modelViewMatrix = inv(eyeMatrix) * modelMatrix;
		GLfloat glmatrix[16];
		modelViewMatrix.writeToColumnMajorMatrix(glmatrix);
		glUniformMatrix4fv(modelViewMatrixUniformLocation, 1, false, glmatrix);

		GLfloat glmatrixProjection[16];
		projectionMatrix.writeToColumnMajorMatrix(glmatrixProjection);
		glUniformMatrix4fv(projectionMatrixUniformLocation, 1, false, glmatrixProjection);

		Matrix4 normalMatrixtemp = normalMatrix(modelMatrix);
		GLfloat glMatrixANormal[16];
		normalMatrixtemp.writeToColumnMajorMatrix(glMatrixANormal);
		glUniformMatrix4fv(normalMatrixUniformLocation, 1, false, glMatrixANormal);
		

		geometry.Draw(type);
	}
};



void display(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, 500, 500);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(program);
	
	int t = timeStart * 100;
	
	//EyeMatrix
	Matrix4 eyeMatrix;
	eyeMatrix = eyeMatrix.makeTranslation(Cvec3(0.0, 0.0, 10.0));

	//Projection Matrix
	Matrix4 projectionMatrix;
	projectionMatrix = projectionMatrix.makeTranslation(Cvec3(0.5, 0.5, 0.0));
	projectionMatrix = projectionMatrix.makeScale(Cvec3(0.5, 0.5, 1.0));
	projectionMatrix = projectionMatrix.makeProjection(45.0, 1.0, -0.1, -100.0);
	
	
	Entity Object3D;
	Object3D.t = Cvec3(0.0, -3.0, 0.0);
	Object3D.r = Cvec3(0.0, 0.0, 0.0);
	Object3D.s = Cvec3(0.5, 0.5, 0.5);
	Object3D.parent = NULL;
	Object3D.Draw(eyeMatrix, projectionMatrix, modelViewMatrixUniformLocation, projectionMatrixUniformLocation, normalMatrixUniformLocation, "Object3D");
	


	glDrawElements(GL_TRIANGLES, sizeof(VertexPN) * 10000, GL_UNSIGNED_SHORT, 0);

	glDisableVertexAttribArray(positionAttribute);
	glDisableVertexAttribArray(colorAttribute);
	glDisableVertexAttribArray(normalAttribute);

	glutSwapBuffers();




	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Start of FrameBuffer2
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer2);
	glViewport(0, 0, 500, 500);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(screenTrianglesProgram);
	glUniform1i(screenFramebufferUniform, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture); 

	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesPositionBuffer);
	glVertexAttribPointer(screenTrianglesPositionAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(screenTrianglesPositionAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesUVBuffer);
	glVertexAttribPointer(screenTrianglesTexCoordAttribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(screenTrianglesTexCoordAttribute);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(screenTrianglesPositionAttribute);
	glDisableVertexAttribArray(screenTrianglesTexCoordAttribute);

	glutSwapBuffers();
	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 500, 500);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Start of FrameBuufer3
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer3);
	glViewport(0, 0, 500, 500);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(screenTrianglesProgram2);
	glUniform1i(screenFramebufferUniform2, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture2);

	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesPositionBuffer2);
	glVertexAttribPointer(screenTrianglesPositionAttribute2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(screenTrianglesPositionAttribute2);

	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesUVBuffer2);
	glVertexAttribPointer(screenTrianglesTexCoordAttribute2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(screenTrianglesTexCoordAttribute2);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(screenTrianglesPositionAttribute2);
	glDisableVertexAttribArray(screenTrianglesTexCoordAttribute2);


	

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, 500, 500);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(screenTrianglesProgram3);
	glUniform1i(screenFramebufferUniform3, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture3);

	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesPositionBuffer3);
	glVertexAttribPointer(screenTrianglesPositionAttribute3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(screenTrianglesPositionAttribute3);

	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesUVBuffer3);
	glVertexAttribPointer(screenTrianglesTexCoordAttribute3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(screenTrianglesTexCoordAttribute3);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(screenTrianglesPositionAttribute3);
	glDisableVertexAttribArray(screenTrianglesTexCoordAttribute3);

	glutSwapBuffers();
	

}

void SphereGenerator() {
	int ibLen, vbLen;
	getSphereVbIbLen(8, 8, vbLen, ibLen);
	vector<VertexPN> vtx(vbLen);
	vector<unsigned short> idx(ibLen);
	makeSphere(2, 8, 8, vtx.begin(), idx.begin());

	// fill our arrays
	glGenBuffers(1, &vertexBO1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &indexBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
}

void PlaneGenerator() {
	int ibLen, vbLen;
	getPlaneVbIbLen(vbLen, ibLen);
	vector<VertexPN> vtx(vbLen);
	vector<unsigned short> idx(ibLen);
	makePlane(4, vtx.begin(), idx.begin());

	// fill our arrays
	glGenBuffers(1, &vertexBO2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &indexBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
}

void CubeGenerator() {
	int ibLen, vbLen;
	getCubeVbIbLen(vbLen, ibLen);
	vector<VertexPN> vtx(vbLen);
	vector<unsigned short> idx(ibLen);
	makeCube(3, vtx.begin(), idx.begin());

	// fill our arrays
	glGenBuffers(1, &vertexBO3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &indexBO3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
}

void init() {
	glClearDepth(0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_GREATER);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glReadBuffer(GL_BACK);

	timeStart = glutGet(GLUT_ELAPSED_TIME);

	glClearColor(1.0, 1.0, 1.0, 0.0);
	program = glCreateProgram();
	readAndCompileShader(program, "vertex.glsl", "fragment.glsl");
	screenTrianglesProgram = glCreateProgram();
	readAndCompileShader(screenTrianglesProgram, "vertex2.glsl", "fragment2.glsl");
	screenTrianglesProgram2 = glCreateProgram();
	readAndCompileShader(screenTrianglesProgram2, "vertex3.glsl", "fragment3.glsl");   //Put 3 unstead of 2 in vertex & fragment
	screenTrianglesProgram3 = glCreateProgram();
	readAndCompileShader(screenTrianglesProgram3, "vertex4.glsl", "fragment4.glsl");

	
	glUseProgram(program);
	positionAttribute = glGetAttribLocation(program, "position");
	colorAttribute = glGetAttribLocation(program, "color");
	modelViewMatrixUniformLocation = glGetUniformLocation(program, "modelViewMatrix");
	projectionMatrixUniformLocation = glGetUniformLocation(program, "projectionMatrix");
	positionUniform = glGetUniformLocation(program, "modelPosition");
	

	normalMatrixUniformLocation = glGetUniformLocation(program, "normalMatrix");
	normalAttribute = glGetAttribLocation(program, "normal");

	texCoordAttribute = glGetAttribLocation(program, "texCoord");
	diffuseTexture = loadGLTexture("Monk_Giveaway/Monk_D.tga");
	diffuseTextureUniformLocation = glGetUniformLocation(program, "diffuseTexture");

	CubeGenerator();
	SphereGenerator();
	PlaneGenerator();


	//FrameBuffer1
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glGenTextures(1, &frameBufferTexture);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, 500, 500, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);



	glGenTextures(1, &depthBufferTexture);
	glBindTexture(GL_TEXTURE_2D, depthBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 500, 500, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 500, 500);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//FrameBuffer2
	glGenFramebuffers(1, &frameBuffer2);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer2);

	glGenTextures(1, &frameBufferTexture2);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture2);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, 500, 500, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture2, 0);



	glGenTextures(1, &depthBufferTexture2);
	glBindTexture(GL_TEXTURE_2D, depthBufferTexture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 500, 500, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 500, 500);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferTexture2, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//FrameBuffer3
	glGenFramebuffers(1, &frameBuffer3);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer3);

	glGenTextures(1, &frameBufferTexture3);
	glBindTexture(GL_TEXTURE_2D, frameBufferTexture3);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, 500, 500, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture3, 0);



	glGenTextures(1, &depthBufferTexture3);
	glBindTexture(GL_TEXTURE_2D, depthBufferTexture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 500, 500, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 500, 500);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferTexture3, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	

	/*Now starts Second program*/
	
	glUseProgram(screenTrianglesProgram);
	screenFramebufferUniform = glGetUniformLocation(screenTrianglesProgram, "screenFramebuffer");
	
	screenTrianglesPositionAttribute = glGetAttribLocation(screenTrianglesProgram, "position");
	glGenBuffers(1, &screenTrianglesPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesPositionBuffer);
	GLfloat screenTrianglePositions[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		-1.0f, 1.0f,
		-1.0f, -1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), screenTrianglePositions, GL_STATIC_DRAW);


	screenTrianglesTexCoordAttribute = glGetAttribLocation(screenTrianglesProgram, "texCoord");
	glGenBuffers(1, &screenTrianglesUVBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesUVBuffer);
	GLfloat screenTriangleUVs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), screenTriangleUVs, GL_STATIC_DRAW);






	/*Now starts third program*/

	glUseProgram(screenTrianglesProgram2);
	screenFramebufferUniform2 = glGetUniformLocation(screenTrianglesProgram2, "screenFramebuffer2");
	

	screenTrianglesPositionAttribute2 = glGetAttribLocation(screenTrianglesProgram2, "position");
	glGenBuffers(1, &screenTrianglesPositionBuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesPositionBuffer2);
	
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), screenTrianglePositions, GL_STATIC_DRAW);


	screenTrianglesTexCoordAttribute2 = glGetAttribLocation(screenTrianglesProgram2, "texCoord");
	glGenBuffers(1, &screenTrianglesUVBuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesUVBuffer2);

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), screenTriangleUVs, GL_STATIC_DRAW);


	
	
	
	
	/*Now starts fourth fragment shader*/

	glUseProgram(screenTrianglesProgram3);
	screenFramebufferUniform3 = glGetUniformLocation(screenTrianglesProgram3, "screenFramebuffer3");

	screenTrianglesPositionAttribute3 = glGetAttribLocation(screenTrianglesProgram3, "position");
	glGenBuffers(1, &screenTrianglesPositionBuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesPositionBuffer3);
	
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), screenTrianglePositions, GL_STATIC_DRAW);

	screenTrianglesTexCoordAttribute3 = glGetAttribLocation(screenTrianglesProgram3, "texCoord");
	glGenBuffers(1, &screenTrianglesUVBuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, screenTrianglesUVBuffer3);
	
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), screenTriangleUVs, GL_STATIC_DRAW);

	

}

static void PrintInfo(const tinyobj::attrib_t& attrib,
	const std::vector<tinyobj::shape_t>& shapes,
	const std::vector<tinyobj::material_t>& materials) {
	vector<VertexPN> vtx;
	vector<unsigned short> idx;
	
	
	for (int i = 0; i < shapes.size(); i++) {
		for (int j = 0; j < shapes[i].mesh.indices.size(); j++) {
			unsigned int vertexOffset = shapes[i].mesh.indices[j].vertex_index * 3;
			unsigned int normalOffset = shapes[i].mesh.indices[j].normal_index * 3;
			unsigned int texOffset = shapes[i].mesh.indices[j].texcoord_index * 2;
			VertexPN v;
			v.p[0] = attrib.vertices[vertexOffset];
			v.p[1] = attrib.vertices[vertexOffset + 1];
			v.p[2] = attrib.vertices[vertexOffset + 2];
			v.n[0] = attrib.normals[normalOffset];
			v.n[1] = attrib.normals[normalOffset + 1];
			v.n[2] = attrib.normals[normalOffset + 2];
			v.t[0] = attrib.texcoords[texOffset];
			v.t[1] = 1.0 - attrib.texcoords[texOffset + 1];
			vtx.push_back(v);
			idx.push_back(vtx.size() - 1);
		}
	}
	
	glGenBuffers(1, &vertexBO4);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBO4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPN) * vtx.size(), vtx.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &indexBO4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBO4);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * idx.size(), idx.data(), GL_STATIC_DRAW);
	cout << "Loaded" << endl;

}


static bool TestLoadObj(const char* filename, const char* basepath = NULL,
	bool triangulate = true) {
	//std::cout << "Loading " << filename << std::endl;

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;


	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename,
		basepath, triangulate);


	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		printf("Failed to load/parse .obj.\n");
		return false;
	}

	PrintInfo(attrib, shapes, materials);

	return true;
}


void reshape(int w, int h) {
	glViewport(0, 0, w, h);
}

void idle(void) {
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("CS - 6533");

	glewInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);

	
	assert(true == TestLoadObj("Monk_Giveaway/Monk_Giveaway_Fixed.obj", "/", false));
	init();
	glutMainLoop();
	

	return 0;
}