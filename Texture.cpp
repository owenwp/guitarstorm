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
	string folder = "images/";
	name = folder + name + ".png";
	
	ILuint texid;
	ilGenImages(1, &texid);
	ilBindImage(texid); 
	
	if(ilLoadImage(name.c_str()))
	{
		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); 
	
		glGenTextures(1, &id); 
		glBindTexture(GL_TEXTURE_2D, id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
					 ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
					 ilGetData());
		
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