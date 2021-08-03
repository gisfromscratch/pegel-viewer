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
#include "WmsLayer.h"

#include "PegelMapApp.h"

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
    addWmsLayer("http://www.pegelonline.wsv.de/webservices/gis/wms/aktuell/mnwmhw?request=GetCapabilities&service=WMS&version=1.1.1", {"PegelonlineWMS"});
    addWmsLayer("https://maps.dwd.de/geoserver/dwd/wms?request=GetCapabilities&service=WMS&version=1.1.0", {"Gewaesser"});

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

void PegelMapApp::addWmsLayer(const QString &wmsServiceUri, const QStringList &layerNames)
{
    WmsLayer* wmsLayer = new WmsLayer(QUrl(wmsServiceUri), layerNames, this);
    m_map->operationalLayers()->append(wmsLayer);
}
