#include "BuildPyramidsWidget.h"
#include <QWidget>

#include "qgslayerTreeLayer.h"
#include "qgslayertreeview.h"
#include "qgslayertreemodel.h"

#include <cstdlib>
#include "gdal_priv.h"
#include "cpl_string.h"
#include "qmessagebox.h"
BuildPyramidsWidget::BuildPyramidsWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle(QStringLiteral("Ӱ�������"));
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
    
    //QHBoxLayout* removeAllCheckBox_hlayout = new QHBoxLayout;
    removeAllCheckBox = new QCheckBox("Remove all");
    //removeAllCheckBox_hlayout->addWidget(removeAllCheckBox);

    ResamplingmethodComboBox = new QComboBox();
    ResamplingmethodComboBox->addItems({ "AVERAGE", "BILINEAR", "CUBIC", "GAUSS", "LANCZOS", "AVERAGE_MAGPHASE", "CUBICSPLINE", "MODE" });

    OverviewListEdit = new QTextEdit();
    OverviewListEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit

    additionalParamsEdit = new QTextEdit();
    additionalParamsEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit
    QVBoxLayout* advanced_params_vlayout = new QVBoxLayout;
    advanced_params_vlayout->addWidget(new QLabel(QStringLiteral("�ز�������ѡ��")));
    advanced_params_vlayout->addWidget(ResamplingmethodComboBox);
    advanced_params_vlayout->addWidget(new QLabel(QStringLiteral("�������㼶����")));
    advanced_params_vlayout->addWidget(OverviewListEdit);
    advanced_params_vlayout->addWidget(new QLabel(QStringLiteral("�����������")));
    advanced_params_vlayout->addWidget(additionalParamsEdit);
    QGroupBox* advanced_params_box = new QGroupBox(QStringLiteral("��������"));
    advanced_params_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    advanced_params_box->setLayout(advanced_params_vlayout);

    //QDialogButtonBox* pbtnboxStandard = new QDialogButtonBox(QDialogButtonBox::Ok);
    //connect(pbtnboxStandard, SIGNAL(accepted()), this, SLOT(accepted()));

    //connect(pbtnboxStandard, SIGNAL(clicked(QAbstractButton*)), this, &BuildPyramidsWidget::onRunClicked);
    runButton = new QPushButton(QStringLiteral("����"));
    

    QBoxLayout* pBottomBtnBoxLayout = new QHBoxLayout;
    pBottomBtnBoxLayout->addWidget(runButton);

    QVBoxLayout* pBodylayout = new QVBoxLayout;
    pBodylayout->addWidget(input_layer_select);
    pBodylayout->addWidget(advanced_params_box);
    
    QVBoxLayout* pStretchlayout = new QVBoxLayout;
    QSpacerItem* sparcer_item = new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    pStretchlayout->addItem(sparcer_item);
    QBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pBodylayout);
    pMainLayout->addLayout(pStretchlayout);
    pMainLayout->addLayout(pBottomBtnBoxLayout);

    this->setLayout(pMainLayout);

    connect(runButton, &QPushButton::clicked, this, &BuildPyramidsWidget::onRunClicked);
    connect(inputLayerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onLayerSelected(int)));

    mGdalProcess = new QProcess(this);

    //const char* key = "PROJ_LIB";
    //const char* value = "C:/Users/hetao/Desktop/hjj_back/pack/release-1928-x64-gdal-3-9-1-mapserver-8-2-0/bin/";
    //_putenv_s(key, value);
}


void BuildPyramidsWidget::updateLayerComboBox(QgsLayerTreeView* treeView)
{
    QgsLayerTreeModel* model = treeView->layerTreeModel();

    inputLayerComboBox->clear();
    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex index = model->index(i, 0);
        QgsLayerTreeNode* node = static_cast<QgsLayerTreeNode*>(index.internalPointer());
        QgsLayerTreeLayer* layerNode = dynamic_cast<QgsLayerTreeLayer*>(node);
        if (layerNode) {
            inputLayerComboBox->addItem(QIcon(":/QtGis/images/res/layer.png"),layerNode->name(), layerNode->layer()->dataProvider()->dataSourceUri());
        }
    }
}



void BuildPyramidsWidget::onLayerSelected(int index)
{
    QString layerId = inputLayerComboBox->itemData(index).toString();
    // ��������ͼ��༭����ı�Ϊͼ���Դ·��
    currentitmPath->setText(layerId);
}

void BuildPyramidsWidget::onRunClicked()
{
    QString inputLayer = currentitmPath->toPlainText();
    QString removeAll = removeAllCheckBox->isChecked() ? " -r " : "";
    QString advancedParams = additionalParamsEdit->toPlainText();

    // ��ȡ������
    QByteArray resamplingMethod = ResamplingmethodComboBox->currentText().toUtf8();
    const char* pszResampling = resamplingMethod.constData();

    // ��ȡ���еĲ�����
    QString input = OverviewListEdit->toPlainText();  // ��ȡ�ı��༭���е��ı�
    QStringList stringList = input.split(" ", QString::SkipEmptyParts);  // �ָ��ַ���
    // ���ַ����б�ת��Ϊ�����б�
    QVector<int> overviewList;
    for (const QString& str : stringList) {
        bool ok;
        int value = str.toInt(&ok);
        if (ok) {
            overviewList.append(value);
        }
        else {
            QMessageBox::critical(this, "Error", "Invalid input:" + str);
            return;
        }
    }
    // �� QVector ת��Ϊ����
    int nOverviews = overviewList.size();
    int* anOverviewList = new int[nOverviews];
    for (int i = 0; i < nOverviews; ++i) {
        anOverviewList[i] = overviewList[i];
    }


    //QString command = "gdaladdo " + inputLayer + " " + removeAll + " " + Resampling + " " + advancedParams;
    //gdalcmd->setText(command);


    const char* imgPathSrc = inputLayer.toUtf8().constData();
    GDALDataset* poDS = static_cast<GDALDataset*>(GDALOpenEx(imgPathSrc, GDAL_OF_RASTER, nullptr, nullptr, nullptr));

    if (poDS == nullptr) {
        QMessageBox::critical(this, "Error", "Failed to open dataset: " + inputLayer);
        return;
    }


    
    //int anOverviewList[] = { 2, 4, 8 }; // �������㼶
    /*int nOverviews = 3;*/
    int nBands = poDS->GetRasterCount(); // ��ȡ��������
    int* panBandList = new int[nBands]; // ���������б�

    for (int i = 0; i < nBands; ++i) {
        panBandList[i] = i + 1; // GDAL ���������� 1 ��ʼ
    }

    GDALProgressFunc pfnProgress = GDALTermProgress;

    CPLErr eErr = GDALBuildOverviews(poDS, pszResampling, nOverviews, anOverviewList, 0, panBandList, pfnProgress, nullptr);
    if (eErr != CE_None) {
        QMessageBox::critical(this, "Error", "GDALBuildOverviews failed: " + QString::fromUtf8(CPLGetLastErrorMsg()));
        GDALClose(poDS);
        return;
    }

    GDALClose(poDS);
    QMessageBox::information(this, "Success", "Overviews created successfully.");
    
}