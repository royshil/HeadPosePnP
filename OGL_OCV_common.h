/*
 *  OGL_OCV_common.h
 *  HeadReplacement
 *
 *  Created by Roy Shilkrot on 11/21/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include <opencv2/opencv.hpp>

using namespace cv;

#if defined(__APPLE__)
#  include <OpenGL/gl.h>
#elif defined(__linux__) || defined(__MINGW32__) || defined(WIN32)
#  define GLEW_STATIC
//#  include <GL/glew.h>
//#  include <GL/wglew.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
//#  include <GL/glext.h>
#else
#  include <gl.h>
#endif

void copyImgToTex(const Mat& _tex_img, GLuint* texID, double* _twr, double* _thr);

typedef struct my_texture {
	GLuint tex_id;
	double twr,thr,aspect_w2h;
	Mat image;
	my_texture():tex_id(-1),twr(1.0),thr(1.0) {}
	bool initialized;
	void set(const Mat& ocvimg) { 
		ocvimg.copyTo(image); 
		copyImgToTex(image, &tex_id, &twr, &thr); 
		aspect_w2h = (double)ocvimg.cols/(double)ocvimg.rows;
	}
} OpenCVGLTexture;

void glEnable2D();	// setup 2D drawing
void glDisable2D(); // end 2D drawing
OpenCVGLTexture MakeOpenCVGLTexture(const Mat& _tex_img); // create an OpenCV-OpenGL image
void drawOpenCVImageInGL(const OpenCVGLTexture& tex); // draw an OpenCV-OpenGL image
