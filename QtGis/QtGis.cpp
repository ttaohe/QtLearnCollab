#include "QtGis.h"
#include <QTabWidget>
#include <QProgressBar>
#include <QGridLayout>
#include <QListWidget>
#include <QGroupBox>
#include "QtGisLayerTreeViewMenuProvider.h"

#include "qgsmapcanvas.h"
#include "qgsapplication.h"
#include "qgsstatusbar.h"
#include "qgstaskmanagerwidget.h"
#include "qgsdoublespinbox.h"
#include "qgsdockwidget.h"
#include "QgsLayerTreeView.h"
#include "QgsLayerTreeModel.h"
#include "QgsLayerTreeMapCanvasBridge.h"
#include "QgsLayerTreeRegistryBridge.h"
#include "QgsLayerTree.h"
#include "QgsLayerTreeLayer.h"
#include "QgsLayerTreeViewDefaultActions.h"
#include "QgsLayerTreeUtils.h"

#include "QGISFiles/qgsStatusBarCoordinatesWidget.h"
#include "QGISFiles/qgsstatusbarscalewidget.h"
#include "QGISFiles/qgsstatusbarmagnifierwidget.h"

QtGis* QtGis::sInstance = nullptr;
QtGis::QtGis(QWidget *parent)
    : DemoRibbonWindow(parent)
{
    sInstance = this;
	//�������title
    setWindowTitle(QStringLiteral("Ŀ����ټ�⾫ȷ��λԭ��ϵͳ"));
	setWindowIcon(QIcon(":/QtGis/images/res/mmdetloc.png"));
	//������ͼ����
    createMapCanvas();
	//����QAction
    createActions();
	//�����˵�
    createMenuFile();
	//�������ٷ��ʰ�ť
    createQuickAccessBar();
	//����RibbonPage�����QAction
    createRibbonBar();
	//����״̬��
	createStatusBar();
	showRotation();
	showScale();

	QgsProject::instance()->setCrs(QgsCoordinateReferenceSystem("EPSG:4326"));
	createDockWidget();
	createLayerTreeView();
	setAcceptDrops(true);
	createAttributeTreeView();

	createModuleWidget();


	


}

QtGis::~QtGis()
{
}

void QtGis::dragEnterEvent(QDragEnterEvent* event)
{
	// �����ק�������Ƿ���� URL���ļ���
	if (event->mimeData()->hasUrls())
	{
		const auto urls = event->mimeData()->urls();
		for (const QUrl& url : urls)
		{
			QString filePath = url.toLocalFile();
			QFileInfo fileInfo(filePath);
			// ����ļ���չ���Ƿ���֧�ֵĸ�ʽ�б���
			if (fileInfo.suffix().toLower() == "tif" || fileInfo.suffix().toLower() == "tiff" ||
				fileInfo.suffix().toLower() == "img" || fileInfo.suffix().toLower() == "hgt" ||
				fileInfo.suffix().toLower() == "grd" || fileInfo.suffix().toLower() == "jpeg" ||
				fileInfo.suffix().toLower() == "jpg" || fileInfo.suffix().toLower() == "png" ||
				fileInfo.suffix().toLower() == "bmp" || fileInfo.suffix().toLower() == "gif")
			{
				event->acceptProposedAction();
				return;
			}
		}
	}
	event->ignore();
}

void QtGis::dropEvent(QDropEvent* event)
{
	// ����Ƿ��� URL
	if (event->mimeData()->hasUrls())
	{
		QList<QUrl> urls = event->mimeData()->urls();
		QStringList filePaths;
		for (const QUrl& url : urls)
		{
			// ת��Ϊ�����ļ�·��
			QString filePath = url.toLocalFile();
			filePaths << filePath;
		}
		// �������ͼ���ĺ���
		addRaterLayer(filePaths);
	}
}

