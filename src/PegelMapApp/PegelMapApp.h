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

#ifndef PEGELMAPAPP_H
#define PEGELMAPAPP_H

namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapQuickView;
}
}

#include <QQuickItem>

class PegelMapApp : public QQuickItem
{
    Q_OBJECT

public:
    PegelMapApp(QQuickItem* parent = nullptr);
    ~PegelMapApp();

    void componentComplete() override;

private:
    void addKmlLayer(const QString &kmlServiceUri);
    void addWmsLayer(const QString &wmsServiceUri, const QStringList &layerNames, QDateTime *time = nullptr);

    Esri::ArcGISRuntime::Map*             m_map = nullptr;
    Esri::ArcGISRuntime::MapQuickView*    m_mapView = nullptr;
};

#endif // PEGELMAPAPP_H
