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
#include <QDoubleSpinBox>
#include <QSlider>
#include "qgslayerTreeLayer.h"
#include "qgslayertreeview.h"
#include "qgslayertreemodel.h"
class ImgZengqiangWidget :
    public QWidget
{
    Q_OBJECT
public:
    explicit ImgZengqiangWidget(QWidget* parent = nullptr);
    void public_updateLayerComboBox(QgsLayerTreeView* treeView) { return updateLayerComboBox(treeView); };
private slots:
    void updateLayerComboBox(QgsLayerTreeView* treeView);
    void onLayerSelected(int index);

private:
    QComboBox* inputLayerComboBox;
    QTextEdit* currentitmPath;

    QComboBox* MethodComboBox;

    QTextEdit* clipEdit;
    QTextEdit* gridsizeEdit;

    QPushButton* runButton;

    QSlider* brightnessSlider;
    QSlider* gammaSlider;
    QSlider* saturationSlider;
    QSlider* contrastSlider;

    QPushButton* output_img_button;
    QTextEdit* output_imgedit;


};