void QtGis::createMapCanvas()
{
	//����һ��QTabWidget
    mCenterTabWidget = new QTabWidget(this);
	//����һ������
    mMapCanvas = new QgsMapCanvas();
    mMapCanvas->setObjectName("mapCanvas");
	//���û�������ɫ��ѡ��ɫ�����桢���¼��������
    mMapCanvas->setPreviewJobsEnabled(true);
    mMapCanvas->setCanvasColor(QColor(255, 255, 255));
    mMapCanvas->setSelectionColor(QColor(0, 255, 255));
    mMapCanvas->setProject(QgsProject::instance());
    mMapCanvas->setParallelRenderingEnabled(true);
    mMapCanvas->setCachingEnabled(true);
    mMapCanvas->setMapUpdateInterval(250);

    // set project linked to main canvas
    mMapCanvas->setProject(QgsProject::instance());
	//����������ڴ�����QTabWidget�ؼ�mCenterTabWidget��
    mCenterTabWidget->addTab(mMapCanvas, QStringLiteral("ͼ�㻭��"));
    mCenterTabWidget->tabBar()->setTabButton(mCenterTabWidget->indexOf(mMapCanvas), QTabBar::RightSide, nullptr) ;
	//���ø�QTabWidget�ؼ�mCenterTabWidgetչʾ�ڴ�����м�
    setCentralWidget(mCenterTabWidget);
    mMapCanvas->setFocus();

	//mImgMapTool = new ImgMapTool(mMapCanvas);
	//mMapCanvas->setMapTool(mImgMapTool); // ����ΪĬ�Ϲ���

	// �½��ƶ�����
	m_toolPan = new QgsMapToolPan(mMapCanvas);
	mMapCanvas->setMapTool(m_toolPan);

}

void QtGis::createActions()
{
	// Project
    mFileNew = new QAction(DemoRibbonWindow::createIcon(Image_New), QStringLiteral("�½�"));
    mFileNew->setToolTip(QStringLiteral("�½��ļ�"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileNew()));

    mFileOpen = new QAction(DemoRibbonWindow::createIcon(Image_Open), QStringLiteral("��"));
    mFileOpen->setToolTip(QStringLiteral("���ļ�"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileOpen()));

    mFileSave = new QAction(DemoRibbonWindow::createIcon(Image_Save), QStringLiteral("����"));
    mFileSave->setToolTip(QStringLiteral("�����ļ�"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileSave()));

    mFileSaveAs = new QAction(DemoRibbonWindow::createIcon(Image_SaveAs), QStringLiteral("���Ϊ"));
    mFileSaveAs->setToolTip(QStringLiteral("�ļ����Ϊ"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileSaveAs()));

    mFileClose = new QAction(DemoRibbonWindow::createIcon(Image_Close), QStringLiteral("�ر�"));
    mFileClose->setToolTip(QStringLiteral("�ر��ļ�"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileClose()));

    mFileExit = new QAction(DemoRibbonWindow::createIcon(Image_Exit), QStringLiteral("�˳�"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileExit()));

    //mAddRasterLayer = new QAction(DemoRibbonWindow::createIcon(Add_RasterLayer), QStringLiteral("Add Raster Layer"));
    //connect(mAddRasterLayer, SIGNAL(triggered()), this, SLOT(slot_addRaterLayer()));

    //mAddVectorLayer = new QAction(DemoRibbonWindow::createIcon(Add_VectorLayer), QStringLiteral("Add Vector Layer"));
    //connect(mAddVectorLayer, SIGNAL(triggered()), this, SLOT(slot_addVectorLayer()));

	// ����Ԥ������ҳ
	mImageOpen = new QAction(DemoRibbonWindow::createIcon(mmImage_Open), QStringLiteral("��Ӱ��"));
	connect(mImageOpen, SIGNAL(triggered()), this, SLOT(slot_addImageLayer()));

	mImageJinzita = new QAction(DemoRibbonWindow::createIcon(ImageJinzita), QStringLiteral("Ӱ�������"));
	connect(mImageJinzita, SIGNAL(triggered()), this, SLOT(slot_ImageJinzitaTriggered()));

	mImageCut = new QAction(DemoRibbonWindow::createIcon(ImageCut), QStringLiteral("Ӱ��ü�"));
	connect(mImageCut, SIGNAL(triggered()), this, SLOT(slot_ImageCutTriggered()));

	mImageLashen = new QAction(DemoRibbonWindow::createIcon(ImageLashen), QStringLiteral("Ӱ������"));
	connect(mImageLashen, SIGNAL(triggered()), this, SLOT(slot_ImageLashenTriggered()));

	mImageLvbo = new QAction(DemoRibbonWindow::createIcon(ImageLvbo), QStringLiteral("Ӱ���˲�"));
	connect(mImageLvbo, SIGNAL(triggered()), this, SLOT(slot_ImageLvboTriggered()));

	mImageZhifangtu = new QAction(DemoRibbonWindow::createIcon(ImageZhifangtu), QStringLiteral("Ӱ��ֱ��ͼ����"));
	connect(mImageZhifangtu, SIGNAL(triggered()), this, SLOT(slot_ImageZhifangtuTriggered()));

	mImageZengqiang = new QAction(DemoRibbonWindow::createIcon(ImageZengqiang), QStringLiteral("Ӱ����ǿ"));
	connect(mImageZengqiang, SIGNAL(triggered()), this, SLOT(slot_ImageZengqianguTriggered()));

	// ���ӻ���Ŀ������ҳ
	mrgbDetect = new QAction(DemoRibbonWindow::createIcon(rgbDetect), QStringLiteral("��ѧӰ���ӻ���Ŀ����"));
	connect(mrgbDetect, SIGNAL(triggered()), this, SLOT(slot_rgbDetect()));
	msarDetect = new QAction(DemoRibbonWindow::createIcon(sarDetect), QStringLiteral("SARӰ���ӻ���Ŀ����"));
	connect(msarDetect, SIGNAL(triggered()), this, SLOT(slot_sarDetect()));

	// ��Դ���ܱ�ʶ�������ҳ
	mrgbsarBianshi = new QAction(DemoRibbonWindow::createIcon(rgbsarBianshi), QStringLiteral("��ѧ��SARͬĿ���ʶ����"));
	connect(mrgbsarBianshi, SIGNAL(triggered()), this, SLOT(slot_rgbsarBianshi()));

	// αװĿ������ҳ
	mrgbsarDetect = new QAction(DemoRibbonWindow::createIcon(rgbsarDetect), QStringLiteral("��ѧ|SAR�ں�αװĿ����"));
	connect(mrgbsarDetect, SIGNAL(triggered()), this, SLOT(slot_rgbsarDetect()));
	mrgbirDetect = new QAction(DemoRibbonWindow::createIcon(rgbirDetect), QStringLiteral("��ѧ|�����ں�αװĿ����"));
	connect(mrgbirDetect, SIGNAL(triggered()), this, SLOT(slot_rgbirDetect()));
	mrgbsarirDetect = new QAction(DemoRibbonWindow::createIcon(rgbsarirDetect), QStringLiteral("��ѧ|SAR|�����ں�αװĿ����"));
	connect(mrgbsarirDetect, SIGNAL(triggered()), this, SLOT(slot_rgbsarirDetect()));

	// Ŀ�꾫ȷ��λ���ҳ
	mJinjingLoc = new QAction(DemoRibbonWindow::createIcon(JinjingLoc), QStringLiteral("����ͼ��Ŀ�궨λ"));
	connect(mJinjingLoc, SIGNAL(triggered()), this, SLOT(slot_JinjingLoc()));
	mUAVLoc = new QAction(DemoRibbonWindow::createIcon(UAVLoc), QStringLiteral("���˻�ͼ��Ŀ�궨λ"));
	connect(mUAVLoc, SIGNAL(triggered()), this, SLOT(slot_UAVLoc()));

    
}

