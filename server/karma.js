/*
#=================================================================
# APPLICATION NAME: karma.js
#
#
# DESCRIPTION: The In-Browser Graphical User Interface for the KARMA Software Package
#
# ENVIRONMENT PARAMS:
#
# INPUT PARAMETERS: 
#
# OUTPUT PARAMETERS:
#
# AUTHORS: Kyle Nolan
#
# DATE: 2013.07.13
#
# CONTRIBUTING AUTHORS:
#
# REVISIONS: 
# 
#
#  
#================================================================
*/


// ------ CREATE THE DROP-DOWN CONTROLS PANEL ------ //
var dropDownControlPanel = function() {
  this.popupOpacity = 75;
  this.Centering = "Hybrid";
  this.coordinateFormat = "deg.min.sec";
  this.windBarbInterval = "2 min.";
  this.barbPopupColor = "#FFFFFF";
  this.iconStyle = "Aircraft";
  this.aircraftPopupColor = "#FFFFFF";
  this.coloredBarbs = true;
  this.legendOpacity = 50;
  this.zonesOpacity = 10;
}
var userOptions = new dropDownControlPanel();

var gui = new dat.GUI({ autoPlace: false, width: 200});
var customContainer = document.getElementById('theGui');
customContainer.appendChild(gui.domElement);

gui.closed = true;

//add all the folders

  // Wind Barb Controls
    var folderBarbs = gui.addFolder('Wind Barbs');

    var barbIntervalControl = folderBarbs.add(userOptions, 'windBarbInterval', ['1 min.', '2 min.', '4 min.', '5 min.', '10 min.', '15 min.', '20 min.', '30 min.', '45 min.', '1 hr.']);//.step(15));
    barbIntervalControl.onChange(function() {refreshAllKml();});

    var coloredBarbsControl = folderBarbs.add(userOptions, 'coloredBarbs');

  // Aircraft Track Controls
    var folderTrack = gui.addFolder('Track');

    var centeringControl = folderTrack.add(userOptions, 'Centering', ['Aircraft', 'Manual', 'Hybrid']);
    centeringControl.onChange(function() {refreshAllKml();});

    var coordinateControl = folderTrack.add(userOptions, 'coordinateFormat', ['deg.min.sec', 'dec.deg']);
    coordinateControl.onChange(function() {refreshAllKml();});

  // User Interface Controls
    var folderUi = gui.addFolder('Appearance');

    var iconStyleControl = folderUi.add(userOptions, 'iconStyle', ['Aircraft', 'Vector + Barb']);
    iconStyleControl.onChange(function() {refreshAllKml();});

    var opacityControl = folderUi.add(userOptions, 'popupOpacity',0, 100);
    var legendOpacityControl = folderUi.add(userOptions, 'legendOpacity',0, 100);
    legendOpacityControl.onChange(function() { 
      document.getElementsByClassName('olControlAttribution').OpenLayers_Control_Attribution_12.style.opacity = userOptions.legendOpacity/100;
      });
    var zonesOpacityControl = folderUi.add(userOptions, 'zonesOpacity',0, 100);
    zonesOpacityControl.onChange(function() {  refreshZones(zonesLayer);});
    var barbColorControl = folderUi.addColor(userOptions, 'barbPopupColor');
    var planeColorControl = folderUi.addColor(userOptions, 'aircraftPopupColor');
// ------ END DAT GUI SETUP ------ //



// ------ DEFINE OTHER GENERAL GLOBAL VARS ------ //
var host = "./";
var kmlUrl = host+"kml-track.kml?key="+Math.random();
var waypointUrl = host+"kml-waypoints.kml?key="+Math.random();
var waypointTrackUrl = host+"kml-waypoints-track.kml?key="+Math.random();
var landmarkUrl = host+"kml-landmarks.kml?key="+Math.random();
var zonesUrl = host+"zones.kml?key="+Math.random();

//something to hold the track so it can be referenced easily
var myTrack;
var clickableFeatures = [];
var importedBarbs = [];
var selectedBarbIndices = [[-1,-1], [-1,-1]]; //first is importedBarbs index, second is thePopups index that maps to this barb
var featureBlackList = [];
var refreshNum = 0;
var initZoom = 10;
var mouse_lon = 0;
var mouse_lat = 0;
var kmlRefreshInterval = 2000;
var waypointFeatures = [];
var landmarkFeatures = [];
var waypointTrack = null;
var hoverPopup = null;
var hoverFeature = null;
var zoneFeatures = [];

