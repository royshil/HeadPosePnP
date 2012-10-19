/*
 *  OGL_OCV_common.cpp
 *  Common interop between OpenCV and OpenGL
 *
 *  Created by Roy Shilkrot on 11/21/11.
 *  Copyright 2011 MIT. All rights reserved.
 *
 */

#include "OGL_OCV_common.h"

void drawOpenCVImageInGL(const OpenCVGLTexture& tex) {
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	int vPort[4]; glGetIntegerv(GL_VIEWPORT, vPort);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex.tex_id);
	glPushMatrix();
	glColor3ub(255, 255, 255);
	
	glScaled(vPort[3], vPort[3], 1);
	
	double aw2h = tex.aspect_w2h, ithr = tex.thr, itwr = tex.twr;
	double n[3] = {0,0,-1};
	
	GLint face_ori; glGetIntegerv(GL_FRONT_FACE, &face_ori);
	glFrontFace(GL_CW); //we're gonna draw clockwise
	
	glBegin(GL_QUADS);	
	
	glTexCoord2d(0, 0);
	glNormal3dv(n); 
	glVertex2d(0, 0);
	
	glTexCoord2d(0, ithr);
	glNormal3dv(n); 
	glVertex2d(0, 1); 
	
	glTexCoord2d(itwr, ithr);	
	glNormal3dv(n); 
	glVertex2d(aw2h, 1);
	
	glTexCoord2d(itwr, 0);				
	glNormal3dv(n); 
	glVertex2d(aw2h, 0); 
	
	glEnd();
	glPopMatrix();
	
	glFrontFace(face_ori); //restore front face orientation
	
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
}

void glEnable2D()
{
	int vPort[4];
	
	glGetIntegerv(GL_VIEWPORT, vPort);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	glOrtho(0, vPort[2], 0, vPort[3], -1, 4);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(0.375, 0.375, 0);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // clear the screen

	glDisable(GL_DEPTH_TEST);	
}

void glDisable2D()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();   
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	
	
	glEnable(GL_DEPTH_TEST);
}

#if defined(WIN32)
#define log2(x) log10((double)(x))/log10(2.0)
#endif

void copyImgToTex(const Mat& _tex_img, GLuint* texID, double* _twr, double* _thr) {
	Mat tex_img = _tex_img;
	flip(_tex_img,tex_img,0);
	Mat tex_pow2(pow(2.0,ceil(log2(tex_img.rows))),pow(2.0,ceil(log2(tex_img.cols))),CV_8UC3);
	std::cout << tex_pow2.rows <<"x"<<tex_pow2.cols<<std::endl;
	Mat region = tex_pow2(Rect(0,0,tex_img.cols,tex_img.rows));
	if (tex_img.type() == region.type()) {
		tex_img.copyTo(region);
	} else if (tex_img.type() == CV_8UC1) {
		cvtColor(tex_img, region, CV_GRAY2BGR);
	} else {
		tex_img.convertTo(region, CV_8UC3, 255.0);
	}
	
	if (_twr != 0 && _thr != 0) {
		*_twr = (double)tex_img.cols/(double)tex_pow2.cols;
		*_thr = (double)tex_img.rows/(double)tex_pow2.rows;
	}
	glBindTexture( GL_TEXTURE_2D, *texID );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex_pow2.cols, tex_pow2.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, tex_pow2.data);
}	


OpenCVGLTexture MakeOpenCVGLTexture(const Mat& _tex_img) {
	OpenCVGLTexture _ocvgl;

	glGenTextures( 1, &_ocvgl.tex_id );
	glBindTexture( GL_TEXTURE_2D, _ocvgl.tex_id );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	if (!_tex_img.empty()) { //image may be dummy, just to generate pointer to texture object in GL
		copyImgToTex(_tex_img,&_ocvgl.tex_id,&_ocvgl.twr,&_ocvgl.thr);
		_ocvgl.aspect_w2h = (double)_tex_img.cols/(double)_tex_img.rows;
	}
	
	return _ocvgl;
}