void QtGis::createMenuFile()
{
    QIcon iconLogo;
    iconLogo.addPixmap(QPixmap(QStringLiteral(":/QtGis/images/qtitanlogo32x32.png")));
	//���Project�˵�
    QAction* actionFile = ribbonBar()->addSystemButton(iconLogo, tr("&Project"));
    ribbonBar()->systemButton()->setToolButtonStyle(Qt::ToolButtonTextOnly);
    actionFile->setToolTip(tr("Click here to see everything you can do with your document"));
    RibbonSystemMenu* systemMenu = qobject_cast<RibbonSystemMenu*>(actionFile->menu());
	//�ڲ˵��������Ӧ�İ�ť
    systemMenu->addAction(mFileNew);
    systemMenu->addAction(mFileOpen);
    systemMenu->addAction(mFileSave);
    systemMenu->addAction(mFileSaveAs);
    systemMenu->addAction(mFileClose);
    systemMenu->addAction(mFileExit);
	//�ڲ˵�������򿪵Ĺ����б�ؼ�
    RibbonPageSystemRecentFileList* pageRecentFile = systemMenu->addPageRecentFile(QStringLiteral("Recent Maps"));
	//�������չʾ�Ѵ򿪵�9������
    pageRecentFile->setSize(9);
	//�󶨲ۺ���
    connect(pageRecentFile, SIGNAL(openRecentFile(const QString&)), this, SLOT(openRecentFile(const QString&)));
    connect(this, SIGNAL(updateRecentFileActions(const QStringList&)), pageRecentFile, SLOT(updateRecentFileActions(const QStringList&)));
}

