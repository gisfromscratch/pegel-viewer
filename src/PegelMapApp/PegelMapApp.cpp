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

PegelMapApp::PegelMapApp(QQuickItem* parent /* = nullptr */):
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
    const QUrl wmsServiceUrl("http://www.pegelonline.wsv.de/webservices/gis/wms/aktuell/mnwmhw?request=GetCapabilities&service=WMS&version=1.1.1");
    const QStringList layerNames{"PegelonlineWMS"};
    WmsLayer* wmsLayer = new WmsLayer(wmsServiceUrl, layerNames, this);
    m_map->operationalLayers()->append(wmsLayer);

    // Set map to map view
    m_mapView->setMap(m_map);
}
