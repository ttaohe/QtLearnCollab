#include "ImgZengqiangWidget.h"

ImgZengqiangWidget::ImgZengqiangWidget(QWidget* parent) : QWidget(parent)
{
    setWindowTitle(QStringLiteral("Ӱ����ǿ"));
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


    QVBoxLayout* first_vlayout = new QVBoxLayout;
    QVBoxLayout* second_vlayout = new QVBoxLayout;
    QVBoxLayout* third_vlayout = new QVBoxLayout;
    QVBoxLayout* forth_vlayout = new QVBoxLayout;
    QHBoxLayout* all_hlayout = new QHBoxLayout;
    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(50); // Ĭ��ֵ
    gammaSlider = new QSlider(Qt::Horizontal);
    gammaSlider->setRange(0.01, 10.0);
    gammaSlider->setValue(1.00); // Ĭ��ֵ
    first_vlayout->addWidget(new QLabel(QStringLiteral("����")));
    first_vlayout->addWidget(new QLabel(QStringLiteral("٤��")));
    second_vlayout->addWidget(brightnessSlider);
    second_vlayout->addWidget(gammaSlider);
    
    
    contrastSlider = new QSlider(Qt::Horizontal);
    contrastSlider->setRange(0, 100);
    contrastSlider->setValue(50); // Ĭ��ֵ
    saturationSlider = new QSlider(Qt::Horizontal);
    saturationSlider->setRange(0, 100);
    saturationSlider->setValue(50); // Ĭ��ֵ
    third_vlayout->addWidget(new QLabel(QStringLiteral("�Աȶ�")));
    third_vlayout->addWidget(new QLabel(QStringLiteral("���Ͷ�")));
    forth_vlayout->addWidget(contrastSlider);
    forth_vlayout->addWidget(saturationSlider);

    all_hlayout->addLayout(first_vlayout);
    all_hlayout->addLayout(second_vlayout);
    all_hlayout->addLayout(third_vlayout);
    all_hlayout->addLayout(forth_vlayout);

    QGroupBox* methodBox = new QGroupBox(QStringLiteral("��ǿ����"));
    methodBox->setLayout(all_hlayout);
    methodBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout* output_button_vlayout = new QVBoxLayout;
    output_img_button = new QPushButton;
    output_img_button->setObjectName(QStringLiteral("input_uav_button"));
    output_img_button->setText(QStringLiteral("���·��"));

    output_button_vlayout->addWidget(output_img_button);

    QVBoxLayout* output_linedit_vlayout = new QVBoxLayout;
    output_imgedit = new QTextEdit;
    output_imgedit->setFixedHeight(30);
    //�����ı��༭��ĳߴ���ԣ�ʹ��߶Ȳ��洰��������仯
    output_imgedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    output_linedit_vlayout->addWidget(output_imgedit);

    QHBoxLayout* output_hlayout = new QHBoxLayout;
    output_hlayout->addLayout(output_button_vlayout, 0);
    output_hlayout->addLayout(output_linedit_vlayout, 0);

    QGroupBox* output_img_box = new QGroupBox(QStringLiteral("������"));
    output_img_box->setLayout(output_hlayout);
    output_img_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    runButton = new QPushButton(QStringLiteral("����"));


    QBoxLayout* pBottomBtnBoxLayout = new QHBoxLayout;
    pBottomBtnBoxLayout->addWidget(runButton);

    QVBoxLayout* pBodylayout = new QVBoxLayout;
    pBodylayout->addWidget(input_layer_select);
    pBodylayout->addWidget(methodBox);
    pBodylayout->addWidget(output_img_box);

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

void ImgZengqiangWidget::updateLayerComboBox(QgsLayerTreeView* treeView)
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

void ImgZengqiangWidget::onLayerSelected(int index)
{
    QString layerId = inputLayerComboBox->itemData(index).toString();
    // ��������ͼ��༭����ı�Ϊͼ���Դ·��
    currentitmPath->setText(layerId);
}