//synchronize wind barb interval controls
var barbInterval = 120;
const barbIntervalMin = 60;

    //synchronize parameter controls
    const numBarbParams = 6;
    const numPlaneParams = numBarbParams;
    var barbParams = [];
    var barbValues = [];
    var barbUnitConventions = [];
    var planeParams = [];
    var planeValues = [];
    var planeUnitConventions = [];

    var planeParamDefaults = ["ALT", "TRACK", "CTR", "CC", "<none>", "<none>"];
    var barbParamDefaults = ["WS", "WD", "DEWPT", "PRESS_ALT", "<none>", "<none>"];

    var masterUnitsList = [["latref","LAT","degs"],["lonref","LON","degs"],["altref","ALT","m"],["psmref","STATIC_PRESS","mb"],["psurf_d","EXTRAP_SLP","mb"],
    ["asfmrws_1","aSFMR_WS","m/s"],["asfmrrainrate_1","aSFMR_RR","mm/hr"],["ws_d","WS","m/s"],["wd_d","WD","deg"],["ta_d","ATMP", "C"],["td_c","DEWPT","C"],
    ["sfmrws_r","pSFMR_WS","m/s"],["sfmrrainrate_r","pSFMR_RR","mm/hr"],["uwz_d","VERT_WIND","m/s"],["gs_d","GROUND_SPD","m/s"],["tas_d","TRUE_AIR_SPD","m/s"],
    ["ht_d","STAND_HT","m"],["altpa_d","PRESS_ALT","m"],["da_d","DRIFT_ANGLE","deg"],["trk_d","TRACK","deg"],["trkdesired_d","CTR","deg"],["coursecorr_d","CC","deg"]];
    
    //set the default to metric and pick default parameters
    var popupOpacity = 75;
    var barbPopupColor = "#FFFFFF";
    var aircraftPopupColor = "#FFFFFF";

    var centering = "Hybrid";
    var lonlatConvention = "metric";
    initUserParamSettings();

    //update the requested parameters list
    updateUserSpecifiedVars();
    //unit conversion constants
    const MSEC2KNOTS = 1.94384;
    const M2FEET = 3.28084;


var barbCount = 0;

//let's start by limiting the number of popups on the screen to one
var thePopups = [null, null];
var theAircraft = null;

var wgs_1984 = new OpenLayers.Projection("EPSG:4326");
var spherical_mercator = new OpenLayers.Projection("EPSG:900913");

// ------ CUSTOM CONTROL DEFINITIONS ------//
//set up the on-screen mouse position to display the right kind of lat/lon
var mouseControl = new OpenLayers.Control.MousePosition(); 
mouseControl.displayProjection = wgs_1984;

var mapScale = new OpenLayers.Control.ScaleLine({
                minWidth: 75,
                maxWidth: 350
            });


// ------ CREATE THE MAP ------ //
var map = new OpenLayers.Map({
    div: "map",
    transitionEffect: null,
    zoomMethod: null,
    //define map layers
    layers: [
        new OpenLayers.Layer.OSM("OSM (with buffer)", null, {buffer: 2, attribution: "<img src='./icons/barbLegend.png'/>"})
    ],
    //define map controls
    controls: [
        new OpenLayers.Control.Navigation({
            dragPanOptions: {
                enableKinetic: true
            }
        }),
        new OpenLayers.Control.PanZoomBar(),
        mouseControl,
        mapScale,
        new OpenLayers.Control.LayerSwitcher(),
        new OpenLayers.Control.KeyboardDefaults(),
        new OpenLayers.Control.Attribution()
    ]
});

//HANDLE TILES THAT DON'T RENDER PROPERLY
OpenLayers.IMAGE_RELOAD_ATTEMPTS = 5;
OpenLayers.Util.onImageLoadErrorColor = "transparent";
OpenLayers.Util.onImageLoadError = function() {
  this.src = "./icons/loading.jpg";
  this.style.display = "";
};


// ------ CREATE THE KML LAYER FOR THE ACTRACK OVERLAY AND THE ONE FOR WAYPOINTS AS WELL ------ //
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

var waypointLayer = new OpenLayers.Layer.Vector("Drops", {
                projection: wgs_1984,
                strategies: [new OpenLayers.Strategy.Fixed()],
                protocol: new OpenLayers.Protocol.HTTP({
                    //set the url to our own var
                    url: waypointUrl,
                    //format this layer as KML//
                    format: new OpenLayers.Format.KML({
                        //maxDepth is how deep it will follow network links
                        maxDepth: 2,
                        extractStyles: true,
                        extractAttributes: true
                    })
                })
            });