void QtGis::createQuickAccessBar()
{
    RibbonQuickAccessBar* quickAccessBar = ribbonBar()->quickAccessBar();
    QAction* action = quickAccessBar->actionCustomizeButton();
    action->setToolTip(tr("Customize Quick Access Bar"));

    QAction* smallButton = quickAccessBar->addAction(DemoRibbonWindow::createIcon(Image_Open), QStringLiteral("Open Project"));
    smallButton->setToolTip(QStringLiteral("Open Project"));
    connect(smallButton, SIGNAL(triggered()), this, SLOT(slot_fileOpen()));

    smallButton = quickAccessBar->addAction(DemoRibbonWindow::createIcon(Image_Save), QStringLiteral("Save Project"));
    smallButton->setToolTip(QStringLiteral("Save Project"));
    connect(smallButton, SIGNAL(triggered()), this, SLOT(slot_fileSave()));

    quickAccessBar->setVisible(true);
}

void QtGis::createRibbonBar()
{
	//�������ݻ�ȡ��Ԥ����ģ�� Layer
	RibbonPage* FirstPageData = ribbonBar()->addPage(QStringLiteral("���ݻ�ȡ��Ԥ����ģ��"));
	//�������� Layer
    RibbonGroup* DatalayerGroup = FirstPageData->addGroup(QStringLiteral(""));
	//�ڷ��� Layer����Ӱ�ť
    //layerGroup->addAction(mAddRasterLayer, Qt::ToolButtonTextUnderIcon);
    //layerGroup->addAction(mAddVectorLayer, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageOpen, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageJinzita, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageCut, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageLashen, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageLvbo, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageZhifangtu, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageZengqiang, Qt::ToolButtonTextUnderIcon);

	//���ӻ�����Ŀ����ģ�� Layer
	RibbonPage* SecondPageData = ribbonBar()->addPage(QStringLiteral("���ӻ�����Ŀ����ģ��"));

	RibbonGroup* RGBSARlayerGroup = SecondPageData->addGroup(QStringLiteral(""));
	RGBSARlayerGroup->addAction(mrgbDetect, Qt::ToolButtonTextUnderIcon);
	//RibbonGroup* SARlayerGroup = SecondPageData->addGroup(QStringLiteral(""));
	RGBSARlayerGroup->addAction(msarDetect, Qt::ToolButtonTextUnderIcon);


	//�������ݻ�ȡ��Ԥ����ģ�� Layer
	RibbonPage* ThirdtPageData = ribbonBar()->addPage(QStringLiteral("��ԴĿ�����ܱ�ʶ����ģ��"));
	//�������� Layer
	RibbonGroup* BianshilayerGroup = ThirdtPageData->addGroup(QStringLiteral(""));
	BianshilayerGroup->addAction(mrgbsarBianshi, Qt::ToolButtonTextUnderIcon);


	//�������ݻ�ȡ��Ԥ����ģ�� Layer
	RibbonPage* FourthPageData = ribbonBar()->addPage(QStringLiteral("αװĿ����ģ��"));
	//�������� Layer
	RibbonGroup* RSlayerGroup = FourthPageData->addGroup(QStringLiteral(""));
	RSlayerGroup->addAction(mrgbsarDetect, Qt::ToolButtonTextUnderIcon);
	RibbonGroup* RIlayerGroup = FourthPageData->addGroup(QStringLiteral(""));
	RIlayerGroup->addAction(mrgbirDetect, Qt::ToolButtonTextUnderIcon);
	RibbonGroup* RSIlayerGroup = FourthPageData->addGroup(QStringLiteral(""));
	RSIlayerGroup->addAction(mrgbsarirDetect, Qt::ToolButtonTextUnderIcon);


	//�������ݻ�ȡ��Ԥ����ģ�� Layer
	RibbonPage* FifthPageData = ribbonBar()->addPage(QStringLiteral("Ŀ�꾫ȷ��λģ��"));
	//�������� Layer
	RibbonGroup* JinJinglayerGroup = FifthPageData->addGroup(QStringLiteral(""));
	JinJinglayerGroup->addAction(mJinjingLoc, Qt::ToolButtonTextUnderIcon);
	RibbonGroup* UAVlayerGroup = FifthPageData->addGroup(QStringLiteral(""));
	UAVlayerGroup->addAction(mUAVLoc, Qt::ToolButtonTextUnderIcon);

    ribbonBar()->setFrameThemeEnabled();
}

