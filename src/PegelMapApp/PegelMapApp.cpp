// Copyright 2016 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

#include "Map.h"
#include "MapQuickView.h"
#include "Basemap.h"
#include "KmlLayer.h"
#include "WmsLayer.h"

#include "PegelMapApp.h"
#include "DateTimeUtils.h"

using namespace Esri::ArcGISRuntime;

PegelMapApp::PegelMapApp(QQuickItem *parent /* = nullptr */):
    QQuickItem(parent)
{
}

PegelMapApp::~PegelMapApp()
{
}

void PegelMapApp::componentComplete()
{
    QQuickItem::componentComplete();

    // find QML MapView component
    m_mapView = findChild<MapQuickView*>("mapView");

    // Create a map using the openStreetMap Basemap
    m_map = new Map(Basemap::openStreetMap(this), this);

    // Add a WMS layer
    //addWmsLayer("https://maps.dwd.de/geoserver/dwd/wms", {"Gewaesser", "RBSN_RR"});
    //addWmsLayer("http://www.pegelonline.wsv.de/webservices/gis/wms/aktuell/mnwmhw", {"PegelonlineWMS"});
    //addWmsLayer("https://maps.dwd.de/geoserver/dwd/wms", {"Niederschlagsradar"});
    QDateTime currentDateTime = QDateTime::currentDateTimeUtc();
    QDateTime dateTime = DateTimeUtils::roundNearestFiveMinutesDown(currentDateTime);
    addKmlLayer("https://maps.dwd.de/geoserver/dwd/wms/kml?layers=dwd%3ANiederschlagsradar", &dateTime);

    //const QUrl wmsServiceUrl("https://maps.dwd.de/geoserver/dwd/ows?request=GetCapabilities&service=WMS");
    //const QUrl wmsServiceUrl("https://nowcoast.noaa.gov/arcgis/services/nowcoast/radar_meteo_imagery_nexrad_time/MapServer/WMSServer?request=GetCapabilities&service=WMS");
    //const QStringList layerNames{"Biowettervorhersage"};
    //const QStringList layerNames{"1"};

    /*
    QMap<QString, QString> wmsParameters;
    QDateTime dateTime = QDateTime::currentDateTimeUtc().addDays(-1);//.addSecs(-1 * 60 * 60);
    QString timeParameterValue = dateTime.toString(Qt::ISODate);
    qDebug() << timeParameterValue;
    wmsParameters.insert("TIME", timeParameterValue);
    wmsLayer->setCustomParameters(wmsParameters);
    */

    // Set map to map view
    m_mapView->setMap(m_map);
}

void PegelMapApp::addKmlLayer(const QString &kmlServiceUri, QDateTime *time)
{
    KmlDataset *kmlDataset = nullptr;

    // Add time restriction
    if (nullptr != time)
    {
        QString timeParameterValue = time->toString(Qt::ISODate);
        qDebug() << timeParameterValue;

        QString timeEnabledkmlServiceUri = QString(kmlServiceUri).append("&time=%1").arg(timeParameterValue);
        qDebug() << timeEnabledkmlServiceUri;
        kmlDataset = new KmlDataset(QUrl(timeEnabledkmlServiceUri), this);
    }
    else
    {
        kmlDataset = new KmlDataset(QUrl(kmlServiceUri), this);
    }

    KmlLayer *kmlLayer = new KmlLayer(kmlDataset, this);
    connect(kmlLayer, &KmlLayer::loadStatusChanged, [kmlLayer] (LoadStatus loadStatus)
    {
        switch (loadStatus)
        {
        case LoadStatus::Loaded:
            {
                qDebug() << kmlLayer->description();
                qDebug() << "Time enabled: " << kmlLayer->isSupportsTimeFiltering();
                qDebug() << kmlLayer->fullTimeExtent().startTime();
                qDebug() << kmlLayer->fullTimeExtent().endTime();
            }
            break;

        default:
             break;
        }
    });

    m_map->operationalLayers()->append(kmlLayer);
}

void PegelMapApp::addWmsLayer(const QString &wmsServiceUri, const QStringList &layerNames, QDateTime *time)
{
    WmsLayer *wmsLayer = new WmsLayer(QUrl(wmsServiceUri), layerNames, this);
    connect(wmsLayer, &WmsLayer::loadStatusChanged, [wmsLayer] (LoadStatus loadStatus)
    {
        switch (loadStatus)
        {
        case LoadStatus::Loaded:
            {
                const QList<WmsLayerInfo> layerInfos = wmsLayer->layerInfos();
                foreach (const WmsLayerInfo &layerInfo, layerInfos)
                {
                    qDebug() << layerInfo.description();
                }
            }
            break;

        default:
             break;
        }
    });

    // Add time restriction
    QMap<QString, QString> wmsParameters;
    if (nullptr != time)
    {
        QString timeParameterValue = time->toString(Qt::ISODate);
        qDebug() << timeParameterValue;
        wmsParameters.insert("TIME", timeParameterValue);
        wmsLayer->setCustomParameters(wmsParameters);
    }

    m_map->operationalLayers()->append(wmsLayer);
}