var waypointTrackLayer = new OpenLayers.Layer.Vector("Drops Track", {
                projection: wgs_1984,
                strategies: [new OpenLayers.Strategy.Fixed()],
                protocol: new OpenLayers.Protocol.HTTP({
                    //set the url to our own var
                    url: waypointTrackUrl,
                    //format this layer as KML//
                    format: new OpenLayers.Format.KML({
                        //maxDepth is how deep it will follow network links
                        maxDepth: 2,
                        extractStyles: true,
                        extractAttributes: true
                    })
                })
            });

var landmarkLayer = new OpenLayers.Layer.Vector("Landmarks", {
                projection: wgs_1984,
                strategies: [new OpenLayers.Strategy.Fixed()],
                protocol: new OpenLayers.Protocol.HTTP({
                    //set the url to our own var
                    url: landmarkUrl,
                    //format this layer as KML//
                    format: new OpenLayers.Format.KML({
                        //maxDepth is how deep it will follow network links
                        maxDepth: 2,
                        extractStyles: true,
                        extractAttributes: true
                    })
                })
            });

var zonesLayer = new OpenLayers.Layer.Vector("Zones", {
                projection: wgs_1984,
                strategies: [new OpenLayers.Strategy.Fixed()],
                protocol: new OpenLayers.Protocol.HTTP({
                    //set the url to our own var
                    url: zonesUrl,
                    //format this layer as KML//
                    format: new OpenLayers.Format.KML({
                        //maxDepth is how deep it will follow network links
                        maxDepth: 2,
                        extractStyles: true,
                        extractAttributes: true
                    })
                })
            });

var selectControlHover = new OpenLayers.Control.SelectFeature(
          [kmlLayer, waypointLayer, landmarkLayer], {
            hover: true,
            highlightOnly: true,
            overFeature: function(feature) {
              if (((featureIsWaypoint(feature))||(featureIsLandmark(feature)))&&(hoverFeature != feature)) {
                  if (hoverPopup != null)
                  {
                    if (hoverPopup.contentHTML != null) 
                    {
                      map.removePopup(hoverPopup);
                      hoverPopup.destroy();
                      hoverPopup = null;
                      hoverFeature = null;
                    }
                  }

                  hoverPopup = new OpenLayers.Popup(
                  "chicken",
                  new OpenLayers.LonLat(feature.geometry.x,feature.geometry.y),
                  null,
                  '<b>'+feature.attributes.name+'</b>',
                  null,
                  true
                  );

                  hoverPopup.autoSize = true;
                  hoverPopup.backgroundColor = "rgb(60,60,60)";
                  hoverPopup.opacity = popupOpacity/100;
                  map.addPopup(hoverPopup);

                  hoverFeature = feature;

              }
            },
            outFeature: function(feature) {
              if (hoverPopup != null)
                  {
                    map.removePopup(hoverPopup);
                    hoverPopup.destroy();
                    hoverPopup = null;
                    hoverFeature = null;
                  }
            }
        }
      );

map.addLayer(waypointTrackLayer);
map.addLayer(waypointLayer);
map.addLayer(landmarkLayer);
zonesLayer.visibility = false;
map.addLayer(zonesLayer);


map.addControl(selectControlHover);
        selectControlHover.activate();

//configure feature selection for the KML layer
var selectControl = new OpenLayers.Control.SelectFeature(
                [kmlLayer, waypointLayer, waypointTrackLayer, landmarkLayer], {
                    multiple: false, 
                    hover: false
                }
            );

map.addLayer(kmlLayer);

map.addControl(selectControl);
selectControl.activate();

kmlLayer.events.on({
                "featureselected": function(e) {
                    //alert("test");
                    onFeatureSelect(e.feature);
                }
            });

//make the kml vector layer refresh periodically
window.setInterval(refreshTrack, kmlRefreshInterval, kmlLayer);
window.setInterval(refreshWaypoints, kmlRefreshInterval, waypointLayer);
window.setInterval(refreshLandmarks, kmlRefreshInterval, landmarkLayer);
window.setInterval(refreshWaypointTrack, kmlRefreshInterval, waypointTrackLayer);


//store updates to the mouse position in two global vars
map.events.register("mousemove", map, function (e) {            
var point = map.getLonLatFromPixel( this.events.getMousePosition(e) )
if (point != null) {
    mouse_lon = point.lon;
    mouse_lat = point.lat;
  }
});
                                          

// ----------- FUNCTION DEFINITIONS ------------ //
function refreshAllKml() 
{
  refreshTrack(kmlLayer);
  refreshLandmarks(landmarkLayer);
  refreshWaypoints(waypointLayer);
  refreshWaypointTrack(waypointTrackLayer);
}