void QtGis::createStatusBar()
{
	QFont statusBarFont = font();
	int fontSize = statusBarFont.pointSize();
	fontSize = std::max(fontSize - 1, 8);
	statusBarFont.setPointSize(fontSize);
	statusBar()->setFont(statusBarFont);

	mStatusBar = new QgsStatusBar();
	// mStatusBar->setParentStatusBar(QMainWindow::statusBar());
	mStatusBar->setFont(statusBarFont);
	statusBar()->addPermanentWidget(mStatusBar, 10);

	// Add a panel to the status bar for the scale, coords and progress
  // And also rendering suppression checkbox
	mProgressBar = new QProgressBar(mStatusBar);
	mProgressBar->setObjectName(QStringLiteral("mProgressBar"));
	mProgressBar->setMaximumWidth(100);
	mProgressBar->setMaximumHeight(18);
	mProgressBar->hide();
	mStatusBar->addPermanentWidget(mProgressBar, 1);

	//����������״̬��������֮����źŲ�
	connect(mMapCanvas, &QgsMapCanvas::renderStarting, this, &QtGis::canvasRefreshStarted);
	connect(mMapCanvas, &QgsMapCanvas::mapCanvasRefreshed, this, &QtGis::canvasRefreshFinished);

	mTaskManagerWidget = new QgsTaskManagerStatusBarWidget(QgsApplication::taskManager(), mStatusBar);
	mTaskManagerWidget->setFont(statusBarFont);
	mStatusBar->addPermanentWidget(mTaskManagerWidget, 0);

	//coords status bar widget
	mCoordsEdit = new QgsStatusBarCoordinatesWidget(mStatusBar);
	
	mCoordsEdit->setObjectName(QStringLiteral("mCoordsEdit"));
	mCoordsEdit->setMapCanvas(mMapCanvas);
	mCoordsEdit->setFont(statusBarFont);
	mStatusBar->addPermanentWidget(mCoordsEdit, 0);

	//mScaleWidget = new QgsStatusBarScaleWidget(mMapCanvas, mStatusBar);
	//mScaleWidget->setObjectName(QStringLiteral("mScaleWidget"));
	//mScaleWidget->setFont(statusBarFont);
	//mStatusBar->addPermanentWidget(mScaleWidget, 0);

	

	// zoom widget
	mMagnifierWidget = new QgsStatusBarMagnifierWidget(mStatusBar);
	mMagnifierWidget->setObjectName(QStringLiteral("mMagnifierWidget"));
	mMagnifierWidget->setFont(statusBarFont);
	connect(mMapCanvas, &QgsMapCanvas::magnificationChanged, mMagnifierWidget, &QgsStatusBarMagnifierWidget::updateMagnification);
	connect(mMagnifierWidget, &QgsStatusBarMagnifierWidget::magnificationChanged, mMapCanvas, [=](double factor) { mMapCanvas->setMagnificationFactor(factor); });
	connect(mMagnifierWidget, &QgsStatusBarMagnifierWidget::scaleLockChanged, mMapCanvas, &QgsMapCanvas::setScaleLocked);
	connect(mMagnifierWidget, &QgsStatusBarMagnifierWidget::scaleLockChanged, mScaleWidget, &QgsStatusBarScaleWidget::setLocked);
	mMagnifierWidget->updateMagnification(QSettings().value(QStringLiteral("/qgis/magnifier_factor_default"), 1.0).toDouble());
	mStatusBar->addPermanentWidget(mMagnifierWidget, 0);

	//// add a widget to show/set current rotation
	//mRotationLabel = new QLabel(QString(), mStatusBar);
	//mRotationLabel->setObjectName(QStringLiteral("mRotationLabel"));
	//mRotationLabel->setFont(statusBarFont);
	//mRotationLabel->setMinimumWidth(10);
	////mRotationLabel->setMaximumHeight( 20 );
	//mRotationLabel->setMargin(3);
	//mRotationLabel->setAlignment(Qt::AlignCenter);
	//mRotationLabel->setFrameStyle(QFrame::NoFrame);
	//mRotationLabel->setText(tr("Rotation"));
	//mRotationLabel->setToolTip(tr("Current clockwise map rotation in degrees"));
	//mStatusBar->addPermanentWidget(mRotationLabel, 0);

	//mRotationEdit = new QgsDoubleSpinBox(mStatusBar);
	//mRotationEdit->setObjectName(QStringLiteral("mRotationEdit"));
	//mRotationEdit->setClearValue(0.0);
	//mRotationEdit->setKeyboardTracking(false);
	//mRotationEdit->setMaximumWidth(120);
	//mRotationEdit->setDecimals(1);
	//mRotationEdit->setRange(-360.0, 360.0);
	//mRotationEdit->setWrapping(true);
	//mRotationEdit->setSingleStep(5.0);
	//mRotationEdit->setFont(statusBarFont);
	//mRotationEdit->setSuffix(tr("degree"));
	//mRotationEdit->setToolTip(tr("Current clockwise map rotation in degrees"));
	//mStatusBar->addPermanentWidget(mRotationEdit, 0);
	//connect(mRotationEdit, static_cast <void (QgsDoubleSpinBox::*)(double)> (&QgsDoubleSpinBox::valueChanged), this, &QtGis::userRotation);


	//mOnTheFlyProjectionStatusButton = new QToolButton(mStatusBar);
	//mOnTheFlyProjectionStatusButton->setAutoRaise(true);
	//mOnTheFlyProjectionStatusButton->setFont(statusBarFont);
	//mOnTheFlyProjectionStatusButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	//mOnTheFlyProjectionStatusButton->setObjectName(QStringLiteral("mOntheFlyProjectionStatusButton"));

	//mOnTheFlyProjectionStatusButton->setMaximumHeight(mScaleWidget->height());
	//mOnTheFlyProjectionStatusButton->setIcon(QgsApplication::getThemeIcon(QStringLiteral("mIconProjectionEnabled.svg")));
	//connect(QgsProject::instance(), &QgsProject::crsChanged, this, &QtGis::projectCrsChanged);
	//mStatusBar->addPermanentWidget(mOnTheFlyProjectionStatusButton, 0);
	mStatusBar->showMessage(QStringLiteral("�Ѿ���"));

}

