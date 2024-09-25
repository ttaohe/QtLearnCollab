#include "ImgLashenWidget.h"

ImgLashenWidget::ImgLashenWidget(QWidget* parent) : QWidget(parent)
{
    

    setWindowTitle(QStringLiteral("影像拉伸"));
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
    MethodComboBox->addItems({ QStringLiteral("最大最小拉伸"), QStringLiteral("百分比拉伸"), QStringLiteral("标准差拉伸")});

    QVBoxLayout* advanced_params_vlayout = new QVBoxLayout;
    advanced_params_vlayout->addWidget(MethodComboBox);

    QGroupBox* advanced_params_box = new QGroupBox(QStringLiteral("拉伸方法"));
    advanced_params_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    advanced_params_box->setLayout(advanced_params_vlayout);

    QHBoxLayout* min_hlayout = new QHBoxLayout;
    QHBoxLayout* max_hlayout = new QHBoxLayout;
    QHBoxLayout* minmax_hlayout = new QHBoxLayout;
    minEdit = new QTextEdit();
    maxEdit = new QTextEdit();
    min_hlayout->addWidget(new QLabel("min: "));
    min_hlayout->addWidget(minEdit);
    max_hlayout->addWidget(new QLabel("max: "));
    max_hlayout->addWidget(maxEdit);
    minmax_hlayout->addLayout(min_hlayout);
    minmax_hlayout->addLayout(max_hlayout);
    minEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit
    maxEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit

    QHBoxLayout* norm_hlayout = new QHBoxLayout;
    QHBoxLayout* percent_hlayout = new QHBoxLayout;
    QHBoxLayout* normpercent_hlayout = new QHBoxLayout;
    normEdit = new QTextEdit();
    percentEdit = new QTextEdit();
    norm_hlayout->addWidget(new QLabel("norm: "));
    norm_hlayout->addWidget(normEdit);
    percent_hlayout->addWidget(new QLabel("percent: "));
    percent_hlayout->addWidget(percentEdit);
    normpercent_hlayout->addLayout(norm_hlayout);
    normpercent_hlayout->addLayout(percent_hlayout);
    normEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit
    percentEdit->setFixedHeight(30); // Set a fixed height for additionalParamsEdit

    QVBoxLayout* zuobiao_vlayout = new QVBoxLayout;
    zuobiao_vlayout->addLayout(minmax_hlayout);
    zuobiao_vlayout->addLayout(normpercent_hlayout);
    QGroupBox* zuobiao_box = new QGroupBox(QStringLiteral("拉伸参数配置"));
    zuobiao_box->setLayout(zuobiao_vlayout);
    zuobiao_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);



    runButton = new QPushButton(QStringLiteral("运行"));


    QBoxLayout* pBottomBtnBoxLayout = new QHBoxLayout;
    pBottomBtnBoxLayout->addWidget(runButton);

    QVBoxLayout* pBodylayout = new QVBoxLayout;
    pBodylayout->addWidget(input_layer_select);
    pBodylayout->addWidget(advanced_params_box);
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

void ImgLashenWidget::onLayerSelected(int index)
{
    QString layerId = inputLayerComboBox->itemData(index).toString();
    // 设置输入图层编辑框的文本为图层的源路径
    currentitmPath->setText(layerId);
}

void ImgLashenWidget::updateLayerComboBox(QgsLayerTreeView* treeView)
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





