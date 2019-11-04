//
//  Layer.hpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "ofMain.h"
#include "MediaObject.h"

class Layer
{
public:
    
    Layer();
    ~Layer();
    
    void setup();
    void update(float dt);
    void draw(ofVec2f offset);
	void drawDebug(ofVec2f offset);
    
    //! Visibility
    void setIsVisible(bool visible);
    
    //! Media Objects
    // Each layer should only have one background image, so this function should only be called once
    void addBgImage(string imgPath);
    
    //string _UID, ofVec2f _pos, int _zone, int _layer
    void addMediaObject(string UID,
                        ofVec2f pos,
                        int zoneOrder,
                        string zoneUID,
                        int layer,
                        LayerData::AnimationType animType);
    
    MediaObject *getMediaObject( string UID);
    string getMediaObjectUID(int index); 
    int getNumMediaObjects();
	bool getHasBgImage(){return hasBgImage;}

private:
    
    //! Visibility
    bool isVisible = true;
    
    //! Media Objects
    ofImage * bgImage;
    bool hasBgImage = false; 
    
    //Replace this image with the media system
    map<string, MediaObject*> mediaObjects;
    vector<string> mediaObjectIds;
};
