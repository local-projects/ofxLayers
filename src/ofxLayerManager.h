//
//  ofxLayaerManager.hpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "ofMain.h"
#include "ofxNotificationCenter.h"
#include "Layer.h"
#include "ofxJson.h"

class ofxLayaerManager
{
public:
    
    ofxLayaerManager();
    ~ofxLayaerManager();
    
    void setup();
    void update(float dt);
    void draw();
    
private:
    //! Layer Settings
    ofxJSONElement jsonRef;
    string jsonFilePath = "MediaLayers.json";
    
    //! Layers
    vector<Layer*> layers;
    
    
};
