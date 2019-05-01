//
//  MediaObject.hpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "ofMain.h"
#include "ofxNotificationCenter.h"
#include "LayerData.h"
#include "ofxAnimatableFloat.h"

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
    
    /*
     Animation Type
     */
    
    LayerData::AnimationType animType = LayerData::AnimationType::IMAGE_SEQUENCE;
    void setAnimationType( LayerData::AnimationType _animType);
    LayerData::AnimationType getAnimationType();
    
    //this should only be used for traversing object
    void setEndPos(ofVec2f _endPos);
    
    //Texutre
    void setTexture(ofTexture &_tex);
    
    //Animations
    void setPlaying(bool _playing);
    void onAnimFinish(ofxAnimatable::AnimationEvent & event);
    
    //bobbing
    void setBobDrift(ofVec2f _bobDrift);
    
private:
    
    //! Name of UID
    string UID = "";
    
    //! Position of media object
    ofVec2f pos = ofVec2f(0.0f, 0.0f); // this is up to date pos
    ofVec2f startPos = ofVec2f(0.0f, 0.0f); // this is the start pos
    ofVec2f endPos = ofVec2f(0.0f, 0.0f); // this is only for traversing animations
    
    //! Layer of object
    int layer = 0;
    
    
    //! If this is empty, there is no object to playnext. If this is not empty, there is an object to play next.
    string nextUID = "";
    
    //! Zone
    int zoneOrder = 0;
    string zoneUID = "";
    
    //Texure
    ofTexture tex;
    
    //Animations
    ofxAnimatableFloat animFloat;
    float animTiming = 2.0f;
    bool playing = false; //used for ALL objects acept
    
    //bobbing
    //! bobDrift is the amount of drift from the startPos in both the x + y
    ofVec2f bobDrift = ofVec2f(5.0, 10.0f);
    float bobRotation = 10.0f; 
};