waypointLayer.preFeatureInsert = function(feature)
{

//only do this for LineStrings
if (feature.geometry.CLASS_NAME=="OpenLayers.Geometry.LineString")
  {
      //it must be the waypoint track
      waypointTrack = feature;
      feature.style.strokeOpacity = 1;
      feature.style.strokeDashstyle = "dash";

  } else {
    //alert("test");
    //console.log("inserting waypoint");
    waypointFeatures.push(feature);
  }
}

zonesLayer.preFeatureInsert = function(feature)
{
  feature.style.fillOpacity = userOptions.zonesOpacity/100;
  feature.style.strokeOpacity = userOptions.zonesOpacity/100;
  zoneFeatures.push(feature);
}

waypointTrackLayer.preFeatureInsert = function(feature)
{

//only do this for LineStrings
if (feature.geometry.CLASS_NAME=="OpenLayers.Geometry.LineString")
  {
      //it must be the waypoint track
      waypointTrack = feature;
      feature.style.strokeOpacity = 1;
      feature.style.strokeDashstyle = "dash";

  }
}

landmarkLayer.preFeatureInsert = function(feature)
{

//only do this for LineStrings
if (feature.geometry.CLASS_NAME=="OpenLayers.Geometry.LineString")
  {
      //it must be the waypoint track
      waypointTrack = feature;
      feature.style.strokeOpacity = 1;
      feature.style.strokeDashstyle = "dash";

  } else {
    //alert("test");
    //console.log("inserting landmark");
    landmarkFeatures.push(feature);
  }
}


//intercept each feature directly before it is added to the KML vector layer
kmlLayer.preFeatureInsert = function(feature)
{
  //only do this for LineStrings
  if (feature.geometry.CLASS_NAME=="OpenLayers.Geometry.LineString")
  {
    if (feature.attributes.name=="ACTrack") 
    {
      //hold on to the actrack line string
      myTrack = feature;
      //override the default opacity -- KML is having trouble controlling this directly
      feature.style.strokeOpacity = 1;

      //---- VIEW CENTERING ----//
      var zoomTo = initZoom;
      if (refreshNum != 0)
        zoomTo = map.zoom;

      if ((refreshNum == 0)||(centering=="Aircraft"))
        map.setCenter(new OpenLayers.LonLat(feature.geometry.components[feature.geometry.components.length-1].x,feature.geometry.components[feature.geometry.components.length-1].y),zoomTo);
    
      //points can now be referenced by myTrack.geometry.components[point_number]
    }
  } else if (featureIsBarb(feature))
  {
    //---- THIS IS A WIND BARB ----//

      barbCount++;

      if ((feature.attributes.index==-201)&&(userOptions.iconStyle=="Aircraft")) {
        featureBlackList.push(feature);
      }

      //add this feature to the full list of imported barbs
      importedBarbs.push(feature);

      if (barbCount%(barbInterval/barbIntervalMin) != 0)
      {
        //blacklist wind barbs that shouldn't be displayed based on current interval setting (removed in onFeatureInsert)
        featureBlackList.push(feature);
      }
      else
      {
        //---- DISPLAY THE WIND BARB ----//

        //break apart its description, parse according to user specified Wind Barb Parameters, perform unit conversions, and put back together again
        feature.attributes.description = makeNewDescription(feature, feature.attributes.description, barbParams, barbUnitConventions);

        //add the barb to list of clickable features for future reference
        clickableFeatures.push(feature);

        //rotate the barb according to its KML heading field
        feature.style.rotation = feature.attributes.heading;
      }
  } else
  {
    //---- DISPLAY THE AIRPLANE ----//

      //break apart its description, parse according to user specified Aircraft Parameters, perform unit conversions, and put back together again
      feature.attributes.description = makeNewDescription(feature, feature.attributes.description, planeParams, planeUnitConventions);
      
      //rotate the barb according to its KML heading field
      feature.style.rotation = feature.attributes.heading;

      //add it to the list of clickable features
      if (feature.attributes.index!=-101)
      clickableFeatures.push(feature);

      //keep track of this feature specifically for future reference
      theAircraft = feature;
  }
}


