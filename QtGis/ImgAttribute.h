#pragma once
#include <qwidget.h>
#include <qplaintextedit.h>
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QStandardItem>
class ImgAttribute :
    public QWidget
{
    Q_OBJECT
public:
    ImgAttribute(QWidget* parent = nullptr);
    ~ImgAttribute() { }

    //QPlainTextEdit* GetTextWidget() { return m_ptextWidget; }

/*Q_SIGNALS:
    void sendData(QString qstr);*/

public slots:
    void PrintData(QString qstr);
    char* findImageTypeGDAL(char* pImgFileName);
private:
    //QPlainTextEdit* m_ptextWidget = nullptr;
    //QLabel* pathLabel;
    //QLabel* formatLabel;
    //QLabel* typeLabel;
    //QLabel* widthLabel;
    //QLabel* heightLabel;
    //QLabel* bandLabel;
    QStandardItemModel* model = nullptr;
    QTableView* table_view = nullptr;
    QVBoxLayout* layout = nullptr;
};

