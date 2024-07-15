///////////////////////////////////////////////////////////////////////////////
// shadermanager.cpp
// ============
// manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//	Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////

#include "SceneManager.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "ShapeMeshes.h"
#endif

#include <glm/gtx/transform.hpp>

// declaration of global variables
namespace
{
	const char* g_ModelName = "model";
	const char* g_ColorValueName = "objectColor";
	const char* g_TextureValueName = "objectTexture";
	const char* g_UseTextureName = "bUseTexture";
	const char* g_UseLightingName = "bUseLighting";
}

/***********************************************************
 *  SceneManager()
 *
 *  The constructor for the class
 ***********************************************************/
SceneManager::SceneManager(ShaderManager *pShaderManager)
{
	m_pShaderManager = pShaderManager;
	m_basicMeshes = new ShapeMeshes();
}

/***********************************************************
 *  ~SceneManager()
 *
 *  The destructor for the class
 ***********************************************************/
SceneManager::~SceneManager()
{
	m_pShaderManager = NULL;
	delete m_basicMeshes;
	m_basicMeshes = NULL;
}

/***********************************************************
 *  CreateGLTexture()
 *
 *  This method is used for loading textures from image files,
 *  configuring the texture mapping parameters in OpenGL,
 *  generating the mipmaps, and loading the read texture into
 *  the next available texture slot in memory.
 ***********************************************************/
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
	int width = 0;
	int height = 0;
	int colorChannels = 0;
	GLuint textureID = 0;

	// indicate to always flip images vertically when loaded
	stbi_set_flip_vertically_on_load(true);

	// try to parse the image data from the specified image file
	unsigned char* image = stbi_load(
		filename,
		&width,
		&height,
		&colorChannels,
		0);

	// if the image was successfully read from the image file
	if (image)
	{
		std::cout << "Successfully loaded image:" << filename << ", width:" << width << ", height:" << height << ", channels:" << colorChannels << std::endl;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if the loaded image is in RGB format
		if (colorChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		// if the loaded image is in RGBA format - it supports transparency
		else if (colorChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
		{
			std::cout << "Not implemented to handle image with " << colorChannels << " channels" << std::endl;
			return false;
		}

		// generate the texture mipmaps for mapping textures to lower resolutions
		glGenerateMipmap(GL_TEXTURE_2D);

		// free the image data from local memory
		stbi_image_free(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

		// register the loaded texture and associate it with the special tag string
		m_textureIDs[m_loadedTextures].ID = textureID;
		m_textureIDs[m_loadedTextures].tag = tag;
		m_loadedTextures++;

		return true;
	}

	std::cout << "Could not load image:" << filename << std::endl;

	// Error loading the image
	return false;
}

/***********************************************************
 *  BindGLTextures()
 *
 *  This method is used for binding the loaded textures to
 *  OpenGL texture memory slots.  There are up to 16 slots.
 ***********************************************************/
void SceneManager::BindGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  DestroyGLTextures()
 *
 *  This method is used for freeing the memory in all the
 *  used texture memory slots.
 ***********************************************************/
void SceneManager::DestroyGLTextures()
{
	for (int i = 0; i < m_loadedTextures; i++)
	{
		glGenTextures(1, &m_textureIDs[i].ID);
	}
}

/***********************************************************
 *  FindTextureID()
 *
 *  This method is used for getting an ID for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureID(std::string tag)
{
	int textureID = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureID = m_textureIDs[index].ID;
			bFound = true;
		}
		else
			index++;
	}

	return(textureID);
}

/***********************************************************
 *  FindTextureSlot()
 *
 *  This method is used for getting a slot index for the previously
 *  loaded texture bitmap associated with the passed in tag.
 ***********************************************************/
int SceneManager::FindTextureSlot(std::string tag)
{
	int textureSlot = -1;
	int index = 0;
	bool bFound = false;

	while ((index < m_loadedTextures) && (bFound == false))
	{
		if (m_textureIDs[index].tag.compare(tag) == 0)
		{
			textureSlot = index;
			bFound = true;
		}
		else
			index++;
	}

	return(textureSlot);
}

/***********************************************************
 *  FindMaterial()
 *
 *  This method is used for getting a material from the previously
 *  defined materials list that is associated with the passed in tag.
 ***********************************************************/
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& material)
{
	if (m_objectMaterials.size() == 0)
	{
		return(false);
	}

	int index = 0;
	bool bFound = false;
	while ((index < m_objectMaterials.size()) && (bFound == false))
	{
		if (m_objectMaterials[index].tag.compare(tag) == 0)
		{
			bFound = true;
			material.ambientColor = m_objectMaterials[index].ambientColor;
			material.ambientStrength = m_objectMaterials[index].ambientStrength;
			material.diffuseColor = m_objectMaterials[index].diffuseColor;
			material.specularColor = m_objectMaterials[index].specularColor;
			material.shininess = m_objectMaterials[index].shininess;
		}
		else
		{
			index++;
		}
	}

	return(true);
}

/***********************************************************
 *  SetTransformations()
 *
 *  This method is used for setting the transform buffer
 *  using the passed in transformation values.
 ***********************************************************/
void SceneManager::SetTransformations(
	glm::vec3 scaleXYZ,
	float XrotationDegrees,
	float YrotationDegrees,
	float ZrotationDegrees,
	glm::vec3 positionXYZ)
{
	// variables for this method
	glm::mat4 modelView;
	glm::mat4 scale;
	glm::mat4 rotationX;
	glm::mat4 rotationY;
	glm::mat4 rotationZ;
	glm::mat4 translation;

	// set the scale value in the transform buffer
	scale = glm::scale(scaleXYZ);
	// set the rotation values in the transform buffer
	rotationX = glm::rotate(glm::radians(XrotationDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
	rotationY = glm::rotate(glm::radians(YrotationDegrees), glm::vec3(0.0f, 1.0f, 0.0f));
	rotationZ = glm::rotate(glm::radians(ZrotationDegrees), glm::vec3(0.0f, 0.0f, 1.0f));
	// set the translation value in the transform buffer
	translation = glm::translate(positionXYZ);

	modelView = translation * rotationZ * rotationY * rotationX * scale;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setMat4Value(g_ModelName, modelView);
	}
}

/***********************************************************
 *  SetShaderColor()
 *
 *  This method is used for setting the passed in color
 *  into the shader for the next draw command
 ***********************************************************/
void SceneManager::SetShaderColor(
	float redColorValue,
	float greenColorValue,
	float blueColorValue,
	float alphaValue)
{
	// variables for this method
	glm::vec4 currentColor;

	currentColor.r = redColorValue;
	currentColor.g = greenColorValue;
	currentColor.b = blueColorValue;
	currentColor.a = alphaValue;

	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, false);
		m_pShaderManager->setVec4Value(g_ColorValueName, currentColor);
	}
}

