
#include <stdio.h>
#include <string>
#include <GL/GL.h>
#include <GL/GLU.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <stdlib.h>

#ifndef IMAGE_H
#define IMAGE_H

struct RGB
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;

};

class Image
{
private:
	RGB pixels[2048][2048];
	int width;
	int height;

public:
	Image(std::string);
	void display(int, int);
};
#endif // IMAGE