void QtGis::createModuleWidget()
{
	// ����Ԥ����ģ��
	mBuildPyramidsWidget = new BuildPyramidsWidget();
	mImgCutWidget = new ImgCutWidget();
	mImgLashenWidget = new ImgLashenWidget();
	mImgLvboWidget = new ImgLvboWidget();
	mImgZhifangtuWidget = new ImgZhifangtuWidget();
	mImgZengqiangWidget = new ImgZengqiangWidget();

	// ģ��
	mrgbDetectWidget = new rgbDetectWidget();
	msarDetectWidget = new sarDetectWidget();
	mrgbsarBianshiWidget = new rgbsarBianshiWidget();
	mrgbsarDetectWidget = new rgbsarDetectWidget();
	mrgbirDetectWidget = new rgbirDetectWidget();
	mrgbsarirDetectWidget = new rgbsarirDetectWidget();
	mJinjingLocWidget = new JinjingLocWidget();
	mUAVLocWidget = new UAVLocWidget();

}

void QtGis::createDockWidget()
{
	//�½�ͼ��QgsDockWidget
	mLayerMapDock = new QgsDockWidget(this);
	mLayerMapDock->setWindowTitle(QStringLiteral("ͼ����ͼ"));
	mLayerMapDock->setObjectName(QStringLiteral("ͼ����ͼ"));
	//��ͼ��QgsDockWidget ��������չʾ
	addDockWidget(Qt::LeftDockWidgetArea, mLayerMapDock);

	mAttributeDock = new QgsDockWidget(this);
	mAttributeDock->setWindowTitle(QStringLiteral("������ͼ"));
	mAttributeDock->setObjectName(QStringLiteral("������ͼ"));
	addDockWidget(Qt::RightDockWidgetArea, mAttributeDock);

}

