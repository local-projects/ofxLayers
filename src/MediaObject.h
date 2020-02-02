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
#include "LayerIDManager.h"
#include "ofxImageSequenceVideo.h"

class MediaObject
{
public:
    MediaObject();
    ~MediaObject();
    
    
    void setup(string _UID, ofVec2f _pos, int _zoneOrder, string _zoneUID, int _layer, ofxImageSequenceVideo * imgSequence );
    void update(float dt);
    void draw(ofVec2f offset = ofVec2f(0.0f, 0.0f));
    void drawDebug();

	void reset();

    /*
     Set up attributes
     */
    void setUID(string _UID);
    void setPosition(ofVec2f _pos);
    void setLayer(int _layer);
    void setZone(int _zoneOrder, string _zoneUID);

	ofxImageSequenceVideo * getImgSequence(){return imgSequence;}
    
    /*
     Playing sequential objects
     */
    
    void setNextObject(string _nextUID);
    void setPrevObject(string _prevUID);
    bool isFirstSequentialObject();
    bool isSecondSequentialObject();

	bool isSequentialObject(); //if true, this animaton consists of two image sequences; when one plays to the end, the next one triggers, and so on.
	string getNextOrPreviousObject();
    
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
		SHOULD_STOP,
        NUM_ANIMATION_STATES
    };
	AnimationState getAnimationState();
	string toString(AnimationState s);

	string toString(LayerData::AnimationType s);
    
    //Playing & Stopping
    void triggerPlay(bool shouldStopAfterDone = false);
    void triggerStop(bool stopRightNow); //if stopRightNow==true, stops mid-animation. if false, anim keeps playing til end then stops
	void genericPlay(); 

    //Visibility
    void makeAnimationVisble();
    void makeAnimationInvisible();
    
    //LINEAR ANIMATIONS
    void setDuration1(float _duration1);
    void setDuration2(float _duration2);
	void setPauseDuration(float _durLow, float _durHigh);
    
    // BOBBING
    void setBobDrift(ofVec2f _bobDrift);
    void setBobRotation(float _bobRotation);
    void setRandomnVariance(ofVec2f _randomnBobVal);
    
    // Animation Callbacks
    void onAnim1Finish(ofxAnimatable::AnimationEvent & event);
    void onAnim2Finish(ofxAnimatable::AnimationEvent & event);
    void onPauseFinish(ofxAnimatable::AnimationEvent & event);

    //debug
    void setDebug(bool _debug);
    void setMeshDebug(bool _meshDebug);
    
    //mesh
    void setAmplitude(int index, float* amplitude);

	//Animation state
	AnimationState animState = AnimationState::STOPPED;
	void setAnimationState(AnimationState _animState);

	bool shouldPauseWhenReachingEnd = false;

private:

	ofxImageSequenceVideo * imgSequence = nullptr; //if the mediaObj is a plain static PNG, this will be null

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
    void sendPlayNextObject();
    
    //! Zone
    int zoneOrder = 0;
    string zoneUID = "";
    
    //Texure
    ofTexture tex;
    
    //Animations
    

	float animationProgress = 0.0f;
    
    // Pause
    ofxAnimatableFloat pause;
    float pauseDurationLow = 0.0f;
	float pauseDurationHigh = 0.0f;
    
    bool animationVisble = true;
    
    //! Use animFloat1 for point A to B
    ofxAnimatableFloat animFloat1;
    float duration1 = 2.0f;
    
    //! Use animFloat2 for point C to A
    ofxAnimatableFloat animFloat2;
    float duration2 = 2.0f;

	bool firstPlay = true;

    //bobbing
    //! bobDrift is the amount of drift from the startPos in both the x + y
    ofVec2f bobDrift = ofVec2f(5.0, 10.0f);
    float bobRotation = 2.5f;
    ofVec2f randomnBobVal = ofVec2f(ofRandom(0.2f, 1.0f), ofRandom(0.2f, 1.0f));
    
    //debugging
    bool debug = false;
    bool meshDebug = false;
    void onKeyPressed(ofKeyEventArgs & args);

    //Mesh Animations
    vector<float*> amplitudes;
    int numAmplitudes = 8;
    float circleSize = 10.0f;
    float debugPos = 100.f;
};

/*
 switch(animType)
 {
 case LayerData::IMAGE_SEQUENCE:{ break;}
 case LayerData::TRAVERSING_2_POINT:{break;}
 case LayerData::ROTATING:{break;}
 case LayerData::STEM:{break;}
 case LayerData::BOBBING:{ break; }
 case LayerData::TRAVERSING_3_POINT:{break;}
 case LayerData::STATIC:{break;}
 case LayerData::TWO_PT_SWAY:{break;}
 default:break;
 }
*/
