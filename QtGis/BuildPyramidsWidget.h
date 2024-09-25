#ifndef BUILDPYRAMIDSWIDGET_H
#define BUILDPYRAMIDSWIDGET_H

#pragma once
#include <QWidget>
#include <qdialog.h>
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
#include <qdialogbuttonbox.h>

#include "qgslayerTreeLayer.h"
#include "qgslayertreeview.h"
#include "qgslayertreemodel.h"

class BuildPyramidsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BuildPyramidsWidget(QWidget* parent = nullptr);
    void public_updateLayerComboBox(QgsLayerTreeView* treeView) { return updateLayerComboBox(treeView); };

private slots:
    void onRunClicked();
    void onLayerSelected(int index);
    void updateLayerComboBox(QgsLayerTreeView* treeView);

private:
    QCheckBox* removeAllCheckBox;
    QComboBox* inputLayerComboBox;
    QComboBox* ResamplingmethodComboBox;
    QTextEdit* OverviewListEdit;
    QTextEdit* additionalParamsEdit;
    QPushButton* runButton;
    QProgressBar* progressBar;

    QTextEdit* currentitmPath;
    QTextEdit* gdalcmd;

    // ´´½¨ QProcess ÊµÀý
    QProcess* mGdalProcess;
};

#endif // BUILDPYRAMIDSWIDGET_H