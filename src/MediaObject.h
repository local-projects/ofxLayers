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
#include "Global.h"

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
    void setZone(int _zoneOrder, string _zoneUID);
    
    
    /*
     Playing sequential objects
     */
    
    void setNextObject(string _nextUID);
    void setPrevObject(string _prevUID);
    string getNotifyUID();
    void setNotifyUID(string _notifyUID);
    bool isFirstSequentialObject();
    bool isSecondSequentialObject();
    
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
    void setB_Pos(ofVec2f _B_Pos);
    void setC_Pos(ofVec2f _C_Pos);
    
    //Texutre
    void setTexture(ofTexture &_tex);
    
    //----- ANIMATIONS ---------
    
    //Animation state
    enum AnimationState
    {
        STOPPED,
        PLAYING,
        BRIEF_PAUSE,
        NUM_ANIMATION_STATES
    };
    AnimationState getAnimationState();
    
    //Playing & Stopping
    void triggerPlay();
    void triggerStop();
    
    //Visibility
    void makeAnimationVisble();
    void makeAnimationInvisible();
    
    //LINEAR ANIMATIONS
    void setDuration1(float _duration1);
    void setDuration2(float _duration2);
    
    // BOBBING
    void setBobDrift(ofVec2f _bobDrift);
    
    // Animation Callbacks
    void onAnim1Finish(ofxAnimatable::AnimationEvent & event);
    void onAnim2Finish(ofxAnimatable::AnimationEvent & event);
    void onPauseFinish(ofxAnimatable::AnimationEvent & event);
    
    // Pause
    void setPauseDuration(float );
    
    //debug
    void setDebug(bool _debug); 
private:
    
    //! Name of UID
    string UID = "";
    
    //! Position of media object
    ofVec2f pos = ofVec2f(0.0f, 0.0f); // this is up to date pos
    ofVec2f A_Pos = ofVec2f(0.0f, 0.0f); // this is the start pos
    ofVec2f B_Pos = ofVec2f(0.0f, 0.0f); // this is only for traversing animations
    ofVec2f C_Pos = ofVec2f(0.0f, 0.0f); //
    
    //! Layer of object
    int layer = 0;
    
    
    //! If this is empty, there is no object to playnext. If this is not empty, there is an object to play next.
    string nextUID = "";
    string prevUID = "";
    bool sequentialObj = false;
    string notifyUID = "playNextObject";
    void sendPlayNextObject();
    
    //! Zone
    int zoneOrder = 0;
    string zoneUID = "";
    
    //Texure
    ofTexture tex;
    
    //Animations
    
    //Animation state
    AnimationState animState = AnimationState::STOPPED;
    void setAnimationState(AnimationState _animState);
    
    // Pause
    ofxAnimatableFloat pause;
    float pauseDuration = 1.0f;
    
    bool animationVisble = true;
    
    //! Use animFloat1 for point A to B
    ofxAnimatableFloat animFloat1;
    float duration1 = 2.0f;
    
    //! Use animFloat2 for point C to A
    ofxAnimatableFloat animFloat2;
    float duration2 = 2.0f;
    
    //bobbing
    //! bobDrift is the amount of drift from the startPos in both the x + y
    ofVec2f bobDrift = ofVec2f(5.0, 10.0f);
    float bobRotation = 2.5f;
    
    //debugging
    bool debug = false;
    void onKeyPressed(ofKeyEventArgs & args);
};

/*
 switch(animType)
 {
 case LayerData::IMAGE_SEQUENCE:{ break;}
 case LayerData::TRAVERSING_2_POINT:{break;}
 case LayerData::ROTATING:{break;}
 case LayerData::WIGGLING:{break;}
 case LayerData::BOBBING:{ break; }
 case LayerData::TRAVERSING_3_POINT:{break;}
 case LayerData::STATIC:{break;}
 default:break;
 }
*/
