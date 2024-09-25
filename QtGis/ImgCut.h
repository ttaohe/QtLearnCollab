#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QComboBox>
#include <QProgressBar>
#include <QGroupBox>

#include "qgslayerTreeLayer.h"
#include "qgslayertreeview.h"
#include "qgslayertreemodel.h"
class ImgCutWidget : public QWidget
{
	Q_OBJECT  // ���ඨ���м��� Q_OBJECT ��
public:
	explicit ImgCutWidget(QWidget* parent = nullptr);
	void public_updateLayerComboBox(QgsLayerTreeView* treeView) { return updateLayerComboBox(treeView); };

private slots:
	//void cropImage(const char* inputPath, const char* outputPath, double minx, double maxy, double maxx, double miny, const char* resamplingMethod);
	void updateLayerComboBox(QgsLayerTreeView* treeView);
	void onLayerSelected(int index);

private:

	QComboBox* inputLayerComboBox;
	QTextEdit* nXEdit;
	QTextEdit* nYEdit;
	QTextEdit* nXoffEdit;
	QTextEdit* nYoffEdit;

	QPushButton* runButton;
	QTextEdit* currentitmPath;
};

