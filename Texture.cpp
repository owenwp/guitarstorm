/*
 *  Texture.cpp
 *  guitarstorm
 *
 *  Created by Owen Pedrotti on 8/11/09.
 *  Copyright 2009 Zombie Process. All rights reserved.
 *
 */

#include "Texture.h"

Texture::Texture(string name)
{ 
	int mWidth = 0;
	int mHeight = 0;
	unsigned char* mData = NULL;
	
	string folder = "images/";
	string mask = folder + name + "_mask.png";
	
	// see if there is an alpha mask
	ILuint maskid;
	ilGenImages(1, &maskid);
	ilBindImage(maskid); 
	
	if(ilLoadImage(mask.c_str()))
	{
		ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE); 
		
		mWidth = ilGetInteger(IL_IMAGE_WIDTH);
		mHeight = ilGetInteger(IL_IMAGE_HEIGHT);
		mData = ilGetData();
	}
	
	name = folder + name + ".png";
	
	ILuint texid;
	ilGenImages(1, &texid);
	ilBindImage(texid); 
	
	if(ilLoadImage(name.c_str()))
	{
		glGenTextures(1, &id); 
		glBindTexture(GL_TEXTURE_2D, id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		int width = ilGetInteger(IL_IMAGE_WIDTH);
		int height = ilGetInteger(IL_IMAGE_HEIGHT);
		int depth = ilGetInteger(IL_IMAGE_BPP);
		unsigned char* iData = ilGetData();
		
		if(mData)
		{
			unsigned char* vData = (unsigned char *)malloc(width * height * 4);
			long int pos1 = 0, pos2 = 0;
			
			for(int i=0; i<width; i++)
			for(int j=0; j<height; j++)
			{
				vData[pos1++] = iData[pos2++];
				vData[pos1++] = iData[pos2++];
				vData[pos1++] = iData[pos2++];
				vData[pos1++] = (depth == 4 ? iData[pos2++] : 0xff) * (mData[i*mHeight*8+j*8] == 0 ? 0 : 1);
			}
			
			glTexImage2D(GL_TEXTURE_2D, 0, 4, width,
						height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
						 vData);
			
			free(vData);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, depth, width,
						 height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
						 iData);
		}

		
		edge = 1.0f;
	}
}

Texture::~Texture()
{
}

void Texture::Bind(GLint p)
{
	glBindTexture(GL_TEXTURE_2D, id);
	
	GLint loc = glGetUniformLocation(p,"edgeSize");
	glUniform1f(loc, edge);
	
	loc = glGetUniformLocation(p,"alphaOnly");
	glUniform1i(loc, 0);
}