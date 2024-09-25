#include "ImgAttribute.h"
#include <qboxlayout.h>
#include "gdal.h"
#include "gdal_priv.h"
#include "gdalwarper.h"
#include <QTextCodec>
#include <locale>
using std::string;
ImgAttribute::ImgAttribute(QWidget* parent) :QWidget(parent)
{

	//// ������ͼ����
	layout = new QVBoxLayout;
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);
	table_view = new QTableView();
	
	// ����ˮƽ��ͷ�ʹ�ֱ��ͷ�����ɼ�
	table_view->horizontalHeader()->setVisible(false);
	table_view->verticalHeader()->setVisible(false);

	layout->addWidget(table_view, 1);

	this->setLayout(layout);
	this->setStyleSheet("background-color: white;");

}

char* ImgAttribute::findImageTypeGDAL(char* pImgFileName) //��ȡӰ���ʽ����
{
	char* Gtype = NULL;
	char fileExtension[_MAX_PATH] = { NULL };
	strcpy_s(fileExtension, pImgFileName);
	if ((strstr(fileExtension, ".tif") != NULL))  Gtype = const_cast <char*>("GTiff");
	else if ((strstr(fileExtension, ".tiff") != NULL)) Gtype = const_cast <char*>("GTiff");
	else if ((strstr(fileExtension, ".jpg") != NULL))  Gtype = const_cast <char*>("JPEG");//Create()������֧��JPEG��PNG�ȸ�ʽ
	else if ((strstr(fileExtension, ".bmp") != NULL))  Gtype = const_cast <char*>("BMP");
	else if ((strstr(fileExtension, ".png") != NULL))  Gtype = const_cast <char*>("PNG");
	else if ((strstr(fileExtension, ".gif") != NULL))  Gtype = const_cast <char*>("GIF");
	else Gtype = NULL;
	return Gtype;
}

void ImgAttribute::PrintData(QString qstr)
{
	// ���û���ʹ��UTF-8����
	std::locale::global(std::locale(""));
	//ע�ᡢ��ȡͼ��
	GDALAllRegister();
	//CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  // windowsϵͳ�ļ���Ӧ��Ĭ����UTF-8����
	const char* imgPathSrc = qstr.toUtf8().constData();
	GDALDatasetH pInDataset = GDALOpenEx(imgPathSrc, GDAL_OF_READONLY | GDAL_OF_SHARED, nullptr, nullptr, nullptr);

	GDALDataType imageType = GDALGetRasterDataType(GDALGetRasterBand(pInDataset, 1));
	const char* gdt = GDALGetDataTypeName(imageType);
	//��ȡԭӰ���ʽ����
	char* GType = NULL;
	char* imgPathSrc2 = const_cast<char*>(imgPathSrc);

	GType = findImageTypeGDAL(imgPathSrc2);

	//��ȡͼ�����ݵĲ�������ʾ
	int nImgWidth = GDALGetRasterXSize(pInDataset);
	int nImgHeight = GDALGetRasterYSize(pInDataset);
	int nImgBandNum = GDALGetRasterCount(pInDataset);

	QString nImgWidth2 = QString::number(nImgWidth);
	QString nImgHeight2 = QString::number(nImgHeight);
	QString nImgBandNum2 = QString::number(nImgBandNum);


	// �������Ը���
	model = new QStandardItemModel();
	model->setColumnCount(2);
	QStandardItem* item0 = new QStandardItem(QStringLiteral("Ӱ��·��"));
	QStandardItem* item1 = new QStandardItem(QStringLiteral("Ӱ���ʽ����"));
	QStandardItem* item2 = new QStandardItem(QStringLiteral("Ӱ����������"));
	QStandardItem* item3 = new QStandardItem(QStringLiteral("Ӱ����"));
	QStandardItem* item4 = new QStandardItem(QStringLiteral("Ӱ��߶�"));
	QStandardItem* item5 = new QStandardItem(QStringLiteral("Ӱ�񲨶���"));

	QStandardItem* item6 = new QStandardItem(qstr);
	QStandardItem* item7 = new QStandardItem(GType);
	QStandardItem* item8 = new QStandardItem(gdt);
	QStandardItem* item9 = new QStandardItem(nImgWidth2);
	QStandardItem* item10 = new QStandardItem(nImgHeight2);
	QStandardItem* item11 = new QStandardItem(nImgBandNum2);

	model->setItem(0, 0, item0);
	model->setItem(1, 0, item1);
	model->setItem(2, 0, item2);
	model->setItem(3, 0, item3);
	model->setItem(4, 0, item4);
	model->setItem(5, 0, item5);

	model->setItem(0, 1, item6);
	model->setItem(1, 1, item7);
	model->setItem(2, 1, item8);
	model->setItem(3, 1, item9);
	model->setItem(4, 1, item10);
	model->setItem(5, 1, item11);

	table_view->setModel(model);
	// ���õ�һ���п�̶�Ϊ100
	table_view->setColumnWidth(1, 100);
	// ���õڶ��п���stretch��������������
	table_view->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	

}
