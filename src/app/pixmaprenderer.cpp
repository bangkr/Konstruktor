// Konstruktor - An interactive LDraw modeler for KDE
// Copyright (c)2006-2008 Park "segfault" J. K. <mastermind@planetmono.org>

#include <cmath>
#include <iostream>

#include <libldr/metrics.h>
#include <renderer/mouse_rotation.h>

#include "viewport.h"

#include "pixmaprenderer.h"

KonstruktorPixmapRenderer::KonstruktorPixmapRenderer(int width, int height)
	: width_(width), height_(height), format_()
{
	set_stud_rendering_mode(line);
	set_base_color(ldraw::color(7));
	
	// Initialize up OpenGL context
	//format_.setDirectRendering(false);
	//format_.setDoubleBuffer(false);
	format_.setSampleBuffers(true);
	format_.setAlpha(true);
	
	buffer_ = new QGLPixelBuffer(width_, height_, format_);
	
	buffer_->makeCurrent();
	
	// Initialize GL
	setup();
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	
	glMatrixMode(GL_PROJECTION);
	glViewport(0, 0, width_, height_);
	
	buffer_->doneCurrent();
}

KonstruktorPixmapRenderer::~KonstruktorPixmapRenderer()
{
	delete buffer_;
}

void KonstruktorPixmapRenderer::setNewSize(int width, int height)
{
	width_ = width;
	height_ = height;
	
	delete buffer_;
	buffer_ = new QGLPixelBuffer(width_, height_, format_);
	
	buffer_->makeCurrent();
	glViewport(0, 0, width_, height_);
	buffer_->doneCurrent();
}

QPixmap KonstruktorPixmapRenderer::render(ldraw::model *m, bool crop)
{
	buffer_->makeCurrent();
	
	//glDrawBuffer(GL_FRONT);
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	KonstruktorViewport dvp;
	if (m) {
		// Setup viewport
		// TODO reuse code

		const ldraw::metrics *metric;
		ldraw::metrics metricp(const_cast<ldraw::model *>(m));
		metric = m->custom_data<ldraw::metrics>();
		if (!metric) {
			metricp.update();
			metric = &metricp;
		}
		const ldraw::vector &min = metric->min();
		const ldraw::vector &max = metric->max();
		
		KonstruktorViewport vp;
		vp.left   = 1e30;
		vp.right  = -1e30;
		vp.top    = 1e30;
		vp.bottom = -1e30;
		
		ldraw::vector transformedVec[8];
		transformedVec[0] = mouse_rotation::isometric_projection_matrix * min;
		transformedVec[1] = mouse_rotation::isometric_projection_matrix * ldraw::vector(min.x(), min.y(), max.z());
		transformedVec[2] = mouse_rotation::isometric_projection_matrix * ldraw::vector(min.x(), max.y(), min.z());
		transformedVec[3] = mouse_rotation::isometric_projection_matrix * ldraw::vector(min.x(), max.y(), max.z());
		transformedVec[4] = mouse_rotation::isometric_projection_matrix * ldraw::vector(max.x(), min.y(), min.z());
		transformedVec[5] = mouse_rotation::isometric_projection_matrix * ldraw::vector(max.x(), min.y(), max.z());
		transformedVec[6] = mouse_rotation::isometric_projection_matrix * ldraw::vector(max.x(), max.y(), min.z());
		transformedVec[7] = mouse_rotation::isometric_projection_matrix * max;
		
		for (int i = 0; i < 8; i++) {
			if (transformedVec[i].x() < vp.left)
				vp.left = transformedVec[i].x();
			if (transformedVec[i].x() > vp.right)
				vp.right = transformedVec[i].x();
			if (transformedVec[i].y() > vp.bottom)
				vp.bottom = transformedVec[i].y();
			if (transformedVec[i].y() < vp.top)
				vp.top = transformedVec[i].y();
		}
		
		float fxlen = std::fabs(vp.right  - vp.left)*0.5f; 
		float fylen = std::fabs(vp.bottom - vp.top )*0.5f;
		
		vp.left   -= fxlen*0.1f;
		vp.right  += fxlen*0.1f;
		vp.top    -= fylen*0.1f;
		vp.bottom += fylen*0.1f;
		vp.aspectRatio = fxlen/fylen;
		dvp = vp;
		
		float median, d;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (std::fabs(vp.bottom-vp.top)*((float)width_/height_) >= std::fabs(vp.right-vp.left)) {
			median = (vp.right + vp.left) * 0.5f;
			d = std::fabs((vp.right-median)*((float)width_/height_))/vp.aspectRatio;
			vp.left = median - d;
			vp.right = median + d;
			glOrtho(vp.left, vp.right, vp.bottom, vp.top, 10000.0f, -10000.0f);
		} else {
			median = (vp.top + vp.bottom) * 0.5f;
			d = std::fabs((vp.bottom-median)/((float)width_/(float)height_))*vp.aspectRatio;
			vp.bottom = median + d;
			vp.top = median - d;
			glOrtho(vp.left, vp.right, vp.bottom, vp.top, 10000.0f, -10000.0f);
		}
		
		// Draw to pixbuf
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glLoadMatrixf(mouse_rotation::isometric_projection_matrix.transpose().get_pointer());
		
		draw_model_full(m->parent(), m, 0, std::set<int>());
	} else {
		crop = false;
	}
	
	glFlush();
	
	buffer_->doneCurrent();
	
	if (crop) {
		int w, h;
		float xl = std::fabs(dvp.right-dvp.left);
		float yl = std::fabs(dvp.bottom-dvp.top);
		
		if (xl >= yl) {
			w = width_;
			h = (int)(width_ * (yl/xl));
		} else {
			h = height_;
			w = (int)(height_ * (xl/yl));
		}
		
		return QPixmap::fromImage(buffer_->toImage().copy(width_/2 - w/2, height_/2 - h/2, w, h));
	} else {
		return QPixmap::fromImage(buffer_->toImage());
	}
}
