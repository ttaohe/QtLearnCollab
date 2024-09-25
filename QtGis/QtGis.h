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

    //����ˢ��
      /**
     * @brief slot--������ʼˢ��
    */
    void canvasRefreshStarted();
    /**
     * @brief slot--����ֹͣˢ��
    */
    void canvasRefreshFinished();
    /**
     * @brief ���û�����ת
    */
    void userRotation();
    /**
    * @brief ����״̬���еĵ�ͼ��ת�Ƕ�
    */
    void showRotation();

    void showScale();

    /**
     * @brief ˢ�»���
    */
    void refreshMapCanvas(bool redrawAllLayers = false);

    void markDirty();
    /**
     * @brief �ı䵱ǰ����ͼ��
    */
    void onActiveLayerChanged(QgsMapLayer* layer);

    void onLayerTreeViewClicked();
    
    void autoSelectAddedLayer(QList<QgsMapLayer*> layers);
    /**
     * @brief �����µ�ͼ�����ʱ��ͼ���б��λ��
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

    // ģ�Ͳۺ���
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

    QgsLayerTreeView* layerTreeView() const { return mLayerTreeView; }  // �ṩ���ʷ���



private:

    static QtGis* sInstance;

    //! ��ͼ����
    QgsMapCanvas* mMapCanvas = nullptr;
    //�м�tab�ؼ�
    QTabWidget* mCenterTabWidget = nullptr;
    //״̬��
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

    // ������ͼ���� ImgAttribute
    ImgAttribute* attributeWidget;

    // �϶�ͼ�㻭�� ImgMapTool����
    ImgMapTool* mImgMapTool;

    QgsMapToolPan* m_toolPan = nullptr;	// �½��ƶ�����

    BuildPyramidsWidget* mBuildPyramidsWidget;
    ImgCutWidget* mImgCutWidget;
    ImgLashenWidget* mImgLashenWidget;
    ImgLvboWidget* mImgLvboWidget;
    ImgZhifangtuWidget* mImgZhifangtuWidget;
    ImgZengqiangWidget* mImgZengqiangWidget;


    // ģ��widget
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

    // ���ӻ���Ŀ������ҳ
    QAction* mrgbDetect = nullptr;
    QAction* msarDetect = nullptr;

    // ��Դ���ܱ�ʶ�������ҳ
    QAction* mrgbsarBianshi = nullptr;
    // αװĿ������ҳ
    QAction* mrgbsarDetect = nullptr;
    QAction* mrgbirDetect = nullptr;
    QAction* mrgbsarirDetect = nullptr;
    // Ŀ�꾫ȷ��λ���ҳ
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
