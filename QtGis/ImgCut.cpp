#include "ImgCut.h"
#include "qmessagebox.h"

#include "gdal_priv.h"
#include "cpl_conv.h"
#include "gdal.h"
#include "gdalwarper.h"

ImgCutWidget::ImgCutWidget(QWidget* parent)
    : QWidget(parent)
{

    setWindowTitle(QStringLiteral("Ӱ��ü�"));
    setWindowIcon(QIcon(":/QtGis/images/res/module.png"));


    inputLayerComboBox = new QComboBox();
    inputLayerComboBox->setPlaceholderText("Select input layer");

    currentitmPath = new QTextEdit();
    currentitmPath->setFixedHeight(30); // Set a fixed height for additionalParamsEdit

    QHBoxLayout* input_layer_hlayout = new QHBoxLayout;
    input_layer_hlayout->addWidget(inputLayerComboBox);
    input_layer_hlayout->addWidget(currentitmPath);
    QGroupBox* input_layer_select = new QGroupBox(QStringLiteral("ͼ��ѡ��"));
    input_layer_select->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    input_layer_select->setLayout(input_layer_hlayout);


    QVBoxLayout* XY_vlayout = new QVBoxLayout;
    nXEdit = new QTextEdit();
    nYEdit = new QTextEdit();
    QHBoxLayout* X_hlayout = new QHBoxLayout;
    X_hlayout->addWidget(new QLabel("X: "));
    X_hlayout->addWidget(nXEdit);

    QHBoxLayout* Y_hlayout = new QHBoxLayout;
    Y_hlayout->addWidget(new QLabel("Y: "));
    Y_hlayout->addWidget(nYEdit);

    nXEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit
    nYEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit
    XY_vlayout->addLayout(X_hlayout);
    XY_vlayout->addLayout(Y_hlayout);


    QVBoxLayout* XYoff_vlayout = new QVBoxLayout;
    nXoffEdit = new QTextEdit();
    nYoffEdit = new QTextEdit();
    QHBoxLayout* Xoff_hlayout = new QHBoxLayout;
    Xoff_hlayout->addWidget(new QLabel("X_off: "));
    Xoff_hlayout->addWidget(nXoffEdit);

    QHBoxLayout* Yoff_hlayout = new QHBoxLayout;
    Yoff_hlayout->addWidget(new QLabel("Y_off: "));
    Yoff_hlayout->addWidget(nYoffEdit);

    nXoffEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit
    nYoffEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit
    XYoff_vlayout->addLayout(Xoff_hlayout);
    XYoff_vlayout->addLayout(Yoff_hlayout);

    QHBoxLayout* zuobiao_vlayout = new QHBoxLayout;
    zuobiao_vlayout->addLayout(XY_vlayout);
    zuobiao_vlayout->addLayout(XYoff_vlayout);
    QGroupBox* zuobiao_box = new QGroupBox(QStringLiteral("�ü���������"));
    zuobiao_box->setLayout(zuobiao_vlayout);
    zuobiao_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    runButton = new QPushButton(QStringLiteral("����"));
    

    QBoxLayout* pBottomBtnBoxLayout = new QHBoxLayout;
    pBottomBtnBoxLayout->addWidget(runButton);

    QVBoxLayout* pBodylayout = new QVBoxLayout;
    pBodylayout->addWidget(input_layer_select);
    pBodylayout->addWidget(zuobiao_box);

    QVBoxLayout* pStretchlayout = new QVBoxLayout;
    QSpacerItem* sparcer_item = new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    pStretchlayout->addItem(sparcer_item);
    QBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pBodylayout);
    pMainLayout->addLayout(pStretchlayout);
    pMainLayout->addLayout(pBottomBtnBoxLayout);

    this->setLayout(pMainLayout);

    connect(inputLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerSelected(int)));
}

