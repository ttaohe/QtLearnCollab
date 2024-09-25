#include "ImgLvboWidget.h"

ImgLvboWidget::ImgLvboWidget(QWidget* parent) : QWidget(parent)
{

    setWindowTitle(QStringLiteral("影像滤波"));
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

    MethodComboBox = new QComboBox();
    MethodComboBox->addItems({ QStringLiteral("高斯滤波"), QStringLiteral("中值滤波"), QStringLiteral("锐化滤波"), QStringLiteral("拉普拉斯滤波") });

    QVBoxLayout* advanced_params_vlayout = new QVBoxLayout;
    advanced_params_vlayout->addWidget(MethodComboBox);

    QGroupBox* advanced_params_box = new QGroupBox(QStringLiteral("滤波方法"));
    advanced_params_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    advanced_params_box->setLayout(advanced_params_vlayout);

    QHBoxLayout* clip_hlayout = new QHBoxLayout;
    QHBoxLayout* gridsize_hlayout = new QHBoxLayout;
    QHBoxLayout* all_hlayout = new QHBoxLayout;
    clipEdit = new QTextEdit();
    gridsizeEdit = new QTextEdit();
    clip_hlayout->addWidget(new QLabel(QStringLiteral("核大小: ")));
    clip_hlayout->addWidget(clipEdit);
    gridsize_hlayout->addWidget(new QLabel(QStringLiteral("标准差: ")));
    gridsize_hlayout->addWidget(gridsizeEdit);
    all_hlayout->addLayout(clip_hlayout);
    all_hlayout->addLayout(gridsize_hlayout);
    clipEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit
    gridsizeEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit

    QGroupBox* paramsBox = new QGroupBox(QStringLiteral("滤波参数配置"));
    paramsBox->setLayout(all_hlayout);


    runButton = new QPushButton(QStringLiteral("运行"));


    QBoxLayout* pBottomBtnBoxLayout = new QHBoxLayout;
    pBottomBtnBoxLayout->addWidget(runButton);

    QVBoxLayout* pBodylayout = new QVBoxLayout;
    pBodylayout->addWidget(input_layer_select);
    pBodylayout->addWidget(advanced_params_box);
    pBodylayout->addWidget(paramsBox);

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

void ImgLvboWidget::updateLayerComboBox(QgsLayerTreeView* treeView)
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

void ImgLvboWidget::onLayerSelected(int index)
{
    QString layerId = inputLayerComboBox->itemData(index).toString();
    // 设置输入图层编辑框的文本为图层的源路径
    currentitmPath->setText(layerId);
}