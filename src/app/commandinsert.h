// Konstruktor - An interactive LDraw modeler for KDE
// Copyright (c)2006-2008 Park "segfault" J. K. <mastermind@planetmono.org>

#ifndef _COMMANDINSERT_H_
#define _COMMANDINSERT_H_

#include <libldr/color.h>
#include <libldr/math.h>

#include "commandbase.h"

class KonstruktorCommandInsert : public KonstruktorCommandBase
{
  public:
	KonstruktorCommandInsert(const QString &filename, const ldraw::matrix &matrix, const ldraw::color &color, const QSet<int> &selection, ldraw::model *model);
	~KonstruktorCommandInsert();

	bool needUpdateDimension() const;
	QPair<KonstruktorCommandBase::AffectedRow, QSet<int> > affectedRows() const;

	void redo();
	void undo();

  private:
	QString filename_;
	ldraw::matrix matrix_;
	ldraw::color color_;
	int offset_;
};	

#endif
