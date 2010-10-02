// Konstruktor - An interactive LDraw modeler for KDE
// Copyright (c)2006-2008 Park "segfault" J. K. <mastermind@planetmono.org>

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <string>

#include <QList>
#include <QSet>
#include <QPair>
#include <QVector>

#include <kxmlguiwindow.h>

namespace ldraw 
{
class model;
}

class KonstruktorContentsModel;
class KonstruktorContentsView;
class KonstruktorDocument;
class KonstruktorEditor;
class KonstruktorPartsWidget;
class KonstruktorRenderWidget;

class QCloseEvent;
class QModelIndex;
class QTreeView;

class KAction;
class KRecentFilesAction;
class KSelectAction;
class KTabBar;
class KUrl;

class KonstruktorMainWindow : public KXmlGuiWindow
{
	Q_OBJECT;
	
  public:
	KonstruktorMainWindow(QWidget *parent = 0L);
	~KonstruktorMainWindow();

	unsigned int viewportModes() const;
	const KonstruktorEditor* editor() const { return editorGroup_; }
	
  signals:
	void activeModelChanged(ldraw::model *m);
	void viewChanged();
									  
  public slots:
	void modelModified(const QSet<int> &selection);
	void updateViewports();
	void changeCaption();
	void activate(bool b);
	
	void newFile();
	void openFile();
	void openFile(const KUrl &url);
	void closeFile();
	void saveFile();
	void saveFileAs();
	void quit();

	void newSubmodel();
	void deleteSubmodel();
	void modelProperties();

	void resetZoom();
	void resetDisplay();

	void render();

	void showConfigDialog();

  private slots:
	void activeDocumentChanged(int index);
	void activeModelChanged(const std::string &name);
	void submodelViewDoubleClicked(const QModelIndex &index);
	void selectionChanged(const QSet<int> &cset);
	void gridModeChanged(QAction *action);
	void modelModified();

  private:
	void closeEvent(QCloseEvent *event);
	
	void initGui();
	void initObjects();
	void initActions();
	void initConnections();
	bool confirmQuit();
	bool doSave(KonstruktorDocument *document, bool newname = false);
	
  private:
	/*
	 * Document management
	 */
	KonstruktorDocument *activeDocument_;
	QVector<QPair<QString, KonstruktorDocument *> > documents_;
	QSet<QString> openedUrls_;
	KonstruktorContentsModel *contentsModel_;
	KonstruktorEditor *editorGroup_;
	
	/*
	 * Widgets
	 */
	KonstruktorPartsWidget *partsWidget_;
	QTreeView *submodelList_;
	KonstruktorContentsView *contentList_;	
	KonstruktorRenderWidget *renderWidget_[4];
	KTabBar *tabbar_;

	/*
	 * State management
	 */
	bool enabled_;
	int newcnt_;

	/*
	 * Actions
	 */
	QList<QAction *> stateChangeableActions_;
	QList<QAction *> selectionDependentActions_;
	
	// File menu
	KAction *actionNew_;
	KAction *actionOpen_;
	KRecentFilesAction *actionOpenRecent_;
	KAction *actionClose_;
	KAction *actionSave_;
	KAction *actionSaveAs_;
	KAction *actionQuit_;

	// Edit menu
	QAction *actionUndo_;
	QAction *actionRedo_;
	KAction *actionSelectAll_;
	KAction *actionSelectNone_;
	KAction *actionHide_;
	KAction *actionUnhideAll_;
	KAction *actionColor_;
	KAction *actionGridSparse_;
	KAction *actionGridNormal_;
	KAction *actionGridDense_;
	KAction *actionGridNone_;
	KAction *actionDelete_;
	KAction *actionMoveByXPositive_;
	KAction *actionMoveByXNegative_;
	KAction *actionMoveByYPositive_;
	KAction *actionMoveByYNegative_;
	KAction *actionMoveByZPositive_;
	KAction *actionMoveByZNegative_;
	KAction *actionRotateByXClockwise_;
	KAction *actionRotateByXCounterClockwise_;
	KAction *actionRotateByYClockwise_;
	KAction *actionRotateByYCounterClockwise_;
	KAction *actionRotateByZClockwise_;
	KAction *actionRotateByZCounterClockwise_;

	// View menu
	KAction *actionResetZoom_;
	KAction *actionReset3dView_;

	// Submodel menu
	KAction *actionNewSubmodel_;
	KAction *actionDeleteSubmodel_;
	KAction *actionCollapseSubmodel_;
	KAction *actionModelProperties_;

	// Render menu
	KAction *actionRender_;
	KAction *actionRenderSteps_;

	// Setting menu
	KAction *actionConfigure_;
};

#endif
