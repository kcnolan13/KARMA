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
//new OpenLayers.Control.KeyboardDefaults(),
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

//draw a track, blue font
var lineLayer = new OpenLayers.Layer.Vector("Line Layer"); 

map.addLayer(lineLayer);                    
map.addControl(new OpenLayers.Control.DrawFeature(lineLayer, OpenLayers.Handler.Path));                                     
var points = new Array(
   new OpenLayers.Geometry.Point(-82.488, 27.858).transform(new OpenLayers.Projection("EPSG:4326"), map.getProjectionObject()),
   new OpenLayers.Geometry.Point(-82.498, 27.868).transform(new OpenLayers.Projection("EPSG:4326"), map.getProjectionObject())
);

var line = new OpenLayers.Geometry.LineString(points);

var style = { 
  strokeColor: '#0000ff', 
  strokeOpacity: 0.5,
  strokeWidth: 5
};

var lineFeature = new OpenLayers.Feature.Vector(line, null, style);
lineLayer.addFeatures([lineFeature]);
//draw a track, blue font

map.addControl(new OpenLayers.Control.LayerSwitcher());
