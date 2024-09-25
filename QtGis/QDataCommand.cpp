#include "QtGis.h"
#include "qgsguiutils.h"
#include "qgsrasterpyramid.h"
#include "qgsrasterpyramidsoptionswidget.h"
void QtGis::slot_ImageJinzitaTriggered()
{
    
    //if (!mBuildPyramidsWidget->isVisible()) {
    //    mBuildPyramidsWidget->show();
    //}
    //else {
    //    mBuildPyramidsWidget->raise();
    //    mBuildPyramidsWidget->activateWindow();
    //}
    
    mBuildPyramidsWidget->show();
    //mPyramidsOptionsWidget->show();
    
    
}

void QtGis::slot_ImageCutTriggered()
{
    mImgCutWidget->show();
}

void QtGis::slot_ImageLashenTriggered()
{
    mImgLashenWidget->show();
}

void QtGis::slot_ImageLvboTriggered()
{
    mImgLvboWidget->show();
}

void QtGis::slot_ImageZhifangtuTriggered()
{
    mImgZhifangtuWidget->show();
}

void QtGis::slot_ImageZengqianguTriggered()
{
    mImgZengqiangWidget->show();
}

