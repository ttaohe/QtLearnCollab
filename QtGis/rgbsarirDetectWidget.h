#pragma once
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


class rgbsarirDetectWidget :
    public QDialog
{
    Q_OBJECT
public:
    explicit rgbsarirDetectWidget(QWidget* parent = nullptr);

private:
    QTextEdit* input_rgbimgedit;
    QPushButton* input_rgb_button;
    QTextEdit* input_sarimgedit;
    QPushButton* input_sar_button;
    QTextEdit* input_irimgedit;
    QPushButton* input_ir_button;


    QTextEdit* input_modeledit;
    QPushButton* input_model_button;

    QTextEdit* output_imgedit;
    QPushButton* output_img_button;
};





