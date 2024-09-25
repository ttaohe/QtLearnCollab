#include "ImgZengqiangWidget.h"

ImgZengqiangWidget::ImgZengqiangWidget(QWidget* parent) : QWidget(parent)
{
    setWindowTitle(QStringLiteral("影像增强"));
    setWindowIcon(QIcon(":/QtGis/images/res/module.png"));

    inputLayerComboBox = new QComboBox();
    inputLayerComboBox->setPlaceholderText("Select input layer");

    currentitmPath = new QTextEdit();
    currentitmPath->setFixedHeight(30); // Set a fixed height for additionalParamsEdit

    QHBoxLayout* input_layer_hlayout = new QHBoxLayout;
    input_layer_hlayout->addWidget(inputLayerComboBox);
    input_layer_hlayout->addWidget(currentitmPath);
    QGroupBox* input_layer_select = new QGroupBox(QStringLiteral("图层选择"));
    input_layer_select->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    input_layer_select->setLayout(input_layer_hlayout);


    QVBoxLayout* first_vlayout = new QVBoxLayout;
    QVBoxLayout* second_vlayout = new QVBoxLayout;
    QVBoxLayout* third_vlayout = new QVBoxLayout;
    QVBoxLayout* forth_vlayout = new QVBoxLayout;
    QHBoxLayout* all_hlayout = new QHBoxLayout;
    brightnessSlider = new QSlider(Qt::Horizontal);
    brightnessSlider->setRange(0, 100);
    brightnessSlider->setValue(50); // 默认值
    gammaSlider = new QSlider(Qt::Horizontal);
    gammaSlider->setRange(0.01, 10.0);
    gammaSlider->setValue(1.00); // 默认值
    first_vlayout->addWidget(new QLabel(QStringLiteral("亮度")));
    first_vlayout->addWidget(new QLabel(QStringLiteral("伽马")));
    second_vlayout->addWidget(brightnessSlider);
    second_vlayout->addWidget(gammaSlider);
    
    
    contrastSlider = new QSlider(Qt::Horizontal);
    contrastSlider->setRange(0, 100);
    contrastSlider->setValue(50); // 默认值
    saturationSlider = new QSlider(Qt::Horizontal);
    saturationSlider->setRange(0, 100);
    saturationSlider->setValue(50); // 默认值
    third_vlayout->addWidget(new QLabel(QStringLiteral("对比度")));
    third_vlayout->addWidget(new QLabel(QStringLiteral("饱和度")));
    forth_vlayout->addWidget(contrastSlider);
    forth_vlayout->addWidget(saturationSlider);

    all_hlayout->addLayout(first_vlayout);
    all_hlayout->addLayout(second_vlayout);
    all_hlayout->addLayout(third_vlayout);
    all_hlayout->addLayout(forth_vlayout);

    QGroupBox* methodBox = new QGroupBox(QStringLiteral("增强配置"));
    methodBox->setLayout(all_hlayout);
    methodBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout* output_button_vlayout = new QVBoxLayout;
    output_img_button = new QPushButton;
    output_img_button->setObjectName(QStringLiteral("input_uav_button"));
    output_img_button->setText(QStringLiteral("输出路径"));

    output_button_vlayout->addWidget(output_img_button);

    QVBoxLayout* output_linedit_vlayout = new QVBoxLayout;
    output_imgedit = new QTextEdit;
    output_imgedit->setFixedHeight(30);
    //设置文本编辑框的尺寸策略，使其高度不随窗口拉伸而变化
    output_imgedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    output_linedit_vlayout->addWidget(output_imgedit);

    QHBoxLayout* output_hlayout = new QHBoxLayout;
    output_hlayout->addLayout(output_button_vlayout, 0);
    output_hlayout->addLayout(output_linedit_vlayout, 0);

    QGroupBox* output_img_box = new QGroupBox(QStringLiteral("结果输出"));
    output_img_box->setLayout(output_hlayout);
    output_img_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    runButton = new QPushButton(QStringLiteral("运行"));


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
    // 设置输入图层编辑框的文本为图层的源路径
    currentitmPath->setText(layerId);
}