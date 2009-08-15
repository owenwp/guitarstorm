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

#include "Label.h"
#include "Texture.h"

FT_Library ft;
map<string, Font*>fonts;

const string Location = "/library/fonts/";

inline int next_p2 (int a )
{
	int rval=1;
	// rval<<=1 Is A Prettier Way Of Writing rval*=2; 
	while(rval<a) rval<<=1;
	return rval;
}

Label::Label(string f, string t)
{
	text = t;
	color = vec(1,1,1,1);
	tints = 0;
	
	makeFont(f);
}

void Label::printf(string format, ...)
{
}

void Label::update(float timeDelta)
{
	color += (tint - color) * min(1.0f, tints * timeDelta);
}

void Label::render(GLint program)
{
	glPushMatrix();
	
	const char* cstr = text.c_str();
	int count = text.length();
	
	GLint loc = glGetUniformLocation(program,"edgeSize");
	glUniform1f(loc, 0.5);
	loc = glGetUniformLocation(program,"alphaOnly");
	glUniform1i(loc, 1);
	
	for(int i=0; i<count; i++)
	{
		glBindTexture(GL_TEXTURE_2D, font->chars[cstr[i]]);
		
		glBegin(GL_TRIANGLE_STRIP);
		
		glColor4f(color.x, color.y, color.z, 1);
		
		glTexCoord2f(0.0, font->ypad[cstr[i]]);
		glVertex3f(0, 0, 0.0);
		
		glTexCoord2f(font->xpad[cstr[i]], font->ypad[cstr[i]]);
		glVertex3f(font->wid[cstr[i]], 0, 0.0);
		
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0, font->hgt[cstr[i]], 0.0);
		
		glTexCoord2f(font->xpad[cstr[i]], 0.0);
		glVertex3f(font->wid[cstr[i]], font->hgt[cstr[i]], 0.0);
		
		glEnd();
		
		if(i+1 < count)
			glTranslatef(font->kerning[cstr[i]][cstr[i+1]], 0, 0);
	}
	
	glPopMatrix();
}

void Label::makeFont(string f)
{	
	if(fonts.find(f) != fonts.end())
	{
		font = fonts[f];
		return;
	}
	
	string floc = Location + f + ".ttf";
	font = fonts[f] = new Font();
	
	font->point = 200;
	
	// Create And Initilize A FreeType Font Library.
	FT_Init_FreeType( &ft );
	
	// This Is Where We Load In The Font Information From The File.
	// Of All The Places Where The Code Might Die, This Is The Most Likely,
	// As FT_New_Face Will Fail If The Font File Does Not Exist Or Is Somehow Broken.
	if (FT_New_Face( ft, floc.c_str(), 0, &font->face )) 
		return;
	
	// For Some Twisted Reason, FreeType Measures Font Size
	// In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
	// h Pixels High, We Need To Request A Size Of h*64.
	// (h << 6 Is Just A Prettier Way Of Writing h*64)
	FT_Set_Char_Size( font->face,  font->point * 64,  font->point * 64, 96, 96);
	
	bool use_kerning = FT_HAS_KERNING(  font->face );
	
	for(unsigned char c=0; c<128; c++)
	{
		// The First Thing We Do Is Get FreeType To Render Our Character
		// Into A Bitmap.  This Actually Requires A Couple Of FreeType Commands:
		
		// Load The Glyph For Our Character.
		if(FT_Load_Glyph( font->face, FT_Get_Char_Index( font->face, c ), FT_LOAD_DEFAULT ))
			return;
		
		// Move The Face's Glyph Into A Glyph Object.
		FT_Glyph glyph;
		if(FT_Get_Glyph( font->face->glyph, &glyph ))
			return;
		
		// Convert The Glyph To A Bitmap.
		FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		
		// This Reference Will Make Accessing The Bitmap Easier.
		FT_Bitmap& bitmap=bitmap_glyph->bitmap;
		
		// Use Our Helper Function To Get The Widths Of
		// The Bitmap Data That We Will Need In Order To Create
		// Our Texture.
		int downsample = 8;
		int width = next_p2( bitmap.width + 4 * downsample );
		int height = next_p2( bitmap.rows + 4 * downsample );
		
		float xs = (float)bitmap.width / width;
		float ys = (float)bitmap.rows / height;
		
		width /= downsample;
		height /= downsample;
		
		// Allocate Memory For The Texture Data.
		GLubyte* tex = (GLubyte*)malloc(width * height);
		memset(tex, 0, width*height);
		
		for(int i=0; i < width; i++)
		for(int j=0; j < height; j++)
		{
			tex[i + j*width]= edgeDistance(bitmap.buffer, bitmap.width, bitmap.rows, (i)/xs/width, (j)/ys/height, 1.0);
		}
		
		glGenTextures(1, &font->chars[c]);
		glBindTexture(GL_TEXTURE_2D, font->chars[c]);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex);
		
		for(int k=0; k<128; k++)
		{
			if(use_kerning)
			{
				FT_Vector kern;
				FT_Get_Kerning( font->face,         
							   FT_Get_Char_Index( font->face, c ),    
							   FT_Get_Char_Index( font->face, k ),        
							   FT_KERNING_DEFAULT, 
							   &kern );   
				
				font->kerning[c][k] = (font->face->glyph->advance.x + kern.x) / (font->point * 64.0f);
			}
			else
			{
				font->kerning[c][k] = font->face->glyph->advance.x / (font->point * 64.0f);
			}
		}
		
		font->wid[c] = bitmap.width / (float)font->point;
		font->hgt[c] = bitmap.rows / (float)font->point;
		
		font->xpad[c] = xs;
		font->ypad[c] = ys;
		
		free(tex);
	}
	
	FT_Done_Face(font->face);
	FT_Done_FreeType(ft);
}
