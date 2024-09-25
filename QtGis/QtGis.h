#pragma once

#include <QtWidgets/QMainWindow>
#include "DemoRibbonWindow.h"
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QTableView>
#include "ImgAttribute.h"
#include "ImgMapTool.h"
#include "BuildPyramidsWidget.h"
#include "qgsrasterpyramidsoptionswidget.h"
#include "ImgCut.h"
#include "ImgLashenWidget.h"
#include "ImgLvboWidget.h"
#include "ImgZhifangtu.h"
#include "ImgZengqiangWidget.h"

#include "rgbDetectWidget.h"
#include "sarDetectWidget.h"
#include "rgbsarBianshiWidget.h"
#include "rgbsarDetectWidget.h"
#include "rgbirDetectWidget.h"
#include "rgbsarirDetectWidget.h"
#include "JinjingLocWidget.h"
#include "UAVLocWidget.h"

#include <qgsmaptoolpan.h>

class QToolButton;
class QTabWidgte;

class QgsMapCanvas;
class QgsDoubleSpinBox;
class QgsStatusBar;
class QProgressBar;
class QgsTaskManagerStatusBarWidget;
class QgsStatusBarCoordinatesWidget;
class QgsStatusBarScaleWidget;
class QgsStatusBarMagnifierWidget;
class QgsDockWidget;
class QgsLayerTreeView;
class QgsLayerTreeMapCanvasBridge;
class QgsMapLayer;

class QtGis : public DemoRibbonWindow
{
    Q_OBJECT

public:
    QtGis(QWidget *parent = Q_NULLPTR);

    ~QtGis();

    static QtGis* Instance() { return sInstance; }

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;

    void dropEvent(QDropEvent* event) override;
private:
    void createMapCanvas();
    void createActions();
    void createMenuFile();
    void createQuickAccessBar();
    void createRibbonBar();
    void createStatusBar();
    void createDockWidget();
    void createModuleWidget();
    void createLayerTreeView();

    void createAttributeTreeView();

    QList<QgsMapCanvas*> mapCanvases();

    /**
    * Freezes all map canvases (or thaws them if the \a frozen argument is false).
    */
    void freezeCanvases(bool frozen = true);

    QgsMapCanvas* mapCanvas();

private slots:
    void slot_fileSaveAs();
    void slot_fileNew();
    void slot_fileOpen();
    void slot_fileSave();
    void slot_fileClose();
    void slot_fileExit();
    void openRecentFile(const QString& file);
    void updateRecentFileActions(const QStringList&);
    void projectCrsChanged();

    //画布刷新
      /**
     * @brief slot--画布开始刷新
    */
    void canvasRefreshStarted();
    /**
     * @brief slot--画布停止刷新
    */
    void canvasRefreshFinished();
    /**
     * @brief 设置画布旋转
    */
    void userRotation();
    /**
    * @brief 更新状态栏中的地图旋转角度
    */
    void showRotation();

    void showScale();

    /**
     * @brief 刷新画布
    */
    void refreshMapCanvas(bool redrawAllLayers = false);

    void markDirty();
    /**
     * @brief 改变当前激活图层
    */
    void onActiveLayerChanged(QgsMapLayer* layer);

    void onLayerTreeViewClicked();
    
    void autoSelectAddedLayer(QList<QgsMapLayer*> layers);
    /**
     * @brief 更新新的图层加载时在图层列表的位置
    */
    void updateNewLayerInsertionPoint();



public slots:
    void slot_addRaterLayer();

    void slot_addVectorLayer();

    // hetao 
    void slot_addImageLayer();

    void slot_ImageJinzitaTriggered();
    void slot_ImageCutTriggered();
    void slot_ImageLashenTriggered();
    void slot_ImageLvboTriggered();
    void slot_ImageZhifangtuTriggered();
    void slot_ImageZengqianguTriggered();

    // 模型槽函数
    void slot_rgbDetect();
    void slot_sarDetect();
    void slot_rgbsarBianshi();
    void slot_rgbsarDetect();
    void slot_rgbsarirDetect();
    void slot_rgbirDetect();
    void slot_JinjingLoc();
    void slot_UAVLoc();

public:
    bool addRaterLayer(const QStringList&);

    bool addVectorLayer(const QStringList&);

    QgsLayerTreeView* layerTreeView() const { return mLayerTreeView; }  // 提供访问方法



private:

    static QtGis* sInstance;