void QtGis::createLayerTreeView()
{
	//�½�ͼ�����οؼ�
	mLayerTreeView = new QgsLayerTreeView(this);
	mLayerTreeView->setObjectName(QStringLiteral("theLayerTreeView"));

	//����QgsLayerTreeModel ʵ��,�������û�ͨ����ѡ��ѡ���ֶ�����/��ʾͼ��
	QgsLayerTreeModel* model = new QgsLayerTreeModel(QgsProject::instance()->layerTreeRoot(), this);
	model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
	model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
	model->setFlag(QgsLayerTreeModel::AllowNodeRename);
	model->setFlag(QgsLayerTreeModel::ShowLegendAsTree);
	model->setFlag(QgsLayerTreeModel::UseEmbeddedWidgets);
	model->setFlag(QgsLayerTreeModel::UseTextFormatting);
	model->setAutoCollapseLegendNodes(10);

	//��View��ͼ��Model���ݰ�
	mLayerTreeView->setModel(model);

	//������ʵ���е�ͼ����ڵ��뻭���󶨣�ʵ�����ͼ�����ؼ������ݽ���
	mLayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge(QgsProject::instance()->layerTreeRoot(), mMapCanvas, this);

	//���ͼ����
	QAction* addGroup = new QAction(tr("Add Group"), this);
	addGroup->setIcon(createIcon(Add_Group));
	addGroup->setToolTip(tr("Add Group"));
	connect(addGroup, &QAction::triggered, mLayerTreeView->defaultActions(), &QgsLayerTreeViewDefaultActions::addGroup);
	//չ������ͼ����
	QAction* actionExpandAll = new QAction(tr("Expand All"), this);
	actionExpandAll->setIcon(createIcon(Expand_All));
	actionExpandAll->setToolTip(tr("Expand All"));
	connect(actionExpandAll, &QAction::triggered, mLayerTreeView, &QgsLayerTreeView::expandAllNodes);
	//�۵�����ͼ����
	QAction* actionCollapseAll = new QAction(tr("Collapse All"), this);
	actionCollapseAll->setIcon(createIcon(Collapse_All));
	actionCollapseAll->setToolTip(tr("Collapse All"));
	connect(actionCollapseAll, &QAction::triggered, mLayerTreeView, &QgsLayerTreeView::collapseAllNodes);

	QToolBar* toolbar = new QToolBar(this);
	toolbar->setIconSize(QSize(16, 16));
	toolbar->addAction(addGroup);
	toolbar->addAction(actionExpandAll);
	toolbar->addAction(actionCollapseAll);

	QVBoxLayout* vboxLayout = new QVBoxLayout;
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	vboxLayout->setSpacing(0);
	vboxLayout->addWidget(toolbar);
	vboxLayout->addWidget(mLayerTreeView);

	QWidget* w = new QWidget;
	w->setLayout(vboxLayout);

	//���õ���Ҽ����ֲ˵���
	mLayerTreeView->setMenuProvider(new QtGisLayerTreeViewMenuProvider(mLayerTreeView, mMapCanvas));

	//���õ�������������
	connect(mLayerTreeView, &QTreeView::clicked, this, &QtGis::onLayerTreeViewClicked);

	//��View��ӵ�����Dock�ؼ���չʾ
	mLayerMapDock->setWidget(w);

	connect(mLayerTreeView, &QgsLayerTreeView::currentLayerChanged, this, &QtGis::onActiveLayerChanged);
	connect(QgsProject::instance()->layerTreeRegistryBridge(), &QgsLayerTreeRegistryBridge::addedLayersToLayerTree, this, &QtGis::autoSelectAddedLayer);
}


void QtGis::createAttributeTreeView()
{

	// ���� MyAttribute ʵ��������Ϊ������ͼ������
	attributeWidget = new ImgAttribute(this);
	mAttributeDock->setWidget(attributeWidget);

	//��View��ӵ�����Dock�ؼ���չʾ
	mAttributeDock->setWidget(attributeWidget);
	
	
}

QList<QgsMapCanvas*> QtGis::mapCanvases()
{
	auto canvases = findChildren< QgsMapCanvas* >();
	canvases.erase(std::remove_if(canvases.begin(), canvases.end(),
		[](QgsMapCanvas* canvas)
		{
			return !canvas || canvas->property("browser_canvas").toBool();
		}), canvases.end());
	return canvases;
}

void QtGis::freezeCanvases(bool frozen)
{
	const auto canvases = mapCanvases();
	for (QgsMapCanvas* canvas : canvases)
	{
		canvas->freeze(frozen);
	}
}

QgsMapCanvas* QtGis::mapCanvas()
{
	return mMapCanvas;
}

void QtGis::refreshMapCanvas(bool redrawAllLayers)
{
	const auto canvases = mapCanvases();
	for (QgsMapCanvas* canvas : canvases)
	{
		//stop any current rendering
		canvas->stopRendering();
		if (redrawAllLayers)
			canvas->refreshAllLayers();
		else
			canvas->refresh();
	}
}

void QtGis::markDirty()
{
	QgsProject::instance()->setDirty(true);
}