void ImgCutWidget::updateLayerComboBox(QgsLayerTreeView* treeView)
{
    QgsLayerTreeModel* model = treeView->layerTreeModel();

    inputLayerComboBox->clear();
    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex index = model->index(i, 0);
        QgsLayerTreeNode* node = static_cast<QgsLayerTreeNode*>(index.internalPointer());
        QgsLayerTreeLayer* layerNode = dynamic_cast<QgsLayerTreeLayer*>(node);
        if (layerNode) {
            inputLayerComboBox->addItem(QIcon(":/QtGis/images/res/layer.png"), layerNode->name(), layerNode->layer()->dataProvider()->dataSourceUri());
        }
    }
}

void ImgCutWidget::onLayerSelected(int index)
{
    QString layerId = inputLayerComboBox->itemData(index).toString();
    // ��������ͼ��༭����ı�Ϊͼ���Դ·��
    currentitmPath->setText(layerId);
}

//void ImgCut::cropImage(const char* inputPath, const char* outputPath, double minx, double maxy, double maxx, double miny, const char* resamplingMethod) {
//    GDALAllRegister(); // ע�����е� GDAL ����
//
//    // ������Ӱ��
//    GDALDataset* poDS = static_cast<GDALDataset*>(GDALOpenEx(inputPath, GDAL_OF_RASTER, nullptr, nullptr, nullptr));
//    if (poDS == nullptr) {
//        QMessageBox::critical(this, "Error", "Failed to open dataset: " + QString::fromUtf8(inputPath));
//        return;
//    }
//
//    // �������Ӱ��
//    GDALDriver* poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
//    GDALDataset* poODS = poDriver->CreateCopy(outputPath, poDS, 0);
//    if (poODS == nullptr) {
//        QMessageBox::critical(this, "Error", "Failed to create output dataset: " + QString::fromUtf8(outputPath));
//        GDALClose(poDS);
//        return;
//    }
//
//    // ���òü�����
//    double adfGeoTransform[6] = { minx, 0, 0, maxy, 0, -1 };
//    poODS->SetGeoTransform(adfGeoTransform);
//
//    // ȷ������任�����ѳɹ���ȡ
//    if (adfGeoTransform[1] != 0 && adfGeoTransform[5] != 0) {
//        // ����ü�����Ĵ�С
//        int nXSize = static_cast<int>((maxx - minx) / adfGeoTransform[1]) + 1;
//        int nYSize = static_cast<int>((maxy - miny) / adfGeoTransform[5]) + 1;
//
//        // ���� nXSize �� nYSize �����˲ü�����Ŀ�Ⱥ͸߶�
//        // ���������Щֵ�����òü�����...
//    }
//
//    // ���òü������ͶӰ
//    const char* pszProjection = poDS->GetProjectionRef();
//    poODS->SetProjection(pszProjection);
//
//    // Ϊÿ���������òü�����
//    for (int i = 1; i <= poDS->GetRasterCount(); ++i) {
//        GDALRasterBand* poBandSrc = poDS->GetRasterBand(i);
//        GDALRasterBand* poBandDst = poODS->GetRasterBand(i);
//
//        // ʹ���ز����������вü�
//        GDALResampleAlg resampleAlg = GRA_NearestNeighbour;
//        if (strcmp(resamplingMethod, "AVERAGE") == 0) {
//            resampleAlg = GRA_Average;
//        }
//        else if (strcmp(resamplingMethod, "BILINEAR") == 0) {
//            resampleAlg = GRA_Bilinear;
//        }
//        else if (strcmp(resamplingMethod, "CUBIC") == 0) {
//            resampleAlg = GRA_Cubic;
//        }
//        else if (strcmp(resamplingMethod, "CUBICSPLINE") == 0) {
//            resampleAlg = GRA_CubicSpline;
//        }
//        else if (strcmp(resamplingMethod, "LANCZOS") == 0) {
//            resampleAlg = GRA_Lanczos;
//        }
//
//        GDALRasterBandCopyWholeRaster(poBandSrc, poBandDst, resampleAlg, 0, 0, nXSize, nYSize);
//    }
//
//    // �ر����ݼ�
//    GDALClose(poODS);
//    GDALClose(poDS);
//
//    QMessageBox::information(this, "Success", "Overviews created successfully.");
//}