//processing to occur directly after a feature is inserted into the KML vector layer
kmlLayer.onFeatureInsert = function(feature)
{
  if ((feature.attributes.index==-101)&&(userOptions.iconStyle=="Aircraft")) {
        kmlLayer.destroyFeatures(feature);
        //console.log("destroying vector");
      } else if ((feature.attributes.index==-99)&&(userOptions.iconStyle!="Aircraft")) {
        kmlLayer.destroyFeatures(feature);
        //console.log("destroying aircraft");
      }

  if (featureIsBarb(feature)) {
    var noPopup = false;

    //remove blacklisted features
    for (var i=0; i<featureBlackList.length; i++)
      {
        if (feature==featureBlackList[i])
        {
          kmlLayer.destroyFeatures(feature);
          noPopup = true;
        }
      }

      //update the current wind barb popup to reflect changes
      if (isSelectedWindBarb(feature)) 
      {

        var which = selectedBarbPopupIndex(feature);
        //alert("popup index is "+which);
        if (thePopups[which] != null)
          {
            if (thePopups[which].contentHTML != null) 
            {
              if (popupIsAircraft(thePopups[which])==false)
              {
                  killPopup(thePopups[which],which);

                  if (noPopup==false)
                  {
                    thePopups[which] = generatePopup(feature, false);
                  }

              }
            }
          }
      } 

  } else if ((feature.geometry.CLASS_NAME != "OpenLayers.Geometry.LineString")&&(feature.attributes.index!=-101)) {
		  	//if the aircraft is selected, make a new bubble for it
        if (thePopups[0] != null)
        {
  		    if (thePopups[0].contentHTML != null)
  		    {
  		      if (popupIsAircraft(thePopups[0]))
  		      {
                //remove the preexisting popup
                killPopup(thePopups[0],0);
                var keepInView;
                //create a new one
                ((centering=="Hybrid")||(centering=="Aircraft"))? keepInView=true : keepInView=false;
    		        thePopups[0] = generatePopup(feature, keepInView);
            }
  		    }
        }
	}
}

//what to do when you select a KML vector layer feature
function onFeatureSelect(feature)
{
  //find the selectable that is closest and select it
  {
    //find the nearest wind barb and click it
    var minDist = 1000, closestFeature= null, closestDist = 10000000000, testDist = 99999;
    var x = mouse_lon;
    var y = mouse_lat;
    //console.log("x = "+x+"\ty = "+y);
    for (var i=0; i<clickableFeatures.length; i++)
    {  
      //console.log("clickableFeatures["+i+"].x = "+clickableFeatures[i].geometry.x, "clickableFeatures["+i+"].y = "+clickableFeatures[i].geometry.y);
      if ((closestFeature==null)&&(clickableFeatures[i]!=null))
      {
        closestFeature= clickableFeatures[i];
        closestDist = Math.sqrt(Math.pow((x - clickableFeatures[i].geometry.x),2)+ Math.pow((y - clickableFeatures[i].geometry.y),2));
        //console.log("first closestDist = "+closestDist);
      }
      else
      {
        testDist = Math.sqrt(Math.pow((x - clickableFeatures[i].geometry.x),2)+ Math.pow((y - clickableFeatures[i].geometry.y),2));
        //console.log("testDist = "+testDist);
        if (testDist < closestDist)
        {
          closestDist = testDist;
          closestFeature = clickableFeatures[i];
          //console.log("closestDist now = "+closestDist);
        }
      }
    }

    if (closestFeature != null)
    {
      var which;

      if ((featureIsBarb(closestFeature))&&(selectedBarbPopupIndex(closestFeature)==-2))
      {

        //kill the oldest wind barb
        killPopup(thePopups[1],1);

        thePopups[1] = thePopups[2];
        thePopups[2] = generatePopup(closestFeature,false);

        selectedBarbIndices[0][0] = selectedBarbIndices[1][0];

        if (selectedBarbIndices[1][1]>0)
        {
          selectedBarbIndices[0][1] = selectedBarbIndices[1][1] - 1;
        } else {
          selectedBarbIndices[0][1] = selectedBarbIndices[1][1];
        }


        selectedBarbIndices[1][0] = importedBarbsIndex(closestFeature);
        selectedBarbIndices[1][1] = 2;


      } else if (featureIsBarb(closestFeature)==false)
      {
        console.log(closestFeature);
        if (closestFeature.attributes.index != -100) {
          //remove preexisting airplane popup to make them less annoying
          killPopup(thePopups[0],0);
          //make the actual popup
          thePopups[0] = generatePopup(closestFeature, false);
        } else console.log("testing");
      }
    }
  }
}

