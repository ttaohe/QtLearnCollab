#include "QtGis.h"
#include <QMessageBox>

#include "qgsguiutils.h"
#include "QgsRasterLayer.h"
#include "QgsProject.h"
#include "QgsProviderRegistry.h"
#include "QgsMapCanvas.h"
#include "QgsVectorLayer.h"
#include "qgslayerpropertieswidget.h"


void QtGis::slot_addRaterLayer()
{
	QStringList layerFiles;
	QString e;
	QString title = tr("Open Raster Layer");
	QString rasterFileFilter = "All supported(*.tif *.img *.hgt *.grd);;GeoTIFF(*.tif *.tiff *.TIF *.TIFF);;Erdas Imagine Images(*.img *.IMG);;NetCDF(*.grd *.GRD);;STRMHGT File(*.hgt *.HGT)";
	QgsGuiUtils::openFilesRememberingFilter(QStringLiteral("lastRasterFileFilter"), rasterFileFilter, layerFiles, e, title);
	if (layerFiles.isEmpty())
		return;
	addRaterLayer(layerFiles);

    

}

void QtGis::slot_addImageLayer()
{
    QStringList layerFiles;
    QString e;
    QString title = tr("Open Raster Layer");
    QString rasterFileFilter = 
        "All supported(*.tif *.tiff *.img *.hgt *.grd *.jpeg *.jpg *.png *.bmp *.gif);;"
        "GeoTIFF(*.tif *.tiff *.TIF *.TIFF);;"
        "Erdas Imagine Images(*.img *.IMG);;"
        "NetCDF(*.grd *.GRD);;"
        "STRMHGT File(*.hgt *.HGT);;"
        "JPEG(*.jpeg *.jpg *.JPEG *.JPG);;"
        "PNG(*.png *.PNG);;"
        "BMP(*.bmp *.BMP);;"
        "GIF(*.gif *.GIF)";
    
    QgsGuiUtils::openFilesRememberingFilter(QStringLiteral("lastRasterFileFilter"), rasterFileFilter, layerFiles, e, title);
    if (layerFiles.isEmpty())
        return;
    
    addRaterLayer(layerFiles);


}


void QtGis::slot_addVectorLayer()
{
    QStringList layerFiles;
    QString e;
    QString title = tr("Open Vector Layer");
    QString vectorFileFilter = "OGR Layer(*.shp *.geojson *.gmt);; KML(*.kml)";
    QgsGuiUtils::openFilesRememberingFilter(QStringLiteral("lastRasterFileFilter"), vectorFileFilter, layerFiles, e, title);
    if (layerFiles.isEmpty())
        return;
    addVectorLayer(layerFiles);
}

bool QtGis::addRaterLayer(const QStringList& layerFiles)
{
    if (layerFiles.empty())
    {
        return false;
    }

    QgsCanvasRefreshBlocker refreshBlocker;

    // this is messy since some files in the list may be rasters and others may
    // be ogr layers. We'll set returnValue to false if one or more layers fail
    // to load.
    bool returnValue = true;
    QList<QgsMapLayer*> myList;

    for (auto myIterator = layerFiles.begin();
        myIterator != layerFiles.end();
        ++myIterator)
    {
        QString errMsg;
        bool ok = false;

        //// 暂未添加对压缩文件的支持
        //QString vsiPrefix = QgsZipItem::vsiPrefix(*myIterator);
        //if (!myIterator->startsWith(QLatin1String("/vsi"), Qt::CaseInsensitive) && (vsiPrefix == QLatin1String("/vsizip/") || vsiPrefix == QLatin1String("/vsitar/")))
        //{
        //    continue;
        //}


        if (QgsRasterLayer::isValidRasterFileName(*myIterator, errMsg))
        {
            QFileInfo myFileInfo(*myIterator);

            QString layerName = myFileInfo.completeBaseName();
            
            const QVariantMap uriDetails = QgsProviderRegistry::instance()->decodeUri(QStringLiteral("gdal"), *myIterator);
            if (!uriDetails[QStringLiteral("layerName")].toString().isEmpty())
            {
                layerName = uriDetails[QStringLiteral("layerName")].toString();
            }

            // try to create the layer
            QgsRasterLayer* layer = new QgsRasterLayer(*myIterator, layerName, QStringLiteral("gdal"));
            if (layer && layer->isValid())
            {
                myList << layer;
                ok = true;

                // 创建属性对话框并显示符号学选项卡
                //QgsLayerPropertiesWidget* dialog = new QgsLayerPropertiesWidget(layer);

                //// 设置符号学选项卡为当前选项卡 (假设索引为1)
                //dialog->setCurrentIndex(1); // 如果索引不同，可以修改为正确的索引

                //// 显示对话框
                //dialog->exec();
            }

        } // valid raster filename
        else
        {
            ok = false;
            QString msg = tr("%1 is not a supported raster data source").arg(*myIterator);
            if (!errMsg.isEmpty())
                msg += '\n' + errMsg;

            QMessageBox::warning(this, "Unsupported Data Source", msg);
        }
        if (!ok)
        {
            returnValue = false;
        }
    }
    QgsProject::instance()->addMapLayers(myList);
    markDirty();

    mBuildPyramidsWidget->public_updateLayerComboBox(mLayerTreeView);
    mImgCutWidget->public_updateLayerComboBox(mLayerTreeView);
    mImgLashenWidget->public_updateLayerComboBox(mLayerTreeView);
    mImgLvboWidget->public_updateLayerComboBox(mLayerTreeView);
    mImgZhifangtuWidget->public_updateLayerComboBox(mLayerTreeView);
    mImgZengqiangWidget->public_updateLayerComboBox(mLayerTreeView);
    

    return returnValue;
}

bool QtGis::addVectorLayer(const QStringList& layerFiles)
{
    QgsCanvasRefreshBlocker refreshBlocker;
    QList<QgsMapLayer*> layersToAdd;

    for (QString src : layerFiles)
    {
        src = src.trimmed();
        QFileInfo fi(src);
        QString baseName = fi.completeBaseName();
        // create the layer
        QgsVectorLayer::LayerOptions options{ QgsProject::instance()->transformContext() };
        options.loadDefaultStyle = false;
        QgsVectorLayer* layer = new QgsVectorLayer(src, baseName, QStringLiteral("ogr"), options);

        if (layer != nullptr && layer->isValid())
        {
            layersToAdd << layer;
        }
        else
        {
            delete layer;
            QString msg = tr("%1 is not a valid or recognized data source.").arg(src);
            QMessageBox::warning(this, tr("Invalid Data Source"), msg);
        }
    }

    if (layersToAdd.size() > 0)
    {
        QgsProject::instance()->addMapLayers(layersToAdd);
        markDirty();
    }
    return true;
}
