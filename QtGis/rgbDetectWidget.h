#pragma once
#include "UAVLocWidget.h"
#include <qdialog.h>
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
#include <qdialogbuttonbox.h>
#include <QSpacerItem>


class rgbDetectWidget :
    public QDialog
{
    Q_OBJECT
public:
    explicit rgbDetectWidget(QWidget* parent = nullptr);

private:
    QTextEdit* input_imgedit;
    QPushButton* input_img_button;

    QTextEdit* input_modeledit;
    QPushButton* input_model_button;

    QTextEdit* output_imgedit;
    QPushButton* output_img_button;
    
};