/***********************************************************
 *  SetShaderTexture()
 *
 *  This method is used for setting the texture data
 *  associated with the passed in ID into the shader.
 ***********************************************************/
void SceneManager::SetShaderTexture(
	std::string textureTag)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setIntValue(g_UseTextureName, true);

		int textureID = -1;
		textureID = FindTextureSlot(textureTag);
		m_pShaderManager->setSampler2DValue(g_TextureValueName, textureID);
	}
}

/***********************************************************
 *  SetTextureUVScale()
 *
 *  This method is used for setting the texture UV scale
 *  values into the shader.
 ***********************************************************/
void SceneManager::SetTextureUVScale(float u, float v)
{
	if (NULL != m_pShaderManager)
	{
		m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
	}
}

/***********************************************************
 *  SetShaderMaterial()
 *
 *  This method is used for passing the material values
 *  into the shader.
 ***********************************************************/
void SceneManager::SetShaderMaterial(
	std::string materialTag)
{
	if (m_objectMaterials.size() > 0)
	{
		OBJECT_MATERIAL material;
		bool bReturn = false;

		bReturn = FindMaterial(materialTag, material);
		if (bReturn == true)
		{
			m_pShaderManager->setVec3Value("material.ambientColor", material.ambientColor);
			m_pShaderManager->setFloatValue("material.ambientStrength", material.ambientStrength);
			m_pShaderManager->setVec3Value("material.diffuseColor", material.diffuseColor);
			m_pShaderManager->setVec3Value("material.specularColor", material.specularColor);
			m_pShaderManager->setFloatValue("material.shininess", material.shininess);
		}
	}
}

