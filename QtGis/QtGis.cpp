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
	//设置软件title
    setWindowTitle(QStringLiteral("目标快速检测精确定位原型系统"));
	setWindowIcon(QIcon(":/QtGis/images/res/mmdetloc.png"));
	//创建地图画布
    createMapCanvas();
	//创建QAction
    createActions();
	//创建菜单
    createMenuFile();
	//创建快速访问按钮
    createQuickAccessBar();
	//创建RibbonPage并添加QAction
    createRibbonBar();
	//创建状态栏
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
	// 检查拖拽的数据是否包含 URL（文件）
	if (event->mimeData()->hasUrls())
	{
		const auto urls = event->mimeData()->urls();
		for (const QUrl& url : urls)
		{
			QString filePath = url.toLocalFile();
			QFileInfo fileInfo(filePath);
			// 检查文件扩展名是否在支持的格式列表中
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
	// 检查是否有 URL
	if (event->mimeData()->hasUrls())
	{
		QList<QUrl> urls = event->mimeData()->urls();
		QStringList filePaths;
		for (const QUrl& url : urls)
		{
			// 转换为本地文件路径
			QString filePath = url.toLocalFile();
			filePaths << filePath;
		}
		// 调用添加图像层的函数
		addRaterLayer(filePaths);
	}
}

void QtGis::createMapCanvas()
{
	//创建一个QTabWidget
    mCenterTabWidget = new QTabWidget(this);
	//创建一个画布
    mMapCanvas = new QgsMapCanvas();
    mMapCanvas->setObjectName("mapCanvas");
	//设置画布背景色、选中色、缓存、更新间隔等属性
    mMapCanvas->setPreviewJobsEnabled(true);
    mMapCanvas->setCanvasColor(QColor(255, 255, 255));
    mMapCanvas->setSelectionColor(QColor(0, 255, 255));
    mMapCanvas->setProject(QgsProject::instance());
    mMapCanvas->setParallelRenderingEnabled(true);
    mMapCanvas->setCachingEnabled(true);
    mMapCanvas->setMapUpdateInterval(250);

    // set project linked to main canvas
    mMapCanvas->setProject(QgsProject::instance());
	//将画布添加在创建的QTabWidget控件mCenterTabWidget中
    mCenterTabWidget->addTab(mMapCanvas, QStringLiteral("图层画布"));
    mCenterTabWidget->tabBar()->setTabButton(mCenterTabWidget->indexOf(mMapCanvas), QTabBar::RightSide, nullptr) ;
	//设置该QTabWidget控件mCenterTabWidget展示在窗体的中间
    setCentralWidget(mCenterTabWidget);
    mMapCanvas->setFocus();

	//mImgMapTool = new ImgMapTool(mMapCanvas);
	//mMapCanvas->setMapTool(mImgMapTool); // 设置为默认工具

	// 新建移动工具
	m_toolPan = new QgsMapToolPan(mMapCanvas);
	mMapCanvas->setMapTool(m_toolPan);

}

void QtGis::createActions()
{
	// Project
    mFileNew = new QAction(DemoRibbonWindow::createIcon(Image_New), QStringLiteral("新建"));
    mFileNew->setToolTip(QStringLiteral("新建文件"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileNew()));

    mFileOpen = new QAction(DemoRibbonWindow::createIcon(Image_Open), QStringLiteral("打开"));
    mFileOpen->setToolTip(QStringLiteral("打开文件"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileOpen()));

    mFileSave = new QAction(DemoRibbonWindow::createIcon(Image_Save), QStringLiteral("保存"));
    mFileSave->setToolTip(QStringLiteral("保存文件"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileSave()));

    mFileSaveAs = new QAction(DemoRibbonWindow::createIcon(Image_SaveAs), QStringLiteral("另存为"));
    mFileSaveAs->setToolTip(QStringLiteral("文件另存为"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileSaveAs()));

    mFileClose = new QAction(DemoRibbonWindow::createIcon(Image_Close), QStringLiteral("关闭"));
    mFileClose->setToolTip(QStringLiteral("关闭文件"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileClose()));

    mFileExit = new QAction(DemoRibbonWindow::createIcon(Image_Exit), QStringLiteral("退出"));
    connect(mFileSaveAs, SIGNAL(triggered()), this, SLOT(slot_fileExit()));

    //mAddRasterLayer = new QAction(DemoRibbonWindow::createIcon(Add_RasterLayer), QStringLiteral("Add Raster Layer"));
    //connect(mAddRasterLayer, SIGNAL(triggered()), this, SLOT(slot_addRaterLayer()));

    //mAddVectorLayer = new QAction(DemoRibbonWindow::createIcon(Add_VectorLayer), QStringLiteral("Add Vector Layer"));
    //connect(mAddVectorLayer, SIGNAL(triggered()), this, SLOT(slot_addVectorLayer()));

	// 数据预处理插件页
	mImageOpen = new QAction(DemoRibbonWindow::createIcon(mmImage_Open), QStringLiteral("打开影像"));
	connect(mImageOpen, SIGNAL(triggered()), this, SLOT(slot_addImageLayer()));

	mImageJinzita = new QAction(DemoRibbonWindow::createIcon(ImageJinzita), QStringLiteral("影像金字塔"));
	connect(mImageJinzita, SIGNAL(triggered()), this, SLOT(slot_ImageJinzitaTriggered()));

	mImageCut = new QAction(DemoRibbonWindow::createIcon(ImageCut), QStringLiteral("影像裁剪"));
	connect(mImageCut, SIGNAL(triggered()), this, SLOT(slot_ImageCutTriggered()));

	mImageLashen = new QAction(DemoRibbonWindow::createIcon(ImageLashen), QStringLiteral("影像拉伸"));
	connect(mImageLashen, SIGNAL(triggered()), this, SLOT(slot_ImageLashenTriggered()));

	mImageLvbo = new QAction(DemoRibbonWindow::createIcon(ImageLvbo), QStringLiteral("影像滤波"));
	connect(mImageLvbo, SIGNAL(triggered()), this, SLOT(slot_ImageLvboTriggered()));

	mImageZhifangtu = new QAction(DemoRibbonWindow::createIcon(ImageZhifangtu), QStringLiteral("影像直方图均衡"));
	connect(mImageZhifangtu, SIGNAL(triggered()), this, SLOT(slot_ImageZhifangtuTriggered()));

	mImageZengqiang = new QAction(DemoRibbonWindow::createIcon(ImageZengqiang), QStringLiteral("影像增强"));
	connect(mImageZengqiang, SIGNAL(triggered()), this, SLOT(slot_ImageZengqianguTriggered()));

	// 复杂环境目标检测插件页
	mrgbDetect = new QAction(DemoRibbonWindow::createIcon(rgbDetect), QStringLiteral("光学影像复杂环境目标检测"));
	connect(mrgbDetect, SIGNAL(triggered()), this, SLOT(slot_rgbDetect()));
	msarDetect = new QAction(DemoRibbonWindow::createIcon(sarDetect), QStringLiteral("SAR影像复杂环境目标检测"));
	connect(msarDetect, SIGNAL(triggered()), this, SLOT(slot_sarDetect()));

	// 异源智能辨识关联插件页
	mrgbsarBianshi = new QAction(DemoRibbonWindow::createIcon(rgbsarBianshi), QStringLiteral("光学与SAR同目标辨识关联"));
	connect(mrgbsarBianshi, SIGNAL(triggered()), this, SLOT(slot_rgbsarBianshi()));

	// 伪装目标检测插件页
	mrgbsarDetect = new QAction(DemoRibbonWindow::createIcon(rgbsarDetect), QStringLiteral("光学|SAR融合伪装目标检测"));
	connect(mrgbsarDetect, SIGNAL(triggered()), this, SLOT(slot_rgbsarDetect()));
	mrgbirDetect = new QAction(DemoRibbonWindow::createIcon(rgbirDetect), QStringLiteral("光学|红外融合伪装目标检测"));
	connect(mrgbirDetect, SIGNAL(triggered()), this, SLOT(slot_rgbirDetect()));
	mrgbsarirDetect = new QAction(DemoRibbonWindow::createIcon(rgbsarirDetect), QStringLiteral("光学|SAR|红外融合伪装目标检测"));
	connect(mrgbsarirDetect, SIGNAL(triggered()), this, SLOT(slot_rgbsarirDetect()));

	// 目标精确定位插件页
	mJinjingLoc = new QAction(DemoRibbonWindow::createIcon(JinjingLoc), QStringLiteral("近景图像目标定位"));
	connect(mJinjingLoc, SIGNAL(triggered()), this, SLOT(slot_JinjingLoc()));
	mUAVLoc = new QAction(DemoRibbonWindow::createIcon(UAVLoc), QStringLiteral("无人机图像目标定位"));
	connect(mUAVLoc, SIGNAL(triggered()), this, SLOT(slot_UAVLoc()));

    
}

void QtGis::createMenuFile()
{
    QIcon iconLogo;
    iconLogo.addPixmap(QPixmap(QStringLiteral(":/QtGis/images/qtitanlogo32x32.png")));
	//添加Project菜单
    QAction* actionFile = ribbonBar()->addSystemButton(iconLogo, tr("&Project"));
    ribbonBar()->systemButton()->setToolButtonStyle(Qt::ToolButtonTextOnly);
    actionFile->setToolTip(tr("Click here to see everything you can do with your document"));
    RibbonSystemMenu* systemMenu = qobject_cast<RibbonSystemMenu*>(actionFile->menu());
	//在菜单下添加相应的按钮
    systemMenu->addAction(mFileNew);
    systemMenu->addAction(mFileOpen);
    systemMenu->addAction(mFileSave);
    systemMenu->addAction(mFileSaveAs);
    systemMenu->addAction(mFileClose);
    systemMenu->addAction(mFileExit);
	//在菜单下最近打开的工程列表控件
    RibbonPageSystemRecentFileList* pageRecentFile = systemMenu->addPageRecentFile(QStringLiteral("Recent Maps"));
	//设置最多展示已打开的9个工程
    pageRecentFile->setSize(9);
	//绑定槽函数
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
	//创建数据获取与预处理模块 Layer
	RibbonPage* FirstPageData = ribbonBar()->addPage(QStringLiteral("数据获取与预处理模块"));
	//创建分组 Layer
    RibbonGroup* DatalayerGroup = FirstPageData->addGroup(QStringLiteral(""));
	//在分组 Layer下添加按钮
    //layerGroup->addAction(mAddRasterLayer, Qt::ToolButtonTextUnderIcon);
    //layerGroup->addAction(mAddVectorLayer, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageOpen, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageJinzita, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageCut, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageLashen, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageLvbo, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageZhifangtu, Qt::ToolButtonTextUnderIcon);
	DatalayerGroup->addAction(mImageZengqiang, Qt::ToolButtonTextUnderIcon);

	//复杂环境下目标检测模块 Layer
	RibbonPage* SecondPageData = ribbonBar()->addPage(QStringLiteral("复杂环境下目标检测模块"));

	RibbonGroup* RGBSARlayerGroup = SecondPageData->addGroup(QStringLiteral(""));
	RGBSARlayerGroup->addAction(mrgbDetect, Qt::ToolButtonTextUnderIcon);
	//RibbonGroup* SARlayerGroup = SecondPageData->addGroup(QStringLiteral(""));
	RGBSARlayerGroup->addAction(msarDetect, Qt::ToolButtonTextUnderIcon);


	//创建数据获取与预处理模块 Layer
	RibbonPage* ThirdtPageData = ribbonBar()->addPage(QStringLiteral("异源目标智能辨识关联模块"));
	//创建分组 Layer
	RibbonGroup* BianshilayerGroup = ThirdtPageData->addGroup(QStringLiteral(""));
	BianshilayerGroup->addAction(mrgbsarBianshi, Qt::ToolButtonTextUnderIcon);


	//创建数据获取与预处理模块 Layer
	RibbonPage* FourthPageData = ribbonBar()->addPage(QStringLiteral("伪装目标检测模块"));
	//创建分组 Layer
	RibbonGroup* RSlayerGroup = FourthPageData->addGroup(QStringLiteral(""));
	RSlayerGroup->addAction(mrgbsarDetect, Qt::ToolButtonTextUnderIcon);
	RibbonGroup* RIlayerGroup = FourthPageData->addGroup(QStringLiteral(""));
	RIlayerGroup->addAction(mrgbirDetect, Qt::ToolButtonTextUnderIcon);
	RibbonGroup* RSIlayerGroup = FourthPageData->addGroup(QStringLiteral(""));
	RSIlayerGroup->addAction(mrgbsarirDetect, Qt::ToolButtonTextUnderIcon);


	//创建数据获取与预处理模块 Layer
	RibbonPage* FifthPageData = ribbonBar()->addPage(QStringLiteral("目标精确定位模块"));
	//创建分组 Layer
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

	//建立画布与状态栏进度条之间的信号槽
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
	mStatusBar->showMessage(QStringLiteral("已就绪"));

}

void QtGis::createModuleWidget()
{
	// 数据预处理模块
	mBuildPyramidsWidget = new BuildPyramidsWidget();
	mImgCutWidget = new ImgCutWidget();
	mImgLashenWidget = new ImgLashenWidget();
	mImgLvboWidget = new ImgLvboWidget();
	mImgZhifangtuWidget = new ImgZhifangtuWidget();
	mImgZengqiangWidget = new ImgZengqiangWidget();

	// 模型
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
	//新建图层QgsDockWidget
	mLayerMapDock = new QgsDockWidget(this);
	mLayerMapDock->setWindowTitle(QStringLiteral("图层视图"));
	mLayerMapDock->setObjectName(QStringLiteral("图层视图"));
	//将图层QgsDockWidget 添加在左侧展示
	addDockWidget(Qt::LeftDockWidgetArea, mLayerMapDock);

	mAttributeDock = new QgsDockWidget(this);
	mAttributeDock->setWindowTitle(QStringLiteral("属性视图"));
	mAttributeDock->setObjectName(QStringLiteral("属性视图"));
	addDockWidget(Qt::RightDockWidgetArea, mAttributeDock);

}

void QtGis::createLayerTreeView()
{
	//新建图层树形控件
	mLayerTreeView = new QgsLayerTreeView(this);
	mLayerTreeView->setObjectName(QStringLiteral("theLayerTreeView"));

	//创建QgsLayerTreeModel 实例,并允许用户通过点选复选框手动隐藏/显示图层
	QgsLayerTreeModel* model = new QgsLayerTreeModel(QgsProject::instance()->layerTreeRoot(), this);
	model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
	model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
	model->setFlag(QgsLayerTreeModel::AllowNodeRename);
	model->setFlag(QgsLayerTreeModel::ShowLegendAsTree);
	model->setFlag(QgsLayerTreeModel::UseEmbeddedWidgets);
	model->setFlag(QgsLayerTreeModel::UseTextFormatting);
	model->setAutoCollapseLegendNodes(10);

	//将View视图与Model数据绑定
	mLayerTreeView->setModel(model);

	//将工程实例中的图层根节点与画布绑定，实现与地图画布控件的数据交互
	mLayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge(QgsProject::instance()->layerTreeRoot(), mMapCanvas, this);

	//添加图层组
	QAction* addGroup = new QAction(tr("Add Group"), this);
	addGroup->setIcon(createIcon(Add_Group));
	addGroup->setToolTip(tr("Add Group"));
	connect(addGroup, &QAction::triggered, mLayerTreeView->defaultActions(), &QgsLayerTreeViewDefaultActions::addGroup);
	//展开所有图层组
	QAction* actionExpandAll = new QAction(tr("Expand All"), this);
	actionExpandAll->setIcon(createIcon(Expand_All));
	actionExpandAll->setToolTip(tr("Expand All"));
	connect(actionExpandAll, &QAction::triggered, mLayerTreeView, &QgsLayerTreeView::expandAllNodes);
	//折叠所有图层组
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

	//设置点击右键出现菜单项
	mLayerTreeView->setMenuProvider(new QtGisLayerTreeViewMenuProvider(mLayerTreeView, mMapCanvas));

	//设置点击左键出现属性
	connect(mLayerTreeView, &QTreeView::clicked, this, &QtGis::onLayerTreeViewClicked);

	//将View添加到左侧的Dock控件中展示
	mLayerMapDock->setWidget(w);

	connect(mLayerTreeView, &QgsLayerTreeView::currentLayerChanged, this, &QtGis::onActiveLayerChanged);
	connect(QgsProject::instance()->layerTreeRegistryBridge(), &QgsLayerTreeRegistryBridge::addedLayersToLayerTree, this, &QtGis::autoSelectAddedLayer);
}


void QtGis::createAttributeTreeView()
{

	// 创建 MyAttribute 实例并设置为属性视图的内容
	attributeWidget = new ImgAttribute(this);
	mAttributeDock->setWidget(attributeWidget);

	//将View添加到左侧的Dock控件中展示
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
			// 尝试从图层的源中获取文件路径
			QString uri = layer->dataProvider()->dataSourceUri();
			//QString uri = layerData["url"].toString();

			//// 尝试解析 URL 来获取文件路径
			//QUrl url(uri);
			//if (url.isLocalFile())
			//{
			//	filePath = url.toLocalFile();
			//}
			//else
			//{
			//	// 如果 URL 不是本地文件，直接使用 URI
			//	filePath = uri;
			//}

			// 更新属性视图
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
	mStatusBar->showMessage(QStringLiteral("渲染中..."));
}

void QtGis::canvasRefreshFinished()
{
	mStatusBar->showMessage(QStringLiteral("已就绪"));
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