void QtGis::onLayerTreeViewClicked()
{
	QgsLayerTreeLayer* layerNode = dynamic_cast<QgsLayerTreeLayer*>(mLayerTreeView->currentNode());
	if (layerNode)
	{
		QString filePath;
		QgsMapLayer* layer = layerNode->layer();
		if (layer)
		{
			// ���Դ�ͼ���Դ�л�ȡ�ļ�·��
			QString uri = layer->dataProvider()->dataSourceUri();
			//QString uri = layerData["url"].toString();

			//// ���Խ��� URL ����ȡ�ļ�·��
			//QUrl url(uri);
			//if (url.isLocalFile())
			//{
			//	filePath = url.toLocalFile();
			//}
			//else
			//{
			//	// ��� URL ���Ǳ����ļ���ֱ��ʹ�� URI
			//	filePath = uri;
			//}

			// ����������ͼ
			attributeWidget->PrintData(uri);
		}
	}
}

void QtGis::onActiveLayerChanged(QgsMapLayer* layer)
{
	const QList< QgsMapCanvas* > canvases = mapCanvases();
	for (QgsMapCanvas* canvas : canvases)
		canvas->setCurrentLayer(layer);
	//TODO emit activeLayerChanged
}

void QtGis::autoSelectAddedLayer(QList<QgsMapLayer*> layers)
{
	if (!layers.isEmpty()) {
		QgsLayerTreeLayer* nodeLayer = QgsProject::instance()->layerTreeRoot()->findLayer(layers[0]->id());
		if (!nodeLayer)
			return;
		auto index = mLayerTreeView->layerTreeModel()->node2index(nodeLayer);
		mLayerTreeView->setCurrentIndex(index);
	}
}

void QtGis::updateNewLayerInsertionPoint()
{
	QgsLayerTreeGroup* insertGroup = mLayerTreeView->layerTreeModel()->rootGroup();
	QModelIndex current = mLayerTreeView->currentIndex();
	int index = 0;
	QgsLayerTreeRegistryBridge::InsertionPoint insertionPoint = QgsLayerTreeRegistryBridge::InsertionPoint(insertGroup, index);;

	if (current.isValid())
	{
		index = current.row();

		QgsLayerTreeNode* currentNode = mLayerTreeView->currentNode();
		if (currentNode)
		{
			// if the insertion point is actually a group, insert new layers into the group
			if (QgsLayerTree::isGroup(currentNode))
			{
				// if the group is embedded go to the first non-embedded group, at worst the top level item
				QgsLayerTreeGroup* insertGroup = QgsLayerTreeUtils::firstGroupWithoutCustomProperty(QgsLayerTree::toGroup(currentNode), QStringLiteral("embedded"));

				insertionPoint = QgsLayerTreeRegistryBridge::InsertionPoint(insertGroup, 0);
			}
			else {
				// otherwise just set the insertion point in front of the current node
				QgsLayerTreeNode* parentNode = currentNode->parent();
				if (QgsLayerTree::isGroup(parentNode))
				{
					// if the group is embedded go to the first non-embedded group, at worst the top level item
					QgsLayerTreeGroup* parentGroup = QgsLayerTree::toGroup(parentNode);
					insertGroup = QgsLayerTreeUtils::firstGroupWithoutCustomProperty(parentGroup, QStringLiteral("embedded"));
					if (parentGroup != insertGroup)
						index = 0;
				}
				insertionPoint = QgsLayerTreeRegistryBridge::InsertionPoint(insertGroup, index);
			}
		}
	}
	QgsProject::instance()->layerTreeRegistryBridge()->setLayerInsertionPoint(insertionPoint);
}

void QtGis::slot_fileNew()
{
}

void QtGis::slot_fileOpen()
{
}

void QtGis::slot_fileSave()
{
}

void QtGis::slot_fileClose()
{
}

void QtGis::slot_fileExit()
{
}

void QtGis::openRecentFile(const QString& file)
{
}

void QtGis::updateRecentFileActions(const QStringList&)
{
}


void QtGis::projectCrsChanged()
{
}

void QtGis::canvasRefreshStarted()
{
	mStatusBar->showMessage(QStringLiteral("��Ⱦ��..."));
}

void QtGis::canvasRefreshFinished()
{
	mStatusBar->showMessage(QStringLiteral("�Ѿ���"));
}

void QtGis::userRotation()
{
}

void QtGis::showRotation()
{
	//double rotation = mRotationEdit->value();
	//statusBar()->showMessage(tr("Rotation %1 degrees").arg(rotation));
}

void QtGis::showScale()
{
	//double scale = mMapCanvas->scale();
	//statusBar()->showMessage(tr("Scale 1:%1").arg(scale));
	
}

void QtGis::slot_fileSaveAs()
{
}
