#include "sarDetectWidget.h"
#include "qicon.h"
sarDetectWidget::sarDetectWidget(QWidget* parent)
    : QDialog(parent)
{

    QMetaObject::Connection conn;

    setWindowTitle(QStringLiteral("SAR海杂波干扰的目标检测"));
    setWindowIcon(QIcon(":/QtGis/images/res/module.png"));

    QVBoxLayout* input_button_vlayout = new QVBoxLayout;
    input_img_button = new QPushButton;
    input_img_button->setObjectName(QStringLiteral("input_uav_button"));
    input_img_button->setText(QStringLiteral("导入影像"));

    input_button_vlayout->addWidget(input_img_button);

    QVBoxLayout* input_linedit_vlayout = new QVBoxLayout;
    input_imgedit = new QTextEdit;
    input_imgedit->setFixedHeight(30);
    //设置文本编辑框的尺寸策略，使其高度不随窗口拉伸而变化
    input_imgedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    input_linedit_vlayout->addWidget(input_imgedit);

    QHBoxLayout* input_hlayout = new QHBoxLayout;
    input_hlayout->addLayout(input_button_vlayout, 0);
    input_hlayout->addLayout(input_linedit_vlayout, 0);

    QGroupBox* input_img_box = new QGroupBox(QStringLiteral("影像输入"));
    input_img_box->setLayout(input_hlayout);
    input_img_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout* model_button_vlayout = new QVBoxLayout;
    input_model_button = new QPushButton;
    input_model_button->setObjectName(QStringLiteral("input_uav_button"));
    input_model_button->setText(QStringLiteral("导入模型"));

    model_button_vlayout->addWidget(input_model_button);

    QVBoxLayout* model_linedit_vlayout = new QVBoxLayout;
    input_modeledit = new QTextEdit;
    input_modeledit->setFixedHeight(30);
    //设置文本编辑框的尺寸策略，使其高度不随窗口拉伸而变化
    input_modeledit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    model_linedit_vlayout->addWidget(input_modeledit);

    QHBoxLayout* model_hlayout = new QHBoxLayout;
    model_hlayout->addLayout(model_button_vlayout, 0);
    model_hlayout->addLayout(model_linedit_vlayout, 0);

    QGroupBox* model_box = new QGroupBox(QStringLiteral("模型选择"));
    model_box->setLayout(model_hlayout);
    model_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


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


    QDialogButtonBox* pbtnboxStandard = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    conn = connect(pbtnboxStandard, SIGNAL(accepted()), this, SLOT(accept()));
    conn = connect(pbtnboxStandard, SIGNAL(rejected()), this, SLOT(reject()));

    QBoxLayout* pBottomBtnBoxLayout = new QHBoxLayout;
    pBottomBtnBoxLayout->addWidget(pbtnboxStandard);

    QVBoxLayout* pBodylayout = new QVBoxLayout;
    pBodylayout->addWidget(input_img_box);
    pBodylayout->addWidget(model_box);
    pBodylayout->addWidget(output_img_box);

    QVBoxLayout* pStretchlayout = new QVBoxLayout;
    QSpacerItem* sparcer_item = new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    pStretchlayout->addItem(sparcer_item);
    QBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pBodylayout);
    pMainLayout->addLayout(pStretchlayout);
    pMainLayout->addLayout(pBottomBtnBoxLayout);

    this->setLayout(pMainLayout);





}