/**************************************************************/
/*** STUDENTS CAN MODIFY the code in the methods BELOW for  ***/
/*** preparing and rendering their own 3D replicated scenes.***/
/*** Please refer to the code in the OpenGL sample project  ***/
/*** for assistance.                                        ***/
/**************************************************************/

/***********************************************************
  *  DefineObjectMaterials()
  *
  *  This method is used for configuring the various material
  *  settings for all of the objects within the 3D scene.
  ***********************************************************/
void SceneManager::DefineObjectMaterials()
{
	/*** STUDENTS - add the code BELOW for defining object materials. ***/
	/*** There is no limit to the number of object materials that can ***/
	/*** be defined. Refer to the code in the OpenGL Sample for help  ***/

	OBJECT_MATERIAL goldMaterial;
	goldMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.1f);
	goldMaterial.ambientStrength = 0.4f;
	goldMaterial.diffuseColor = glm::vec3(0.3f, 0.3f, 0.2f);
	goldMaterial.specularColor = glm::vec3(0.6f, 0.5f, 0.4f);
	goldMaterial.shininess = 22.0;
	goldMaterial.tag = "gold";
	m_objectMaterials.push_back(goldMaterial);

	OBJECT_MATERIAL stainlessMaterial;
	stainlessMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	stainlessMaterial.ambientStrength = 0.4f;
	stainlessMaterial.diffuseColor = glm::vec3(0.3f, 0.3f, 0.3f);
	stainlessMaterial.specularColor = glm::vec3(0.6f, 0.6f, 0.6f);
	stainlessMaterial.shininess = 22.0;
	stainlessMaterial.tag = "stainless";
	m_objectMaterials.push_back(stainlessMaterial);


	OBJECT_MATERIAL cementMaterial;
	cementMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);
	cementMaterial.ambientStrength = 0.2f;
	cementMaterial.diffuseColor = glm::vec3(0.5f, 0.5f, 0.5f);
	cementMaterial.specularColor = glm::vec3(0.4f, 0.4f, 0.4f);
	cementMaterial.shininess = 0.5;
	cementMaterial.tag = "cement";
	m_objectMaterials.push_back(cementMaterial);

	OBJECT_MATERIAL woodMaterial;
	woodMaterial.ambientColor = glm::vec3(0.4f, 0.3f, 0.1f);
	woodMaterial.ambientStrength = 0.2f;
	woodMaterial.diffuseColor = glm::vec3(0.3f, 0.2f, 0.1f);
	woodMaterial.specularColor = glm::vec3(0.1f, 0.1f, 0.1f);
	woodMaterial.shininess = 0.3;
	woodMaterial.tag = "wood";
	m_objectMaterials.push_back(woodMaterial);

	OBJECT_MATERIAL tileMaterial;
	tileMaterial.ambientColor = glm::vec3(0.2f, 0.3f, 0.4f);
	tileMaterial.ambientStrength = 0.3f;
	tileMaterial.diffuseColor = glm::vec3(0.3f, 0.2f, 0.1f);
	tileMaterial.specularColor = glm::vec3(0.4f, 0.5f, 0.6f);
	tileMaterial.shininess = 25.0;
	tileMaterial.tag = "tile";
	m_objectMaterials.push_back(tileMaterial);

	OBJECT_MATERIAL glassMaterial;
	glassMaterial.ambientColor = glm::vec3(0.4f, 0.4f, 0.4f);
	glassMaterial.ambientStrength = 0.3f;
	glassMaterial.diffuseColor = glm::vec3(0.3f, 0.3f, 0.3f);
	glassMaterial.specularColor = glm::vec3(0.6f, 0.6f, 0.6f);
	glassMaterial.shininess = 85.0;
	glassMaterial.tag = "glass";
	m_objectMaterials.push_back(glassMaterial);

	OBJECT_MATERIAL clayMaterial;
	clayMaterial.ambientColor = glm::vec3(0.2f, 0.2f, 0.3f);
	clayMaterial.ambientStrength = 0.3f;
	clayMaterial.diffuseColor = glm::vec3(0.4f, 0.4f, 0.5f);
	clayMaterial.specularColor = glm::vec3(0.2f, 0.2f, 0.4f);
	clayMaterial.shininess = 0.5;
	clayMaterial.tag = "clay";
	m_objectMaterials.push_back(clayMaterial);


}

