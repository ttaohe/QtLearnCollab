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


class JinjingLocWidget :
    public QDialog
{
    Q_OBJECT
public:
    explicit JinjingLocWidget(QWidget* parent = nullptr);

private:
    QGroupBox* input_uav_groupbox;
    QTextEdit* input_uavimgedit;
    QPushButton* input_uav_button;
    QGroupBox* input_base_groupbox;
    QTextEdit* input_baseimgedit;
    QPushButton* input_base_button;
    QComboBox* configcombox;
    QTextEdit* input_querylocedit;
    QTextEdit* input_configedit;
};

