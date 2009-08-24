/*
 This file is part of Guitar Storm.
 Copyright 2008 Zombie Process, Owen Pedrotti
 
 Guitar Storm is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Guitar Storm is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Guitar Storm.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Texture.h"
#include <math.h>
#include <vector>

map<string, Texture*> textures;
map<int, Texture*> shapes;

const string Location = "images/";

unsigned char edgeDistance(unsigned char* mask, int w, int h, float x, float y, float domain)
{	
	int ix = x*w;
	int iy = y*h;
	
	int inside;
	if(ix<0 || iy<0 || ix>=w || iy>=h)
	{
		inside = 0;
	}
	else
	{
		inside = mask[ix + iy*w] != 0;
	}

	int kernel = 16;
	int mindist;
	float mindist2 = 512*512;
	
	for(int i=-kernel; i<kernel; i++)
	for(int j=-kernel; j<kernel; j++)
	{
		if(ix+i<0 || iy+j<0 || ix+i>=w || iy+j>=h)
		{
			if(inside != 0)
			{
				float dist2 = i*i + j*j;
				if(dist2 < mindist2)
					mindist2 = dist2;
			}
		}
		else if((mask[ix+iy*w + i+j*w] != 0) != inside)
		{
			float dist2 = i*i + j*j;
			if(dist2 < mindist2)
				mindist2 = dist2;
		}
	}
	
	if(mindist2 > kernel*kernel)
	{
		mindist = 127;
	}
	else
	{
		mindist = sqrt(mindist2) / kernel * 127;
	}
	
	return inside ? 128+mindist : 128-mindist;
}

void Texture::preLoad(string name)
{
	loaded = false;
	textures[name] = this;
	
	mWidth = 0;
	mHeight = 0;
	mData = NULL;
	
	int index = name.find_last_of('.');
	string type;
	if(index != string::npos)
	{
		type = name.substr(index, 4);
		name = name.substr(0, index);
	}
	else
		type = ".png";
	
	string mask = Location + name + "_mask" + type;
	
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
		
		//ilDeleteImages(1, &maskid);
		
		blend = false;
	}
	
	name = Location + name + type;
	
	ILuint texid;
	ilGenImages(1, &texid);
	ilBindImage(texid);
	
	if(ilLoadImage(name.c_str()))
	{
		depth = ilGetInteger(IL_IMAGE_BPP);
		width = ilGetInteger(IL_IMAGE_WIDTH);
		height = ilGetInteger(IL_IMAGE_HEIGHT);
		iData = ilGetData();
		
		glGenTextures(1, &id);
		
		if(mData)
		{
			vData = (unsigned char *)malloc(width * height * 4);
		}
		else
		{
			postLoad();
		}
		
		//ilDeleteImages(1, &texid);
		
		edge = 0.25f;
	}
}

void Texture::run()
{
	if(mData)
	{
		for(int i=0; i<width; i++)
		for(int j=0; j<height; j++)
		{
			vData[(i + j*width)*4+3] = edgeDistance(mData, mWidth, mHeight, i/(float)width, j/(float)height, 1.0f);
		}
	}
}

void Texture::postLoad()
{
	if(loaded) 
		return;
	
	if(mData)
	{
		//free(mData);
		
		long int pos1 = 0, pos2 = 0;
		
		for(int i=0; i<width; i++)
		for(int j=0; j<height; j++)
		{
			pos1 = (i + j*width)*4;
			pos2 = (i + j*width)*3;
			vData[pos1++] = iData[pos2++];
			vData[pos1++] = iData[pos2++];
			vData[pos1++] = iData[pos2++];
		}
	
		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); 
		
		glBindTexture(GL_TEXTURE_2D, id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, 4, width,
					 height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
					 vData);
		
		edge = 100.0f / mWidth;
		
		free(vData);
	}
	else
	{ 
		glBindTexture(GL_TEXTURE_2D, id);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, depth, width,
					 height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
					 iData);
	}
	
	aspect = width / (float)height;
	
	//free(iData);
	
	loaded = true;
}

void Texture::Cache(string name)
{
	if(textures.find(name) == textures.end())
	{
		Texture* t = new Texture();
		t->preLoad(name);
		t->loading = new thread(t);
	}
}

void Texture::UnloadAll()
{
	for(map<string, Texture*>::iterator ti = textures.begin(); ti != textures.end(); ti++)
	{
		if(ti->second->loading)
		{
			ti->second->loading->cancel();
			ti->second->loading->join();
		}
		delete ti->second;
	}
	
	for(map<int, Texture*>::iterator si = shapes.begin(); si != shapes.end(); si++)
	{
		delete si->second;
	}
}

Texture::Texture(spriteShape shape)
{
	alphaOnly = true;
	blend = false;
	
	if(shapes.find(shape) != shapes.end())
	{
		id = shapes[shape]->id;
		edge = shapes[shape]->edge;
		aspect = shapes[shape]->aspect;
		return;
	}
	
	shapes[shape] = this;
	
	unsigned char* tex = (unsigned char*)malloc(16 * 16);
	
	switch (shape) 
	{
		case shapeCircle:
			for(int i=0; i<16; i++)
			for(int j=0; j<16; j++)
			{
				int x = i-8;
				int y = j-8;
				float dist2 = sqrt(x*x + y*y);
				
				dist2 -= 7;
				dist2 *= -0.1f;
				dist2 += 0.5f;
				dist2 = min(1.0f, max(0.0f, dist2));
				
				tex[i+16*j] = (unsigned char)(dist2 * 255.0f);
			}
			
			edge = 1.5f;
			aspect = 1;
			break;
	}
	
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 16, 16, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex);
	
	free(tex);
	
	loaded = true;
}

Texture::Texture(string name)
{ 
	loading = NULL;
	alphaOnly = false;
	blend = true;
	
	if(textures.find(name) != textures.end())
	{
		Texture* t = textures[name];
		if(t->loading)
			t->loading->join();
		
		t->postLoad();
		
		id = t->id;
		edge = t->edge;
		aspect = t->aspect;
		blend = t->blend;
		return;
	}
	
	preLoad(name);
			
	run();
			
	postLoad();
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

float Texture::Aspect()
{
	return aspect;
}

void Texture::Bind(GLint p)
{
	glBindTexture(GL_TEXTURE_2D, id);
	
	if(p)
	{
		GLint loc = glGetUniformLocation(p,"edgeSize");
		glUniform1f(loc, edge);
		loc = glGetUniformLocation(p,"alphaOnly");
		glUniform1i(loc, alphaOnly);
		loc = glGetUniformLocation(p,"blend");
		glUniform1i(loc, blend);
	}
	else
	{
		if(blend)
		{
			glEnable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}
		else
		{
			glEnable(GL_ALPHA_TEST);
			glDisable(GL_BLEND);
		}
	}
}