/***********************************************************
 *  SetupSceneLights()
 *
 *  This method is called to add and configure the light
 *  sources for the 3D scene.  There are up to 4 light sources.
 ***********************************************************/
void SceneManager::SetupSceneLights()
{
	// this line of code is NEEDED for telling the shaders to render 
	// the 3D scene with custom lighting, if no light sources have
	// been added then the display window will be black - to use the 
	// default OpenGL lighting then comment out the following line

	//m_pShaderManager->setBoolValue(g_UseLightingName, true);

	/*** STUDENTS - add the code BELOW for setting up light sources ***/
	/*** Up to four light sources can be defined. Refer to the code ***/
	/*** in the OpenGL Sample for help                              ***/
	
    m_pShaderManager->setVec3Value("lightSources[0].position", -6.0f, 6.0f, 12.0f);
	m_pShaderManager->setVec3Value("lightSources[0].ambientColor", 0.0f, 0.0f, 0.0f);
	m_pShaderManager->setVec3Value("lightSources[0].diffuseColor", 0.0f, 0.0f, 0.0f);
	m_pShaderManager->setVec3Value("lightSources[0].specularColor", 0.1f, 0.1f, 0.1f);
	m_pShaderManager->setFloatValue("lightSources[0].focalStrength", 0.125);
	m_pShaderManager->setFloatValue("lightSources[0].specularIntensity", 0.125f);
	
	m_pShaderManager->setVec3Value("lightSources[1].position", 0.0f, 20.0f, 0.0f);
	m_pShaderManager->setVec3Value("lightSources[1].ambientColor", 0.01f, 0.01f, 0.05f);
	m_pShaderManager->setVec3Value("lightSources[1].diffuseColor", 0.5f, 0.5f, 0.1f);
	m_pShaderManager->setVec3Value("lightSources[1].specularColor", 0.0f, 0.0f, 0.0f);
	m_pShaderManager->setFloatValue("lightSources[1].focalStrength", 1.5f);
	m_pShaderManager->setFloatValue("lightSources[1].specularIntensity", 0.0f);
	
	m_pShaderManager->setBoolValue("bUseLighting", true);

}

 /***********************************************************
  *  LoadSceneTextures()
  *
  *  This method is used for preparing the 3D scene by loading
  *  the shapes, textures in memory to support the 3D scene
  *  rendering
  ***********************************************************/
void SceneManager::LoadSceneTextures()
{
	/*** STUDENTS - add the code BELOW for loading the textures that ***/
	/*** will be used for mapping to objects in the 3D scene. Up to  ***/
	/*** 16 textures can be loaded per scene. Refer to the code in   ***/
	/*** the OpenGL Sample for help.                                 ***/


	bool bReturn = false;

	bReturn = CreateGLTexture("Source/floor.jpg", "floor");
	bReturn = CreateGLTexture("Source/stainless.jpg", "metal");
	bReturn = CreateGLTexture("Source/wallpaper.jpg", "wallpaper");
	bReturn = CreateGLTexture("Source/wood.jpg", "table");
	bReturn = CreateGLTexture("Source/Army.jpg", "army");
	bReturn = CreateGLTexture("Source/camo.jpg", "camo");
	bReturn = CreateGLTexture("Source/bagcoffee.jpg", "bagcoffee");
	bReturn = CreateGLTexture("Source/ingredients.jpg", "ingredients");
	bReturn = CreateGLTexture("Source/bag.jpg", "bag");

	// after the texture image data is loaded into memory, the
	// loaded textures need to be bound to texture slots - there
	// are a total of 16 available slots for scene textures
	BindGLTextures();
}


/***********************************************************
 *  PrepareScene()
 *
 *  This method is used for preparing the 3D scene by loading
 *  the shapes, textures in memory to support the 3D scene 
 *  rendering
 ***********************************************************/
void SceneManager::PrepareScene()
{

	// define the materials for objects in the scene
	DefineObjectMaterials();

	LoadSceneTextures();

	// add and define the light sources for the scene
	SetupSceneLights();

	// only one instance of a particular mesh needs to be
	// loaded in memory no matter how many times it is drawn
	// in the rendered 3D scene

	m_basicMeshes->LoadPlaneMesh();
	m_basicMeshes->LoadBoxMesh();
	m_basicMeshes->LoadPrismMesh();
	m_basicMeshes->LoadCylinderMesh();
	m_basicMeshes->LoadTorusMesh();
	m_basicMeshes->LoadPyramid3Mesh();

}

