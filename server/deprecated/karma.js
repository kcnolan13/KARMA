var map = new OpenLayers.Map({
    div: "map",
    layers: [
        new OpenLayers.Layer.OSM("OSM (without buffer)"),
        new OpenLayers.Layer.OSM("OSM (with buffer)", null, {buffer: 2})
    ],
    controls: [
        new OpenLayers.Control.Navigation({
            dragPanOptions: {
                enableKinetic: true
            }
        }),
        new OpenLayers.Control.PanZoom(),
new OpenLayers.Control.MousePosition(),			// stuff I'm trying out
//new OpenLayers.Control.OverviewMap(),
new OpenLayers.Control.KeyboardDefaults(),
        new OpenLayers.Control.Attribution()
    ],
    center: [0, 0],
    zoom: 3
});

//responsible for the red diamond marker
var lonlat = new OpenLayers.LonLat(-82.498,27.868).transform(
            new OpenLayers.Projection("EPSG:4326"), // transform from WGS 1984
            new OpenLayers.Projection("EPSG:900913") // to Spherical Mercator
          );

        var zoom = 13;

        var markers = new OpenLayers.Layer.Markers( "Markers" );
        map.addLayer(markers);
        markers.addMarker(new OpenLayers.Marker(lonlat));

        map.setCenter(lonlat, zoom);
//responsible for the red diamond marker

map.addControl(new OpenLayers.Control.LayerSwitcher());

/*// Add the Layer with the GPX Track
var gpxLayer = new OpenLayers.Layer.Vector("Aircraft Track", {
  strategies: [new OpenLayers.Strategy.Fixed()],
  protocol: new OpenLayers.Protocol.HTTP({
    url: "/home/iansears/kyle/web/actrack.gpx",
    format: new OpenLayers.Format.GPX()
  }),
  style: {strokeColor: "red", strokeWidth: 5, strokeOpacity: 0.5},
  projection: new OpenLayers.Projection("EPSG:4326")
  });

map.addLayer(gpxLayer);
map.addControl(new OpenLayers.Control.DrawFeature(gpxLayer, OpenLayers.Handler.Path));*/

var kmlLayer = new OpenLayers.Layer.PointTrack("Aircraft Tracks");
map.addLayer(kmlLayer);

function updateTrack()
{
  //keep track of visiblity
  var visibility = kmlLayer.getVisibility();

  //remove the main kml layer
  map.removeLayer(kmlLayer);
  delete kmlLayer;

  //recreate the main kml layer
  kmlLayer = new OpenLayers.Layer.PointTrack("Aircraft Tracks", {
                projection: new OpenLayers.Projection("EPSG:4326"),
                strategies: [new OpenLayers.Strategy.Fixed()],
                protocol: new OpenLayers.Protocol.HTTP({
                    url: "/home/iansears/kyle/web/kml-track.kml",
                    format: new OpenLayers.Format.KML({
                        extractTracks: true,
                        extractStyles: true
                    })
                }),
                dataFrom: OpenLayers.Layer.PointTrack.TARGET_NODE,
                styleFrom: OpenLayers.Layer.PointTrack.TARGET_NODE,

                eventListeners: {
                    "beforefeaturesadded": function(e) {
                        // group the tracks by fid and create one track for every fid
                        var fid, points = [], feature;
                        for (var i=0, len=e.features.length; i<len; i++) {
                            feature = e.features[i];
                            if ((fid && feature.fid !== fid) || i === len-1) {
                                this.addNodes(points, {silent: true});
                                points = [];
                            } else {
                                points.push(feature);
                            }
                            fid = feature.fid;
                        }
                        return false;
                    }
                }
  });
  kmlLayer.setVisibility(visibility);
  map.addLayer(kmlLayer);
}

setInterval(updateTrack, 1000);
