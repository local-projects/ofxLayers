//
//  MediaObject.hpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "ofMain.h"
#include "ofxNotificationCenter.h"

class MediaObject
{
public:
    MediaObject();
    ~MediaObject();
    
    void setup(string _UID, ofVec2f _pos, int _zoneOrder, string _zoneUID, int _layer);
    void update(float dt);
    void draw();
    void drawDebug(); 
    
    /*
     Set up attributes
     */
    void setUID(string _UID);
    void setPosition(ofVec2f _pos);
    void setLayer(int _layer);
    void setNextOjbect(string _nextUID);
    void setZone(int _zoneOrder, string _zoneUID);
    
    /*
     Get Attributes
     */
    ofVec2f getPos();
    
private:
    
    //! Name of UID
    string UID = "";
    
    //! Position of media object
    ofVec2f pos = ofVec2f(0.0f, 0.0f);
    
    //! Layer of object
    int layer = 0;
    
    
    //! If this is empty, there is no object to playnext. If this is not empty, there is an object to play next.
    string nextUID = "";
    
    //! Zone
    int zoneOrder = 0;
    string zoneUID = "";
    
    //Texure
    ofTexture tex;
    
    
};