/***********************************************************
 *  RenderScene()
 *
 *  This method is used for rendering the 3D scene by 
 *  transforming and drawing the basic 3D shapes
 ***********************************************************/
void SceneManager::RenderScene()
{
	// declare the variables for the transformations
	glm::vec3 scaleXYZ;
	float XrotationDegrees = 0.0f;
	float YrotationDegrees = 0.0f;
	float ZrotationDegrees = 0.0f;
	glm::vec3 positionXYZ;

	// Set Texture scale to 4x4 for first three (plane) shapes
	// reset later
	SetTextureUVScale(4.0, 4.0);

	// set dimensions of shapes


	/******************************************************************/
	/*** Right Plane												***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(10.0f, 1.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 90.0f; 
	YrotationDegrees = -45.0f;
	ZrotationDegrees = 0.0f;
	
	// set the XYZ position for the mesh
	//printf("%f\n", (10.0 * sqrt(2.0)) / 2.0f); // ~7.07
	positionXYZ = glm::vec3(7.07f, 10.0f, 7.07f);
	

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0, 0, 1, 1);

	SetShaderTexture("wallpaper");
	SetShaderMaterial("cement");
	// draw the mesh with transformation values
	m_basicMeshes->DrawPlaneMesh();

	/******************************************************************/
	/*** Left Plane													***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(10.0f, 1.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 90.0f;
	YrotationDegrees = 45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	//printf("%f\n", (10.0 * sqrt(2.0)) / 2.0f); // ~7.07
	positionXYZ = glm::vec3(-7.07,10.0f,7.07f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("wallpaper");
	SetShaderMaterial("cement");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPlaneMesh();


	/******************************************************************/
	/*** Bottom Plane												***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	scaleXYZ = glm::vec3(10.0f, 1.0f, 10.0f);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	// printf("%f\n",sqrt(15*15+15*15)); ~21.2
	positionXYZ = glm::vec3(0.0f, 0.0f, 14.14f);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("floor");
	SetShaderMaterial("tile");
	
	// draw the mesh with transformation values
	SetTextureUVScale(16.0, 16.0);
	m_basicMeshes->DrawPlaneMesh();


	// Reset UV Scale to 1.0,1.0
	SetTextureUVScale(1.0, 1.0);

/******************************************************************/
/*** Box (Table)												***/
/*** Set needed transformations before drawing the basic mesh.  ***/
/*** This same ordering of code should be used for transforming ***/
/*** and drawing all the basic 3D shapes.						***/
/******************************************************************/

// set the XYZ scale for the mesh
	float side = 5.0;
	scaleXYZ = glm::vec3(side, side, side);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 45.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	positionXYZ = glm::vec3(0.0f, side / 2.0, (side * sqrt(2.0)) / 2.0);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("table");
	SetShaderMaterial("wood");

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();

	/******************************************************************/
	/*** Box (Bottom of coffee bag)								   	***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	
	// set the XYZ scale for the mesh
	float bagBottomX = 0.5;
	float bagBottomY = 1.5;
	float bagBottomZ = 1.0;
	scaleXYZ = glm::vec3(bagBottomX, bagBottomY, bagBottomZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 110.0f;
	ZrotationDegrees = 0.0f;

	// set the XYZ position for the mesh
	float xPos = -1.5;
	float yPos = bagBottomY / 2 + side; // side of table
	float zPos = 3.5;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

    SetShaderTexture("bagcoffee");
	SetShaderMaterial("clay");


	m_basicMeshes->DrawBoxMeshSide(ShapeMeshes::BoxSide::left);

	SetShaderTexture("ingredients");
	m_basicMeshes->DrawBoxMeshSide(ShapeMeshes::BoxSide::back);
	
	SetShaderTexture("bag");
	m_basicMeshes->DrawBoxMeshSide(ShapeMeshes::BoxSide::front);
	m_basicMeshes->DrawBoxMeshSide(ShapeMeshes::BoxSide::right);
	m_basicMeshes->DrawBoxMeshSide(ShapeMeshes::BoxSide::top);
	m_basicMeshes->DrawBoxMeshSide(ShapeMeshes::BoxSide::bottom);

	// draw the mesh with transformation values
	m_basicMeshes->DrawBoxMesh();


	/******************************************************************/
	/*** Prism (Top of coffee bag)									***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh
	
	float bagTopX = 0.5;
	float bagTopY = 1.0;
	float bagTopZ =	1.0;

	scaleXYZ = glm::vec3(bagTopX, bagTopY, bagTopZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = -90.0f;
	YrotationDegrees = 110.0f;
	ZrotationDegrees = 0.0f;
	

	// set the XYZ position for the mesh

	xPos = -1.5;
	//yPos = side + bagBottomY + bagTopY / 2;
	yPos = 7;
	zPos = 3.5;

	positionXYZ = glm::vec3(xPos, yPos, zPos);
	
	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("bag");
	SetShaderMaterial("clay");
	
	// draw the mesh with transformation values
	m_basicMeshes->DrawPrismMesh();


	/******************************************************************/
	/*** Coffee Press Body											***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh

	float pressBodyX = 0.75;
	float pressBodyY = 2.0;
	float pressBodyZ = 0.75;

	scaleXYZ = glm::vec3(pressBodyX, pressBodyY, pressBodyZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh

	xPos = 1.5;
	yPos = 5;
	zPos = 3.5;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("metal");
	SetShaderMaterial("stainless");
	
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/******************************************************************/
	/*** Coffee Press Lid											***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh

	float pressLidX = 0.75;
	float pressLidY = 0.25;
	float pressLidZ = 0.75;

	scaleXYZ = glm::vec3(pressLidX, pressLidY, pressLidZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh

	xPos = 1.5;
	yPos = 7;
	zPos = 3.5;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("metal");
	// draw the mesh with transformation values
	
	m_basicMeshes->DrawCylinderMesh();

	/******************************************************************/
	/*** Coffee Press Rod											***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh

	float pressRodX = 0.05f;
	float pressRodY = 0.25f;
	float pressRodZ = 0.05f;

	scaleXYZ = glm::vec3(pressRodX, pressRodY, pressRodZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh

	xPos = 1.5;
	yPos = 7.25;
	zPos = 3.5;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("metal");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/******************************************************************/
	/*** Coffee Press Stopper										***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh

	float pressStopperX = 0.1f;
	float pressStopperY = 0.25f;
	float pressStopperZ = 0.1f;

	scaleXYZ = glm::vec3(pressStopperX, pressStopperY, pressStopperZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh

	xPos = 1.5;
	yPos = 7.5;
	zPos = 3.5;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("metal");
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/******************************************************************/
	/*** Coffee Press Plunger										***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh

	float pressPlungerX = 0.3f;
	float pressPlungerY = 0.125f;
	float pressPlungerZ = 0.3f;

	scaleXYZ = glm::vec3(pressPlungerX, pressPlungerY, pressPlungerZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh

	xPos = 1.5;
	yPos = 7.75;
	zPos = 3.5;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("metal");

	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/******************************************************************/
	/*** Coffee Press Connector										***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/
	// set the XYZ scale for the mesh

	float pressConnectorX = 0.15f;
	float pressConnectorY = 0.5f;
	float pressConnectorZ = 0.15f;

	scaleXYZ = glm::vec3(pressConnectorX, pressConnectorY, pressConnectorZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 90.0f;


	// set the XYZ position for the mesh
	// right side of pot
	//xPos = 1.5 + .75 + pressConnectorY;
	xPos = 2.75;
	yPos = 6.75;
	zPos = 3.5;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("metal");

	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/******************************************************************/
    /*** Coffee Press Handle										***/
    /*** Set needed transformations before drawing the basic mesh.  ***/
    /*** This same ordering of code should be used for transforming ***/
    /*** and drawing all the basic 3D shapes.						***/
    /******************************************************************/
    
	// set the XYZ scale for the mesh

	float pressHandleX = 0.15f;
	float pressHandleY = 1.5f;
	float pressHandleZ = 0.15f;

	scaleXYZ = glm::vec3(pressHandleX, pressHandleY, pressHandleZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh
	// right side of pot + connector + handleX
	xPos = 2.75;
	// just below top of pot;
	yPos = 5.5;
	zPos = 3.5;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("metal");
	
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/******************************************************************/
	/*** Coffee Press Spout											***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/

	// set the XYZ scale for the mesh

	float pressSpoutX = 0.75f;
	float pressSpoutY = 0.75f;
	float pressSpoutZ = 0.75f;

	scaleXYZ = glm::vec3(pressSpoutX, pressSpoutY, pressSpoutZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 45.0f;
	ZrotationDegrees = 180.0f;


	// set the XYZ position for the mesh
	xPos = 0.875;
	yPos = 6.575;
	zPos = 3.25;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	//SetShaderColor(0, 1, 0, 1);
	SetShaderTexture("metal");

	// draw the mesh with transformation values
	m_basicMeshes->DrawPyramid3Mesh();

	/******************************************************************/
    /*** Cup														***/
    /*** Set needed transformations before drawing the basic mesh.  ***/
    /*** This same ordering of code should be used for transforming ***/
    /*** and drawing all the basic 3D shapes.						***/
    /******************************************************************/

    // set the XYZ scale for the mesh

	float cupX = 0.5f;
	float cupY = 1.0f;
	float cupZ = 0.5f;

	scaleXYZ = glm::vec3(cupX, cupY, cupZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh
	// right side of pot + connector + handleX
	xPos = 0.0;
	// on Table
	yPos = 5.0;
	zPos = 5.0;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("army");
	SetShaderMaterial("glass");

	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/******************************************************************/
    /*** Cup Top													***/
    /*** Set needed transformations before drawing the basic mesh.  ***/
    /*** This same ordering of code should be used for transforming ***/
    /*** and drawing all the basic 3D shapes.						***/
    /******************************************************************/

    // set the XYZ scale for the mesh

	float cupTopX = 0.5f;
	float cupTopY = 0.01f;
	float cupTopZ = 0.5f;

	scaleXYZ = glm::vec3(cupTopX, cupTopY, cupTopZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh
	// right side of pot + connector + handleX
	xPos = 0.0;
	// on Table
	yPos = 6.0;
	zPos = 5.0;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(1, 1, 1, 1);
	
	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();

	/******************************************************************/
	/*** Cup Bottom													***/
	/*** Set needed transformations before drawing the basic mesh.  ***/
	/*** This same ordering of code should be used for transforming ***/
	/*** and drawing all the basic 3D shapes.						***/
	/******************************************************************/

	// set the XYZ scale for the mesh

	float cupBottomX = 0.5f;
	float cupBottomY = 0.01f;
	float cupBottomZ = 0.5f;

	scaleXYZ = glm::vec3(cupBottomX, cupBottomY, cupBottomZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh
	// right side of pot + connector + handleX
	xPos = 0.0;
	// on Table
	yPos = 4.99;
	zPos = 5.0;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderColor(1, 1, 1, 1);


	// draw the mesh with transformation values
	m_basicMeshes->DrawCylinderMesh();


	/******************************************************************/
    /*** Cup Handle													***/
    /*** Set needed transformations before drawing the basic mesh.  ***/
    /*** This same ordering of code should be used for transforming ***/
    /*** and drawing all the basic 3D shapes.						***/
    /******************************************************************/

    // set the XYZ scale for the mesh

	float handleX = 0.5f;
	float handleY = 0.375;
	float handleZ = 0.5f;

	scaleXYZ = glm::vec3(handleX, handleY, handleZ);

	// set the XYZ rotation for the mesh
	XrotationDegrees = 0.0f;
	YrotationDegrees = 0.0f;
	ZrotationDegrees = 0.0f;


	// set the XYZ position for the mesh
	// right side of cup
	xPos = 0.5;
	// on Table + center of cup
	yPos = 5.5;
	zPos = 5.0;

	positionXYZ = glm::vec3(xPos, yPos, zPos);

	// set the transformations into memory to be used on the drawn meshes
	SetTransformations(
		scaleXYZ,
		XrotationDegrees,
		YrotationDegrees,
		ZrotationDegrees,
		positionXYZ);

	SetShaderTexture("camo");

	// draw the mesh with transformation values
	m_basicMeshes->DrawTorusMesh();

	// wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	/****************************************************************/
}
