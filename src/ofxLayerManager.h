//
//  ofxLayerManager.hpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "ofMain.h"
#include "ofxNotificationCenter.h"
#include "Layer.h"
#include "ofxJson.h"

class ofxLayerManager
{
public:
    
    ofxLayerManager();
    ~ofxLayerManager();
    
    void setup();
    void update(float dt);
    void draw();
    
    /*
     LAYERS
     */
    int getNumLayers();
    Layer* getLayer(int index); 
    
    //NEED TO DO: temporary until MediaLayers is above this class
    map<string, string> animationsByZone; //pair is <zoneUID, animationUID>
    vector<string> zoneNames;
    
private:
    //NEED TO DO: Move this outside of the json file so that is more global
    
    //! Layer Settings
    ofxJSONElement jsonRef;
    string jsonFilePath = "MediaLayers.json";
    
    //! Layers
    vector<Layer*> layers;
    
    
};