//perform the required operations to refresh all track data display elements
function refreshTrack(layer)
{
  barbCount = 0;
  featureBlackList = [];
  clickableFeatures = [];
  importedBarbs = [];
  refreshNum++;

  //update the requested parameters list
  updateUserSpecifiedVars();

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

function refreshWaypoints(layer)
{
    waypointFeatures = [];
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

function refreshWaypointTrack(layer)
{
    waypointTrack = [];
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

function refreshLandmarks(layer)
{
    landmarkFeatures = [];
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

function refreshZones(layer)
{
    zoneFeatures = [];
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

//update all user-specified variables based on current HTML button and select settings
function updateUserSpecifiedVars()
{
  //update planeParams[] and planeUnitConventions[]
  for (var i=1; i<=numPlaneParams; i++)
  {
      planeParams[i-1] = document.getElementById("planeParam"+String(i)).value;

      if (document.getElementById("planeRadio"+String(i)+"_2").checked)
        planeUnitConventions[i-1] = "standard";
      else
        planeUnitConventions[i-1] = "metric";
  }

  //update barbParams[]
  for (var i=1; i<=numBarbParams; i++)
  {
      barbParams[i-1] = document.getElementById("barbParam"+String(i)).value;
      
      if (document.getElementById("barbRadio"+String(i)+"_2").checked)
        barbUnitConventions[i-1] = "standard";
      else
        barbUnitConventions[i-1] = "metric";
  }

  //update popup opacity
    popupOpacity = userOptions.popupOpacity;
    barbPopupColor = userOptions.barbPopupColor;
    aircraftPopupColor = userOptions.aircraftPopupColor;
    //barbPopupColorRgb = "rgb("+Math.floor(Number(barbPopupColor[0]))+", "+Math.floor(Number(barbPopupColor[1]))+", "+Math.floor(Number(barbPopupColor[2]))+")";

  //update centering
  centering = userOptions.Centering;

  //update lonlat convention
  if (userOptions.coordinateFormat=="deg.min.sec")
    lonlatConvention = "metric";
  else
    lonlatConvention = "standard";

  //update wind barb interval
  var tempBarbInterval = userOptions.windBarbInterval;
  if (tempBarbInterval.indexOf("sec") > 0) {
    barbInterval = Number(tempBarbInterval.substring(0,tempBarbInterval.indexOf(" ")));
  } else if (tempBarbInterval.indexOf("min") > 0) {
    barbInterval = Number(tempBarbInterval.substring(0,tempBarbInterval.indexOf(" ")))*60;
  } else if (tempBarbInterval.indexOf("hr") > 0) {
    barbInterval = Number(tempBarbInterval.substring(0,tempBarbInterval.indexOf(" ")))*3600;
  } 
}

//parse a popup description, grab only desired parameters, convert units, reassemble
function makeNewDescription(feature, oldDescription, paramNames, paramUnits)
{
  var newDescription = "";
  var units = "";
  var value = 0;

  //colored wind barbs
  if (featureIsBarb(feature))
  {
    var category = 0;
    var index = oldDescription.indexOf("\tWS");
    if (index > -1)
        {
              var line_temp = oldDescription.substring(index,index+1000);
              var res = (line_temp.substring(0,line_temp.indexOf("<br>"))).split(" ");
              value = res[2];
        }
    //colors: black, blue, green, yellow, red, sub-magenta
    //scale: Saffir-Simpson Hurricane Wind Scale
    value *= MSEC2KNOTS;

    //value *= value;
    var fileExtension = ".png";

    if (userOptions.coloredBarbs==true) {
      if ((value >= 64)&&(value < 83)) {
        category = 1;
        fileExtension = "_blue.png";
      } else if ((value >= 83)&&(value < 96)) {
        category = 2;
        fileExtension = "_green.png";
      } else if ((value >= 96)&&(value < 113)) {
        category = 3;
        fileExtension = "_yellow.png";
      } else if ((value >= 113)&&(value < 137)) {
        category = 4;
        fileExtension = "_red.png";
      } else if (value >= 137) {
        category = 5;
        fileExtension = "_magenta.png";
      }
    }

    feature.style.externalGraphic = feature.style.externalGraphic.substring(0,feature.style.externalGraphic.indexOf("png")-1)+fileExtension;
    //console.log(feature.style.externalGraphic+"\t\t"+value);



    //blacklist the feature if ground speed is less than 40 knots
    index = oldDescription.indexOf("\tGROUND_SPD");
    if (index > -1)
        {
              line_temp = oldDescription.substring(index,index+1000);
              res = (line_temp.substring(0,line_temp.indexOf("<br>"))).split(" ");
              value = res[2];

              if (value*MSEC2KNOTS < 40)
                featureBlackList.push(feature);
        }
  }

  for (var i=-3; i<paramNames.length; i++)
  {
    var matchName;
    var requiredNames = ["Time", lookupNickName("latref"), lookupNickName("lonref")];

    if (i>-1)
    {
      matchName = paramNames[i];
    } else {
      matchName = requiredNames[i+3];
    }
    //alert("matchName is: "+matchName);

    //grab the line corresponding to the parameter we're looking for
    if (matchName!="WS")
      var index = oldDescription.indexOf(matchName);
    else
      var index = oldDescription.indexOf("\t"+matchName);

    //we only continue parsing if there is actually the parameter that we want in there
    if (index > -1)
    {
          var line_temp = oldDescription.substring(index,index+1000);
          var res = (line_temp.substring(0,line_temp.indexOf("<br>"))).split(" ");
          value = res[2];

          if (matchName=="Time")
          {
            value = line_temp.substring(6,16);
          }

              //if (matchName=="WS") alert(value);

          //this will be an array
          if (i>=0) {
            var unitsToPass = paramUnits[i];
          } else {
            var unitsToPass = "metric";
            if ((matchName==lookupNickName("latref"))||(matchName==lookupNickName("lonref")))
              unitsToPass = lonlatConvention;
          }
          var convertedUnits = convertUnits(matchName, value, unitsToPass);

          //append the right string components to newDescription
          if ((matchName != "Time")&&((convertedUnits[0].indexOf("&deg") < 0)||(convertedUnits[1]=="C")||(convertedUnits[1])=="F"))
            newDescription += String(matchName)+": "+convertedUnits[0]+" ("+convertedUnits[1]+")<br>\n";
          else {
            newDescription += String(matchName)+": "+convertedUnits[0]+"<br>\n";
          }

          if (i==-1) {
            newDescription += "<br>";
          }
    }
  }

  return newDescription;
}

//set default HTML user parameter settings
function initUserParamSettings()
{
  for (var i=0; i<numBarbParams; i++)
  {
    document.getElementById("barbRadio"+String(i+1)+"_1").checked = true;
    document.getElementById("barbParam"+String(i+1)).value = barbParamDefaults[i];
  }

  for (var i=0; i<numPlaneParams; i++)
  {
    document.getElementById("planeRadio"+String(i+1)+"_1").checked = true;
    document.getElementById("planeParam"+String(i+1)).value = planeParamDefaults[i];
  }

  lonlatConvention = "metric";

}

//convert a value and unit string to a given convention
function convertUnits(paramName, oldValue, convention)
{
  var oldUnits = lookupUnits(paramName);

  newValue = oldValue;
  newUnits = oldUnits;

  var appendStr = "";

  //convert unit text and values
  switch (oldUnits) {

      case "m":
        if (convention=="standard")
        {
          newUnits = "ft";
          newValue = oldValue*M2FEET;
        }
      break;

      case "ft":
        if (convention=="metric")
        {
          newUnits = "m";
          newValue = oldValue/M2FEET;
        }

      case "m/s":
        if (convention=="standard")
        {
          newUnits = "kts";
          newValue = oldValue*MSEC2KNOTS;
        }
      break;

      case "kts":
        if (convention=="metric")
        {
          newUnits = "m/s";
          newValue = oldValue/MSEC2KNOTS;
        }
      break;

      case "C":
        if (convention=="standard")
        {
          newUnits = "F";
          newValue = oldValue*1.8+32;
        }
      break;

      case "F":
        if (convention=="metric")
        {
          newUnits = "C";
          newValue = (oldValue-32)/1.8;
        }
      break;

      case "degs":
        if (convention=="metric")
        {
          newUnits = "dms";
          newValue = "";
          //alert(lookupAampsName(paramName));
          if (lookupAampsName(paramName)=="latref")
          {
            if (oldValue < 0) {
              appendStr = " S";
              oldValue = Math.abs(oldValue);
            }
            else
              appendStr = " N";
          } else if (lookupAampsName(paramName)=="lonref")
          {
            if (oldValue < 0) {
              appendStr = " W";
              oldValue = Math.abs(oldValue);
            }
            else
              appendStr = " E";
          }

          newValue += String(Math.floor(oldValue))+"&deg";
          newValue += String(pad(((oldValue%1)*60).toFixed(0)))+"'";
          newValue += String(pad((((oldValue%1)*60)%1*60).toFixed(0)))+"\"";
          newValue += appendStr;
        }
      break;
  }

  //address rounding
  if (paramName==lookupNickName("altref"))
  {
    newValue = Math.round(parseFloat(newValue));
  } else if ((paramName != "Time")&&(paramName != lookupNickName("latref"))&&(paramName != lookupNickName("lonref"))) {
    newValue = parseFloat(newValue).toFixed(1);
  }

  //add the degrees symbol in some occasions
  if ((newUnits=="C")||(newUnits=="F")||(newUnits=="deg")||(newUnits=="degs"))
    newValue = String(newValue)+"&deg";

  return [String(newValue), newUnits];
}


//what default units are associated with a certain parameter nickname?
function lookupUnits(paramName)
{
  for (i=0; i<masterUnitsList.length; i++)
  {
    if (masterUnitsList[i][1]==paramName)
    {
      return masterUnitsList[i][2];
    }
  }

  return "";
}


//what is the nickname (user display name) for a certain AAMPS parameter?
function lookupNickName(aampsName)
{
  for (i=0; i<masterUnitsList.length; i++)
  {
    if (masterUnitsList[i][0]==aampsName)
    {
      return masterUnitsList[i][1];
    }
  }

  return "unknown";
}

function lookupAampsName(paramName)
{
  for (i=0; i<masterUnitsList.length; i++)
  {
    if (masterUnitsList[i][1]==paramName)
    {
      return masterUnitsList[i][0];
    }
  }

  return "unknown";
}

//pad a number with n leading zeros
function pad(n)
{
  if (n>0)
    return (n < 10) ? ("0" + n) : n;
  else if (n>-10) {
    var a = Math.abs(n)
    var temp = (a < 10) ? ("0" + a) : a;

    if (n != 0)
      return "-0"+a;
    else
      return a+"0";
  } else return n;
}

//in the array of all wind barbs imported from KML, what is the index of a certain wind barb?
function importedBarbsIndex (feature)
{
  for (var i=0; i<importedBarbs.length; i++)
  {
    if (importedBarbs[i]==feature)
      return i;
  }

  return -1;
}

//generate a popup for this feature
function generatePopup(feature, keepInView)
{
  var popup = new OpenLayers.Popup(
  "chicken",
  new OpenLayers.LonLat(feature.geometry.x,feature.geometry.y),
  null,
  '<b>'+feature.attributes.name+'</b><p>'+feature.attributes.description+'</p>',
  null,
  true
  );
  popup.autoSize = true;
  popup.panMapIfOutOfView = keepInView;
  popup.addCloseBox(function() {unReferencePopup(popup); map.removePopup(popup); popup.destroy(); delete popup;});
  if (featureIsBarb(feature))
    popup.backgroundColor = barbPopupColor;
  else
    popup.backgroundColor = aircraftPopupColor;
  popup.opacity = popupOpacity/100;
  map.addPopup(popup);
  return popup;
}

function killPopup(popup, index)
{
  if (popup != null)
  {
    if (popup.contentHTML != null) {
      //alert("popup wasn't null");

      for (var i=0; i<3; i++)
        {
          if (thePopups[i]==popup)
            thePopups[i]=null;
        }

      map.removePopup(popup);
      popup.destroy();
      delete popup;
      thePopups[index] = null;
    }
  }
}

function unReferencePopup(popup)
{
  for (var i=0; i<3; i++)
  {
    if (thePopups[i]==popup)
      thePopups[i]=null;

    for (var j=0; j<2; j++)
    {
      if (selectedBarbIndices[j][1] == i) {
        selectedBarbIndices[j][1] = -1;
        selectedBarbIndices[j][0] = -1;
      }
    }
  }
}

function featureIsBarb(feature)
{
  if ((feature.attributes.styleUrl.indexOf("knots") > 0)&&(feature.attributes.index != -101))
    return true;
  else
    return false;
}

function popupIsAircraft(popup)
{
  if (popup.contentHTML.indexOf("Aircraft") > 0)
    return true;
  else
    return false;
}

function isSelectedWindBarb(feature)
{
  for (var i=0; i<2; i++) {
    if (importedBarbsIndex(feature)==selectedBarbIndices[i][0])
      return true
  }
  return false
}

function selectedBarbPopupIndex(feature)
{
  for (var i=0; i<2; i++) {
    if (importedBarbsIndex(feature)==selectedBarbIndices[i][0])
    {
      return selectedBarbIndices[i][1];
    }
  }
  return -2;
}

function featureIsWaypoint(feature)
{
  for (var i=0; i<waypointFeatures.length; i++)
  {
    if (waypointFeatures[i]==feature)
      return true;
  }
  return false;
}

function featureIsLandmark(feature)
{
  for (var i=0; i<landmarkFeatures.length; i++)
  {
    if (landmarkFeatures[i]==feature)
      return true;
  }
  return false;
}
