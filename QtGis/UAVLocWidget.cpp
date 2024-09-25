#include "UAVLocWidget.h"
#include "qicon.h"
UAVLocWidget::UAVLocWidget(QWidget* parent)
    : QDialog(parent)
{

    QMetaObject::Connection conn;

    setWindowTitle(QStringLiteral("���˻�Ӱ��Ŀ�궨λ"));
    setWindowIcon(QIcon(":/QtGis/images/res/module.png"));

    QVBoxLayout* input_button_vlayout = new QVBoxLayout;
    input_uav_button = new QPushButton;
    input_uav_button->setObjectName(QStringLiteral("input_uav_button"));
    input_uav_button->setText(QStringLiteral("�������˻�Ӱ��"));

    input_base_button = new QPushButton;
    input_base_button->setObjectName(QStringLiteral("input_base_button"));
    input_base_button->setText(QStringLiteral("�����׼Ӱ��"));

    input_button_vlayout->addWidget(input_uav_button);
    input_button_vlayout->addWidget(input_base_button);

    QVBoxLayout* input_linedit_vlayout = new QVBoxLayout;
    input_uavimgedit = new QTextEdit;
    input_uavimgedit->setFixedHeight(30);
     //�����ı��༭��ĳߴ���ԣ�ʹ��߶Ȳ��洰��������仯
    input_uavimgedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    input_baseimgedit = new QTextEdit;
    input_baseimgedit->setFixedHeight(30);
    // //�����ı��༭��ĳߴ���ԣ�ʹ��߶Ȳ��洰��������仯
    input_baseimgedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    input_linedit_vlayout->addWidget(input_uavimgedit);
    input_linedit_vlayout->addWidget(input_baseimgedit);

    QHBoxLayout* input_hlayout = new QHBoxLayout;
    input_hlayout->addLayout(input_button_vlayout, 0);
    input_hlayout->addLayout(input_linedit_vlayout, 0);

    QGroupBox* input_img_box = new QGroupBox(QStringLiteral("Ӱ������"));
    input_img_box->setLayout(input_hlayout);
    input_img_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    input_querylocedit = new QTextEdit;
    input_querylocedit->setFixedHeight(30);
    // //�����ı��༭��ĳߴ���ԣ�ʹ��߶Ȳ��洰��������仯
    input_querylocedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* input_querylocedit_layout = new QVBoxLayout;
    input_querylocedit_layout->addWidget(input_querylocedit);
    QGroupBox* query_loc_box = new QGroupBox(QStringLiteral("���Ŀ����������[x y]��ע�⣺�ո������"));
    query_loc_box->setLayout(input_querylocedit_layout);
    query_loc_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    input_configedit = new QTextEdit;
    input_configedit->setFixedHeight(50);
    // //�����ı��༭��ĳߴ���ԣ�ʹ��߶Ȳ��洰��������仯
    input_configedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    input_configedit->setText(QStringLiteral("--x 108.860726 --y 37.774952 --pitch -40 --yaw 240 --h 3301 --j 6.5 --e_p 3.45 --res 0.3 --drawmatch true --output_GCP test.gcp --loc 960,540 --UAV 1"));
    QVBoxLayout* input_configedit_layout = new QVBoxLayout;
    input_configedit_layout->addWidget(input_configedit);
    QGroupBox* config_box = new QGroupBox(QStringLiteral("���������--x --y --pitch --yaw --h --j --e_p --res --drawmatch --output_GCP --loc --UAV ��"));
    config_box->setLayout(input_configedit_layout);
    config_box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);


    QDialogButtonBox* pbtnboxStandard = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    conn = connect(pbtnboxStandard, SIGNAL(accepted()), this, SLOT(accept()));
    conn = connect(pbtnboxStandard, SIGNAL(rejected()), this, SLOT(reject()));

    QBoxLayout* pBottomBtnBoxLayout = new QHBoxLayout;
    pBottomBtnBoxLayout->addWidget(pbtnboxStandard);

    QVBoxLayout* pBodylayout = new QVBoxLayout;
    pBodylayout->addWidget(input_img_box);
    pBodylayout->addWidget(query_loc_box);
    pBodylayout->addWidget(config_box);

    QVBoxLayout* pStretchlayout = new QVBoxLayout;
    QSpacerItem* sparcer_item = new QSpacerItem(0, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    pStretchlayout->addItem(sparcer_item);
    QBoxLayout* pMainLayout = new QVBoxLayout;
    pMainLayout->addLayout(pBodylayout);
    pMainLayout->addLayout(pStretchlayout);
    pMainLayout->addLayout(pBottomBtnBoxLayout);

    this->setLayout(pMainLayout);





}