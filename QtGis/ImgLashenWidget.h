#pragma once
#include <qwidget.h>
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
class ImgLashenWidget :
    public QWidget
{
    Q_OBJECT
public:
    explicit ImgLashenWidget(QWidget* parent=nullptr);
    void public_updateLayerComboBox(QgsLayerTreeView* treeView) { return updateLayerComboBox(treeView); };
private slots:
    void updateLayerComboBox(QgsLayerTreeView* treeView);
    void onLayerSelected(int index);

private:
    QComboBox* inputLayerComboBox;
    QTextEdit* minEdit;
    QTextEdit* maxEdit;
    QTextEdit* normEdit;
    QTextEdit* percentEdit;

    QPushButton* runButton;
    QTextEdit* currentitmPath;

    QComboBox* MethodComboBox;



};

