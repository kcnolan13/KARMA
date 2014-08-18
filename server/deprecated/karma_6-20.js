//  ----- GLOBAL VARS ------ //

var host = "/home/iansears/kyle/web/";
var kmlUrl = host+"kml-track.kml?key="+Math.random();

//something to hold the track so it can be referenced easily
var myTrack;
var barbFeatures = [];
var barbIntervalMin = 15;
var refreshNum = 0;
var initZoom = 10;

//synchronize wind barb interval controls
var barbInterval = 120;
document.getElementById("barbInterval").value = 120;

var barbCount = 0;

//let's start by limiting the number of popups on the screen to one
var thePopup = null;

var wgs_1984 = new OpenLayers.Projection("EPSG:4326");
var spherical_mercator = new OpenLayers.Projection("EPSG:900913");

//set up the mouse position to display the right kind of lat/lon
var mouseControl = new OpenLayers.Control.MousePosition(); 
mouseControl.displayProjection = wgs_1984;

var mapScale = new OpenLayers.Control.ScaleLine({
                minWidth: 75,
                maxWidth: 350
            });

// ------ EXPERIMENTAL ------ //
        /*var vLayer = new OpenLayers.Layer.Vector( "Editable" );

        var zb = new OpenLayers.Control.ZoomBox(
                        {title:"Zoom box: Selecting it you can zoom on an area by clicking and dragging."});

        var panel = new OpenLayers.Control.Panel({defaultControl: zb});
        panel.addControls([
            zb,
            new OpenLayers.Control.DrawFeature(vLayer, OpenLayers.Handler.Path,
                {title:'Draw a feature'})
        ]);*/

// ---- END GLOBAL VARS ---- //

// ------ Filters ------ //
/*var barbFilter = new OpenLayers.Filter.Comparison({
    type: OpenLayers.Filter.Comparison.EQUAL_TO,
    property: "index",
    value: 0
});

var barbStrategy = new OpenLayers.Strategy.Filter({filter: barbFilter});*/



//CREATE THE MAP
var map = new OpenLayers.Map({
    div: "map",
    layers: [
        //vLayer,
        new OpenLayers.Layer.OSM("OSM (without buffer)"),
        new OpenLayers.Layer.OSM("OSM (with buffer)", null, {buffer: 2})
    ],
    //define map controls
    controls: [
        new OpenLayers.Control.Navigation({
            dragPanOptions: {
                enableKinetic: true
            }
        }),
        //panel,
        new OpenLayers.Control.PanZoomBar(),
        mouseControl,
        mapScale,
        new OpenLayers.Control.KeyboardDefaults(),
        new OpenLayers.Control.Attribution()
    ]
});

OpenLayers.IMAGE_RELOAD_ATTEMPTS = 3;

map.addControl(new OpenLayers.Control.LayerSwitcher());

//create the layer that will hold the aircraft track
var kmlLayer = new OpenLayers.Layer.Vector("Aircraft Track", {
                projection: wgs_1984,
                strategies: [new OpenLayers.Strategy.Fixed()],
                protocol: new OpenLayers.Protocol.HTTP({
                    //set the url to our own var
                    url: kmlUrl,
                    //format this layer as KML//
                    format: new OpenLayers.Format.KML({
                        //maxDepth is how deep it will follow network links
                        maxDepth: 2,
                        extractStyles: true,
                        extractAttributes: true
                    })
                })
            });

//enable feature selection for the KML Layer
var selectControl = new OpenLayers.Control.SelectFeature(
                kmlLayer
                ,{
                    callbacks:
                    {
                          click: onFeatureSelect
                    }
                }
            );
    //add the feature selection control to the map and then activate it
    map.addControl(selectControl);
    selectControl.activate();

map.addLayer(kmlLayer);

window.setInterval(refreshTrack, 2000, kmlLayer);

// ----------- FUNCTION DEFINITIONS ------------ //

//settings for every time the actrack LineString gets updated
kmlLayer.preFeatureInsert = function(feature) {
  //only do this for LineStrings
  if (feature.geometry.CLASS_NAME=="OpenLayers.Geometry.LineString")
  {
    //hold on to the line string
    myTrack = feature;
    //override the default opacity -- KML is having trouble controlling this directly
    feature.style.strokeOpacity = 1;

    //center the map around the last aircraft point on the first load
    if (refreshNum == 0)
      map.setCenter(new OpenLayers.LonLat(feature.geometry.components[feature.geometry.components.length-1].x,feature.geometry.components[feature.geometry.components.length-1].y),initZoom);
  
    //---- points can now be referenced by myTrack.geometry.components[point_number] ----//
  } else if (feature.attributes.styleUrl.contains("knots"))
  {
    barbCount++;
      //if its stylUrl contains knots, it is definitely a wind barb
      if (barbCount%(barbInterval/barbIntervalMin) == 0)
      {
        barbFeatures.push(feature);
        //feature.style.graphicYOffset = feature.style.graphicHeight/2;
        feature.style.rotation = feature.attributes.heading;
        //console.log("accepting barb #"+barbCount);
      }
  } else 
  {
    feature.style.rotation = feature.attributes.heading;
  }
}

kmlLayer.onFeatureInsert = function(feature) {
  if (feature.attributes.styleUrl.contains("knots")) {
    if (barbCount%(barbInterval/barbIntervalMin) != 0)
      {
        //console.log("blacklisting barb #"+barbCount);
        kmlLayer.removeFeatures(feature);
      }
  }
}

//what to do when you select a KML feature
function onFeatureSelect(feature)
{
  //create a popup if it has a description
  if (feature.attributes.description != null)
  {
    //remove preexisting popups to make them less annoying
    if (thePopup != null)
      map.removePopup(thePopup)

    thePopup = new OpenLayers.Popup.FramedCloud(
      "chicken",
      new OpenLayers.LonLat(feature.geometry.x,feature.geometry.y),
      null,
      '<b>'+feature.attributes.name+'</b><p>'+feature.attributes.description+'</p>',
      null,
      true
      );
    map.addPopup(thePopup);
  }
}

function refreshTrack(layer)
{
  barbCount = 0;
  featureBlackList = [];
  refreshNum++;
  //only refresh if visible. saves overhead and prevents from popping up again
  if (layer.getVisibility()) 
  {
    //setting loaded to false unloads the layer//
    layer.loaded = false;
    //setting visibility to true forces a reload of the layer//
    layer.setVisibility(true);
    //the refresh will force it to get the new KML data//
    layer.refresh({ force: true, params: { 'key': Math.random()} });
  }
}

function Select(obj) //the button that someone clicked
{
    if (obj.id=="barbInterval")
    {
      barbInterval = obj.value;
      //get a new KML
      refreshTrack(kmlLayer);
    }
}