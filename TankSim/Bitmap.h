//
//  Bitmap.h
//  TankSim
//
//  Created by Vitaliy Krasylovets on 2014-11-15.
//  Copyright (c) 2014 Matthew Tesfaldet. All rights reserved.
//

#ifndef __TankSim__Bitmap__
#define __TankSim__Bitmap__

#include <stdio.h>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <gl/glut.h>
#endif

typedef unsigned char  byte;
typedef unsigned short word;
typedef unsigned long  dword;
typedef unsigned short ushort;
typedef unsigned long  ulong;

typedef struct RGB
{
    byte r,g,b;
} RGB;

typedef struct RGBpixmap
{
    int nRows, nCols;
    RGB *pixel;
} RGBpixmap;

void fskip(FILE *fp, int num_bytes);
ushort getShort(FILE *fp);
ulong getLong(FILE *fp);
void readBMPFile(RGBpixmap *pm, char *file);
void setTexture(RGBpixmap *p, GLuint textureID);

#endif /* defined(__TankSim__Bitmap__) */
