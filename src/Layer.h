//
//  Layer.hpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "ofMain.h"
#include "ofxNotificationCenter.h"
#include "MediaObject.h"
#include "AppStructs.h"

class Layer
{
public:
    
    Layer();
    ~Layer();
    
    void setup();
    void update(float dt);
    void draw();
    
    //! Visibility
    void setIsVisble(bool visible);
    
    //! Media Objects
    // Each layer should only have one background image, so this function should only be called once
    void addBgImage(string imgPath);
    
    void addMediaObject(string path,
                        ofVec2f pos,
                        ofVec2f size,
                        int layer,
                        float fps,
                        string name,
                        App::LoadType loadType,
                        App::AnimationType animType,
                        string next,
                        int zone);
    
private:
    
    //! Visibility
    bool isVisble = true;
    
    //! Media Objects
    ofImage * bgImage;
    bool hasBgImage = false; 
    
    //Replace this image with the media system
    vector<MediaObject*> mediaObjects;
};