    //! 地图画布
    QgsMapCanvas* mMapCanvas = nullptr;
    //中间tab控件
    QTabWidget* mCenterTabWidget = nullptr;
    //状态栏
    QgsStatusBar* mStatusBar = nullptr;
    QProgressBar* mProgressBar = nullptr;
    QgsTaskManagerStatusBarWidget* mTaskManagerWidget = nullptr;
    QgsStatusBarCoordinatesWidget* mCoordsEdit = nullptr;
    QgsStatusBarScaleWidget* mScaleWidget = nullptr;
    QgsStatusBarMagnifierWidget* mMagnifierWidget = nullptr;
    QLabel* mRotationLabel = nullptr;
    QgsDoubleSpinBox* mRotationEdit = nullptr;
    QToolButton* mOnTheFlyProjectionStatusButton = nullptr;

    //Dock
    QgsDockWidget* mAttributeDock = nullptr;
    QgsDockWidget* mLayerMapDock = nullptr;
    QgsLayerTreeView* mLayerTreeView = nullptr;
    QgsLayerTreeMapCanvasBridge* mLayerTreeCanvasBridge = nullptr;

    // 属性视图声明 ImgAttribute
    ImgAttribute* attributeWidget;

    // 拖动图层画布 ImgMapTool声明
    ImgMapTool* mImgMapTool;

    QgsMapToolPan* m_toolPan = nullptr;	// 新建移动工具

    BuildPyramidsWidget* mBuildPyramidsWidget;
    ImgCutWidget* mImgCutWidget;
    ImgLashenWidget* mImgLashenWidget;
    ImgLvboWidget* mImgLvboWidget;
    ImgZhifangtuWidget* mImgZhifangtuWidget;
    ImgZengqiangWidget* mImgZengqiangWidget;


    // 模型widget
    rgbDetectWidget* mrgbDetectWidget;
    sarDetectWidget* msarDetectWidget;
    rgbsarBianshiWidget* mrgbsarBianshiWidget;
    rgbsarDetectWidget* mrgbsarDetectWidget;
    rgbirDetectWidget* mrgbirDetectWidget;
    rgbsarirDetectWidget* mrgbsarirDetectWidget;
    JinjingLocWidget* mJinjingLocWidget;
    UAVLocWidget* mUAVLocWidget;


    QgsRasterPyramidsOptionsWidget* mPyramidsOptionsWidget;

    //Actions Project
    QAction* mFileNew = nullptr;
    QAction* mFileOpen = nullptr;
    QAction* mFileSave = nullptr;
    QAction* mFileSaveAs = nullptr;
    QAction* mFileClose = nullptr;
    QAction* mFileExit = nullptr;

    //Actions Layer
    QAction* mAddRasterLayer = nullptr;
    QAction* mAddVectorLayer = nullptr;
    QAction* mImageOpen = nullptr;
    QAction* mImageJinzita = nullptr;
    QAction* mImageCut = nullptr;
    QAction* mImageLashen = nullptr;
    QAction* mImageLvbo = nullptr;
    QAction* mImageZhifangtu = nullptr;
    QAction* mImageZengqiang = nullptr;

    // 复杂环境目标检测插件页
    QAction* mrgbDetect = nullptr;
    QAction* msarDetect = nullptr;

    // 异源智能辨识关联插件页
    QAction* mrgbsarBianshi = nullptr;
    // 伪装目标检测插件页
    QAction* mrgbsarDetect = nullptr;
    QAction* mrgbirDetect = nullptr;
    QAction* mrgbsarirDetect = nullptr;
    // 目标精确定位插件页
    QAction* mJinjingLoc = nullptr;
    QAction* mUAVLoc = nullptr;



    QString mRasterFileFilter;
    QString mVectorFileFilter;

    class QgsCanvasRefreshBlocker
    {
    public:

        QgsCanvasRefreshBlocker()
        {
            if (QtGis::Instance()->mFreezeCount++ == 0)
            {
                // going from unfrozen to frozen, so freeze canvases
                QtGis::Instance()->freezeCanvases(true);
            }
        }
        QgsCanvasRefreshBlocker(const QgsCanvasRefreshBlocker& other) = delete;
        QgsCanvasRefreshBlocker& operator=(const QgsCanvasRefreshBlocker& other) = delete;

        void release()
        {
            if (mReleased)
                return;

            mReleased = true;
            if (--QtGis::Instance()->mFreezeCount == 0)
            {
                // going from frozen to unfrozen, so unfreeze canvases
                QtGis::Instance()->freezeCanvases(false);
                QtGis::Instance()->refreshMapCanvas();
            }
        }

        ~QgsCanvasRefreshBlocker()
        {
            if (!mReleased)
                release();
        }

    private:

        bool mReleased = false;
    };
    int mFreezeCount = 0;
    friend class QgsCanvasRefreshBlocker;
};
