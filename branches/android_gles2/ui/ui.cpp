/*
Portable ZX-Spectrum emulator.
Copyright (C) 2001-2010 SMT, Dexus, Alone Coder, deathsoft, djdron, scor

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "../std.h"
#include "ui.h"
#include "../platform/io.h"

#ifdef USE_UI

#ifdef USE_EMBEDDED_RESOURCES
#include "res/font/spxtrm4f.h"
#endif//USE_EMBEDDED_RESOURCES

#ifdef USE_EXTERN_RESOURCES
extern byte spxtrm4f[];
#endif//USE_EXTERN_RESOURCES

namespace xUi
{

#ifdef UI_REAL_ALPHA

eRGBAColor palette[] = { 0x00000000, 0xffffffff, 0xffb06000, 0x80202020, 0xff008000, 0xff0000b0, 0xff800080 };

#else//UI_REAL_ALPHA

eRGBAColor palette[] = { 0x00000000, 0x08ffffff, 0x08b06000, 0x01202020, 0x08008000, 0x080000b0, 0x08800080 };

#endif//UI_REAL_ALPHA

byte screen[WIDTH*HEIGHT];

static struct eInit
{
	eInit() { memset(screen, 0, sizeof(screen)); }
}init;

inline void DrawPixel(const ePoint& p, ePalettedColor c)
{
	screen[p.y*WIDTH + p.x] = c;
}
inline ePalettedColor GetPixel(const ePoint& p)
{
	return (ePalettedColor)screen[p.y*WIDTH + p.x];
}

void DrawRect(const eRect& r, ePalettedColor c)
{
	ePoint p;
	for(p.y = r.top; p.y < r.bottom; ++p.y)
	{
		for(p.x = r.left; p.x < r.right; ++p.x)
		{
			DrawPixel(p, c);
		}
	}
}

void DrawRect(const eRect& r, ePalettedColor c, ePalettedColor key)
{
	ePoint p;
	for(p.y = r.top; p.y < r.bottom; ++p.y)
	{
		for(p.x = r.left; p.x < r.right; ++p.x)
		{
			if(GetPixel(p) != key)
			{
				DrawPixel(p, c);
			}
		}
	}
}

static class eFont
{
public:
	eFont() : w(0), h(0), data(NULL) {}
	~eFont()
	{
#if !defined(USE_EMBEDDED_RESOURCES) && !defined(USE_EXTERN_RESOURCES)
		SAFE_DELETE(data);
#endif//USE_EMBEDDED_RESOURCES
	}
	void Create(int _w, int _h, const char* fname)
	{
		assert(!data);
		w = _w; h = _h;
#if defined(USE_EMBEDDED_RESOURCES) || defined(USE_EXTERN_RESOURCES)
		data = spxtrm4f;
#else//USE_EMBEDDED_RESOURCES
		FILE* f = fopen(xIo::ResourcePath(fname), "rb");
		assert(f);
		size_t size = 8 * 256;
		data = new byte[size];
		size_t readed = fread(data, 1, size, f);
		assert(readed == size);
		fclose(f);
#endif//USE_EMBEDDED_RESOURCES
	}
	void Draw(int _char, const ePoint& p)
	{
		int pitch = 256;
		byte* src = data + _char + pitch*1;
		for(int y = 0; y < h; ++y)
		{
			for(int x = 0; x < w; ++x)
			{
				if(!((*src << x)&0x80))
					continue;
				DrawPixel(ePoint(p.x + x, p.y + y), COLOR_WHITE);
			}
			src += pitch;
		}
	}

public:
	int w, h;
	byte* data;
}font;

void DrawText(const eRect& r, const char* s)
{
	//	strupr(const_cast<char*>(s));
	ePoint p = r.Beg();
	for(int i = 0; s[i]; ++i)
	{
		if(p.x + font.w > r.right)
			break;
		char c = s[i];
		if(c >= 96)
			c -= 32;
		font.Draw(c, p);
		p.x += font.w;
	}
}

void _CreateFont(int _w, int _h, const char* fname) { font.Create(_w, _h, fname); }
ePoint FontSize() { return ePoint(font.w, font.h); }

}
//namespace xUi

#endif//USE_UI
