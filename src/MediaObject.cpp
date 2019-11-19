//
//  MediaObject.cpp
//  LayerSystem
//
//  Created by Nicole Messier on 3/25/19.
//
//

#include "MediaObject.h"

MediaObject::MediaObject()
{
    
}

MediaObject::~MediaObject()
{
    
}


void MediaObject::setup(string _UID, ofVec2f _pos, int _zoneOrder, string _zoneUID, int _layer, ofxImageSequenceVideo * imgSequence)
{
	this->imgSequence = imgSequence;
    UID = _UID; 
    A_Pos = _pos;
    pos = _pos;
    layer = _layer;
    zoneOrder = _zoneOrder;
    zoneUID = _zoneUID; 
    layer = _layer;
    
    animFloat1.reset(0);
    animFloat1.setRepeatType(PLAY_ONCE);
    animFloat1.setDuration(ofRandom(1.5f, duration1));
    animFloat1.setCurve(LINEAR);
    ofAddListener(animFloat1.animFinished, this, &MediaObject::onAnim1Finish);
    
    animFloat2.reset(0);
    animFloat2.setRepeatType(PLAY_ONCE);
    animFloat2.setDuration(duration2);
    animFloat2.setCurve(LINEAR);
    ofAddListener(animFloat2.animFinished, this, &MediaObject::onAnim2Finish);
    
    pause.reset(0);
    pause.setRepeatType(PLAY_ONCE);
    pause.setDuration(0.01);
    pause.setCurve(LINEAR);
    ofAddListener(pause.animFinished, this, &MediaObject::onPauseFinish);
    
    ofAddListener(animFloat2.animFinished, this, &MediaObject::onAnim2Finish);
    
    
    ofAddListener(ofEvents().keyPressed, this, &MediaObject::onKeyPressed);
    
    for(int i = 0; i < numAmplitudes; i++)
    {
        float* temp = new float;
        amplitudes.push_back(temp);
    }
}

void MediaObject::reset(){

	triggerStop();
	animFloat1.reset(0);
	animFloat2.reset(0);
	pause.reset(0);
	setAnimationState(STOPPED);
	pos = A_Pos;
	firstPlay = true;
}

void MediaObject::update(float dt)
{
    float time = ofGetElapsedTimef();
    animFloat1.update(dt);
    animFloat2.update(dt);
    pause.update(dt);

	if(imgSequence){
		animationProgress = imgSequence->getPosition();
	}

    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:
			break;

        case LayerData::TRAVERSING_2_POINT:
            pos = A_Pos + (B_Pos - A_Pos)*animFloat1.val();
            break;

        case LayerData::ROTATING:
			break;

        case LayerData::STEM:
			break;

        case LayerData::BOBBING:
        {
            pos.x = A_Pos.x + (bobDrift.x + randomnBobVal.x*ofNoise(time) )*animFloat1.val() + tex.getWidth()/2;
            pos.y = A_Pos.y + (bobDrift.y + randomnBobVal.y*ofNoise(time) )*animFloat1.val() + tex.getHeight()/2;
            break;
        }
        case LayerData::TRAVERSING_3_POINT: {
            
            
            //Three point traversing
            //Duraiton one is point A to B
            //Duraiton two is point C to A
            
            if(animFloat1.isAnimating())
            {
                pos = A_Pos + (B_Pos - A_Pos)*animFloat1.val();
            }
            else if(animFloat2.isAnimating())
            {
                pos = C_Pos + (A_Pos - C_Pos)*animFloat2.val();
            }
            
            break;
        }
        case LayerData::STATIC: { break;}
        case LayerData::TWO_PT_SWAY: { break;}
        default:break;
    }

    
    
}

void MediaObject::draw(ofVec2f offset)
{
    
    
    if(!animationVisble)
    {
        return;
    }
 
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:
            tex.draw(int(pos.x + offset.x), int(pos.y + offset.y), tex.getWidth(), tex.getHeight());
            break;

        case LayerData::TRAVERSING_2_POINT:
            tex.draw(pos.x + offset.x, pos.y + offset.y, tex.getWidth(), tex.getHeight());
           
            if(debug){
                ofVec2f debugPos = ofVec2f(pos.x, pos.y + tex.getHeight());
                ofDrawBitmapStringHighlight(UID + "\n" +
                                   "duration1:" + ofToString(duration1) + "\n",
                                   debugPos.x,
                                   debugPos.y, ofColor(0,200), ofColor::white);
        	}break;

        case LayerData::ROTATING:
            ofPushMatrix();
            ofTranslate(pos.x + tex.getWidth()/2 + offset.x, pos.y + tex.getHeight()/2 + offset.y);
            ofRotateDeg(360.0f*animFloat1.val());
            tex.draw(-tex.getWidth()/2, -tex.getHeight()/2);
            ofPopMatrix();
            break;

        case LayerData::STEM: {
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            
			const float val = animFloat1.val();
            //Create Values
            float value0 = (*amplitudes[0]) * val;
            float value1 = (*amplitudes[1]) * val;
            float value2 = (*amplitudes[2]) * val;
            float value3 = (*amplitudes[3]) * val;
            float value4 = (*amplitudes[4]) * val;
            float value5 = (*amplitudes[5]) * val;
            float value6 = (*amplitudes[6]) * val;
            float value7 = (*amplitudes[7]) * val;
            
            //Add positions
            ofVec3f ml = ofVec3f(A_Pos.x + value0, A_Pos.y + tex.getHeight()/2 + value1);
            ofVec3f tl = ofVec3f(A_Pos.x + value2, A_Pos.y  + value3);
            ofVec3f tr = ofVec3f(A_Pos.x + tex.getWidth() + value4, A_Pos.y + value5 );
            ofVec3f mr = ofVec3f(ofVec3f(A_Pos.x + tex.getWidth() + value6, A_Pos.y + tex.getHeight()/2 + value7));
            ofVec3f br = ofVec3f(ofVec3f(A_Pos.x + tex.getWidth(), A_Pos.y + tex.getHeight()));
            ofVec3f bl = ofVec3f(ofVec3f(A_Pos.x, A_Pos.y + tex.getHeight()));
        
            
            mesh.addVertex(ml); //half left
            mesh.addVertex(tl); //top left
            mesh.addVertex(tr); // top right
            mesh.addVertex(mr); // half right
            mesh.addVertex(br); // bottom right
            mesh.addVertex(bl);
            
            mesh.addTexCoord(ofVec2f(0.0f, 0.5f)); //ml
            mesh.addTexCoord(ofVec2f(0.0f, 0.0f)); //tl
            mesh.addTexCoord(ofVec2f(1.0f, 0.0f)); //tr
            mesh.addTexCoord(ofVec2f(1.0f, 0.5f)); //mr
            mesh.addTexCoord(ofVec2f(1.0f, 1.0f)); //br
            mesh.addTexCoord(ofVec2f(0.0f, 1.0f)); //bl
            
            tex.bind();
                mesh.draw();
            tex.unbind();
            
            if(meshDebug)
            {
                
                ofSetColor(255, 0, 0);
                ofNoFill();
                
                ofDrawCircle(tl.x, tl.y, circleSize);
                ofDrawCircle(tr.x, tr.y, circleSize);
                ofDrawCircle(mr.x, mr.y, circleSize);
                ofDrawCircle(ml.x, ml.y, circleSize);
                
                ofDrawRectangle(tl.x, tl.y, tex.getWidth(), tex.getHeight());
                
                ofSetColor(ofColor::pink);
                ofDrawBitmapString("pos: amp[0], amp[1]", ml.x - debugPos, ml.y);
                ofDrawBitmapString("pos: amp[2], amp[3]", tl.x - debugPos, tl.y);
                ofDrawBitmapString("pos: amp[4], amp[5]", tr.x, tr.y);
                ofDrawBitmapString("pos: amp[6], amp[7]", mr.x, mr.y);
                ofFill();
                ofSetColor(255);
            }
        }break;

        case LayerData::BOBBING:{
            ofPushMatrix();
            ofTranslate(pos.x + offset.x, pos.y + offset.y);
            
            float rotationVal = bobRotation*animFloat1.val();
            float rotationValMapped = ofMap(rotationVal, 0, bobRotation, -bobRotation/2, bobRotation/2);
            ofRotateDeg(rotationValMapped);

            tex.draw(-tex.getWidth()/2, -tex.getHeight()/2);
            ofPopMatrix();
            break;
        }

        case LayerData::TRAVERSING_3_POINT: {
            //Three point traversing
            //Duraiton one is point A to B
            //Duraiton two is point C to A
            
            tex.draw(pos.x + offset.x, pos.y + offset.y, tex.getWidth(), tex.getHeight());
            
//            if(debug)
//            {
//                ofVec2f debugPos = ofVec2f(pos.x, pos.y + tex.getHeight());
//                ofDrawBitmapStringHighlight(UID + "\n" +
//                                   "duration1:" + ofToString(duration1,1) + "\n"
//                                   "duration2:" + ofToString(duration2,1) + "\n"
//								    "pauseDuration:" + ofToString(pauseDurationLow,1) + "-" + ofToString(pauseDurationHigh,1) + "\n",
//                                   debugPos.x,
//                                   debugPos.y,
//								    ofColor(0,200), ofColor::white);
//            }
        }break;

        case LayerData::STATIC: {
            tex.draw(int(pos.x + offset.x), int(pos.y + offset.y), tex.getWidth(), tex.getHeight());
            break;
        }

        case LayerData::TWO_PT_SWAY: {
            
            ofMesh mesh;
            mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
            
            const float val = animFloat1.val();

            //Create Values
			float value0 = (*amplitudes[0]) * val;
            float value1 = (*amplitudes[1]) * val;
            float value2 = (*amplitudes[2]) * val;
            float value3 = (*amplitudes[3]) * val;
            
            //Add positions
            ofVec3f tl = ofVec3f(A_Pos.x + value0 , A_Pos.y + value1 );
            ofVec3f tr = ofVec3f(A_Pos.x + tex.getWidth() + value2, A_Pos.y + value3 );
            ofVec3f br = ofVec3f(ofVec3f(A_Pos.x + tex.getWidth(), A_Pos.y + tex.getHeight()));
            ofVec3f bl = ofVec3f(ofVec3f(A_Pos.x, A_Pos.y + tex.getHeight()));
            
            //Add verticies
            mesh.addVertex(tl); //top left
            mesh.addVertex(tr); // top right
            mesh.addVertex(br); // bottom right
            mesh.addVertex(bl); //bottom left
            
            //Add texture vertices
            mesh.addTexCoord(ofVec2f(0.0f, 0.0f));
            mesh.addTexCoord(ofVec2f(1, 0.0f));
            mesh.addTexCoord(ofVec2f(1, 1));
            mesh.addTexCoord(ofVec2f(0.0f, 1));

            tex.bind();
                mesh.draw();
            tex.unbind();

            if(meshDebug){

                ofSetColor(255, 0, 0);
                ofNoFill();
                
                ofDrawCircle(tl.x, tl.y, circleSize);
                ofDrawCircle(tr.x, tr.y, circleSize);
                ofDrawCircle(br.x, br.y, circleSize);
                ofDrawCircle(bl.x, bl.y, circleSize);
                
                ofDrawRectangle(tl.x, tl.y, tex.getWidth(), tex.getHeight());
                
                ofSetColor(ofColor::pink);
                ofDrawBitmapString("pos: amp[0], amp[1]", tl.x - debugPos, tl.y);
                ofDrawBitmapString("pos: amp[2], amp[3]", tr.x, tr.y);
                ofFill();
                ofSetColor(255);
            }
        }break;

        default:
			break;
    }
    
//    if(debug){
//        drawDebug();
//    }
}

void MediaObject::drawDebug()
{

	ofColor textColor;
	if(imgSequence){
		if(animState == PLAYING){
			textColor = ofColor::red;
		}else{
			textColor = ofColor::magenta;
		}

	}else{
		textColor = ofColor::limeGreen;
	}

	ofSetColor(textColor);
	const float offY = -8;

	if(animType == LayerData::TRAVERSING_2_POINT){
		ofDrawBitmapStringHighlight("A", A_Pos.x, A_Pos.y + offY);
		ofDrawBitmapStringHighlight("B", B_Pos.x, B_Pos.y + offY);
		ofLine(A_Pos.x, A_Pos.y , B_Pos.x, B_Pos.y );
	}

	if(animType == LayerData::TRAVERSING_3_POINT){
		ofDrawBitmapStringHighlight("A", A_Pos.x, A_Pos.y + offY);
		ofDrawBitmapStringHighlight("B", B_Pos.x, B_Pos.y + offY);
		ofDrawBitmapStringHighlight("C", C_Pos.x, C_Pos.y + offY);
		ofLine(A_Pos.x, A_Pos.y , B_Pos.x, B_Pos.y );
		ofLine(C_Pos.x, C_Pos.y , B_Pos.x, B_Pos.y );
	}

	ofNoFill();
	if(animType == LayerData::BOBBING ){
		ofDrawRectangle(pos.x - tex.getWidth() * 0.5f, pos.y - tex.getHeight() * 0.5f, tex.getWidth(), tex.getHeight());
	}else{
		ofDrawRectangle(pos.x, pos.y, tex.getWidth(), tex.getHeight());
	}
	ofFill();

	int lines = 8;
	string msg =
	"UID: " + UID + "\n" +
	"layer: " + ofToString(layer) + "\n" +
	"zoneUID: " + zoneUID + "\n" +
	"pos: " + ofToString(pos.x) + ", " + ofToString(pos.y) + "\n" +
	"state: " + toString(animState) + "\n" +
	"type: " + toString(animType);

	if(pause.isAnimating()){
		msg += "\npause Dur: " + ofToString(pause.getDuration()); lines++;
	}
	if(animFloat1.isAnimating()){
		msg += "\nanimFloat1 Dur: " + ofToString(animFloat1.getDuration()); lines++;
	}
	if(animFloat2.isAnimating()){
		msg += "\nanimFloat2 Dur: " + ofToString(animFloat2.getDuration()); lines++;
	}

	if(imgSequence){
		msg += "\nfr: " + ofToString(imgSequence->getPlaybackFramerate(),0) + "fps"; lines++;
	}
	if(imgSequence && animationProgress >= 0.0f){
		msg += "\npct: " + ofToString(100.0f * animationProgress, 1) + "%"; lines++;
	}
	ofDrawBitmapStringHighlight(msg, pos.x, pos.y + tex.getHeight() - 10 * lines, ofColor(0), textColor);
	ofSetColor(255);
}

#pragma mark ZONES

void MediaObject::setUID(string _UID)
{
    UID = _UID;
}

void MediaObject::setPosition(ofVec2f _pos)
{
    pos = _pos;
}


void MediaObject::setLayer(int _layer)
{
    layer = _layer;
}

void MediaObject::setZone(int _zoneOrder, string _zoneUID)
{
    zoneOrder = _zoneOrder;
    zoneUID = _zoneUID;
}

#pragma mark PLAYING SEQUENTIAL OBJECTS

void MediaObject::setNextObject(string _nextUID)
{
    nextUID =ofToUpper(_nextUID);
    sequentialObj = true;
}

void MediaObject::setPrevObject(string _prevUID)
{
    prevUID = ofToUpper(_prevUID);
    sequentialObj = true;
    animationVisble = false;
}

void MediaObject::sendPlayNextObject()
{
    
    //Send event via notification center
    ofxNotificationCenter::Notification mnd;

    if(nextUID.size())
    {
        mnd.data["next"] = nextUID;
        ofLogNotice("MediaObject") << UID << " send play next object " << nextUID;
    }
    else
    {
        mnd.data["next"] = prevUID;
        ofLogNotice("MediaObject") << UID << " send play next object " << prevUID;
    }


    ofxNotificationCenter::one().postNotification(LayerIDManager::one().playNextObject, mnd);
    makeAnimationInvisible();
    
}

bool MediaObject::isSequentialObject(){
	return isFirstSequentialObject() || isSecondSequentialObject();
}


bool MediaObject::isFirstSequentialObject()
{
    
     if (nextUID.size())
     {
         return true;
     }
    else
    {
        return false;
    }
}

bool MediaObject::isSecondSequentialObject()
{
    if (prevUID.size())
    {
        return true;
    }
    else
    {
        return false;
    }
}

string MediaObject::getNextOrPreviousObject(){

	if (nextUID.size() && prevUID.size()){
		ofLogError("MediaObject") << "getNextOrPreviousObject() this media object has both a NEXT and PREVIOUS object!";
	}
	if (nextUID.size()){
		return nextUID;
	}
	if (prevUID.size()){
		return prevUID;
	}
	return "";
}

#pragma mark GET

ofVec2f MediaObject::getPos()
{
    return pos; 
}

#pragma mark ANIMATION TYPES

void MediaObject::setAnimationType( LayerData::AnimationType _animType)
{
    animType = _animType;
    
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: IMAGE_SEQUENCE"; break;}
        case LayerData::TRAVERSING_2_POINT: {
            ofLogNotice("MediaObject") << UID << " ANIMTYPE: TRAVERSING_2_POINT";
            break;
        }
        case LayerData::ROTATING: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: ROTATING"; break;}
        case LayerData::STEM: {
            animFloat1.setCurve(EASE_IN_EASE_OUT);
            ofLogNotice("MediaObject") << UID << " ANIMTYPE: STEM"; break;}
        case LayerData::BOBBING: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: BOBBING"; break;}
        case LayerData::TRAVERSING_3_POINT: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: TRAVERSING_3_POINT"; break;}
        case LayerData::STATIC: { ofLogNotice("MediaObject") << UID << " ANIMTYPE: STATIC"; break;}
        case LayerData::TWO_PT_SWAY: {
            ofLogNotice("TWO_PT_SWAY") << UID << " ANIMTYPE: TWO_PT_SWAY";
            animFloat1.setCurve(EASE_IN_EASE_OUT);
            
            break;}
        default:break;
    }
}

LayerData::AnimationType MediaObject::getAnimationType()
{
    return animType;
}


#pragma mark DURATIONS

void MediaObject::setDuration1(float _duration1)
{
    duration1 = _duration1;
	animFloat1.setDuration(duration1); 
}

void MediaObject::setDuration2(float _duration2)
{
    duration2 = _duration2;
	animFloat2.setDuration(duration2);
}

void MediaObject::setPauseDuration(float _durLow, float _durHigh)
{
	pauseDurationLow = _durLow ;
	pauseDurationHigh = _durHigh ;
	pause.setDuration(ofRandom(pauseDurationLow, pauseDurationHigh));
}


#pragma mark POSITIONS
void MediaObject::setB_Pos(ofVec2f _B_Pos)
{
    B_Pos = _B_Pos;
}

void MediaObject::setC_Pos(ofVec2f _C_Pos)
{
    C_Pos = _C_Pos;
}

#pragma mark TEXTURE

void MediaObject::setTexture(ofTexture &_tex)
{
    tex = _tex; 
}

#pragma mark ANIMATIONS

void MediaObject::setAnimationState(AnimationState _animState)
{
    animState  = _animState;
    
    switch(animState)
    {
        case AnimationState::STOPPED: 
		{
			animFloat1.pause(); 
			animFloat2.pause(); 
            break;
        }
            case AnimationState::PLAYING: {
            makeAnimationVisble();
            break;
        }
        case AnimationState::BRIEF_PAUSE: {
            
            if(animType == LayerData::TWO_PT_SWAY || animType == LayerData::STEM)
            {
                animFloat1.setRepeatType(LOOP_BACK_AND_FORTH_ONCE);
            }
            break;
        }
        default: break;
    }
}

void MediaObject::genericPlay()
{
	if (firstPlay)
	{
		animFloat1.animateFromTo(0.0f, 1.0f);
		firstPlay = false;
	}
	else
	{
		if(isSequentialObject() && animType == LayerData::AnimationType::IMAGE_SEQUENCE){ //sequential objects imgsequence re-start each other, not resume (bc they dont loop)
			animFloat1.animateFromTo(0.0f, 1.0f);
		}else{
			animFloat1.resume();
		}
	}
};

void MediaObject::triggerPlay()
{

    ofLogNotice("MediaObject") << "Toggle object-" << UID << " to playing";
    
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:
        case LayerData::TRAVERSING_2_POINT:
			genericPlay();
            break;

        case LayerData::ROTATING:
            animFloat1.setDuration(12);
			genericPlay();
            break;

        case LayerData::STEM:
			if (firstPlay){
				animFloat1.reset(0.0f);
				float delay = ofRandom(1.0f, 3.0f);
				animFloat1.animateToAfterDelay(1.0f, delay);
				animFloat1.setRepeatType(LOOP_BACK_AND_FORTH);
				firstPlay = false; 
			}
			animFloat1.resume();
            break;

        case LayerData::BOBBING:
			genericPlay();
			break; 

        case LayerData::TRAVERSING_3_POINT:
			genericPlay();
			break;

        case LayerData::STATIC:
			break;

        case LayerData::TWO_PT_SWAY:
			if (firstPlay){
				float delay = ofRandom(0.25f, 3.0f);
				animFloat1.reset(0.0f);
				animFloat1.animateToAfterDelay(1.0f, delay);
				animFloat1.setRepeatType(LOOP_BACK_AND_FORTH);
				firstPlay = false; 
			}
			animFloat1.resume();
            break;

        default:
			break;
    }
    
    setAnimationState(AnimationState::PLAYING);
    
}


string MediaObject::toString(AnimationState s){
	switch(s){
		case STOPPED: return "STOPPED";
		case PLAYING: return "PLAYING";
		case BRIEF_PAUSE: return "BRIEF_PAUSE";
	}
	return "AnimationState unknown state?";
}

string MediaObject::toString(LayerData::AnimationType s){
	switch(s){
		case LayerData::IMAGE_SEQUENCE: return "IMAGE_SEQUENCE";
		case LayerData::TRAVERSING_2_POINT: return "TRAVERSING_2_POINT";
		case LayerData::ROTATING: return "ROTATING";
		case LayerData::STEM: return "STEM";
		case LayerData::BOBBING: return "BOBBING";
		case LayerData::TRAVERSING_3_POINT: return "TRAVERSING_3_POINT";
		case LayerData::STATIC: return "STATIC";
		case LayerData::TWO_PT_SWAY: return "TWO_PT_SWAY";
	}
	ofLogError("MediaObject") << "toString() unknown LayerData::AnimationType!";
	return "Unknown LayerData::AnimationType";
}

void MediaObject::triggerStop()
{
	setAnimationState(AnimationState::STOPPED);
    //No longer having a "BRIEF PAUSE"
	//setAnimationState(AnimationState::BRIEF_PAUSE);
}

void MediaObject::makeAnimationVisble()
{
    animationVisble = true;
}

void MediaObject::makeAnimationInvisible()
{
    animationVisble = false;
}

#pragma mark BOBBING

void MediaObject::setBobDrift(ofVec2f _bobDrift)
{
    ofLogNotice("MediaObject") << "bobDrift " << UID << " : " << bobDrift;
    bobDrift = _bobDrift;
}

void MediaObject::setBobRotation(float _bobRotation)
{
    bobRotation = _bobRotation; 
}

void MediaObject::setRandomnVariance(ofVec2f _randomnBobVal)
{
    randomnBobVal = _randomnBobVal;
}

#pragma mark ANIMATION CALLBACKS

void MediaObject::onAnim1Finish(ofxAnimatable::AnimationEvent & event)
{

	//ofLogNotice() << "onAnim1Finish " << ofGetElapsedTimef();
    switch(animType)
    {
		case LayerData::IMAGE_SEQUENCE:
        case LayerData::TRAVERSING_2_POINT:
        {
            pause.setDuration(ofRandom(pauseDurationLow, pauseDurationHigh));
			pause.animateFromTo(0.0f, 1.0f);
            setAnimationState(AnimationState::BRIEF_PAUSE);
            break;
        }
        case LayerData::ROTATING:
        {
            
            if(animState == AnimationState::PLAYING)
            {
                animFloat1.animateFromTo(0.0f, 1.0f);
            }
            break;
        }
        case LayerData::STEM:{
            if(animState == AnimationState::PLAYING)
            {
                animFloat1.animateFromTo(0.0f, 1.0f);
            }
            break;
        }
        case LayerData::BOBBING:
        {
            //ofLogNotice() << "animFloat1.val(): " << animFloat1.val();
            
            if(animState == AnimationState::PLAYING && animFloat1.val() == 1)
            {
                animFloat1.animateFromTo(1.0f, 0.0f);
            }
            else if (animFloat1.val() == 0.0)
            {
                animFloat1.animateFromTo(0.0f, 1.0f);
            }
            break;
        }
        case LayerData::TRAVERSING_3_POINT:
        {
         
            setAnimationState(AnimationState::BRIEF_PAUSE);
			pause.setDuration(ofRandom(pauseDurationLow, pauseDurationHigh));
            pause.animateFromTo(0.0f, 1.0f);
            animFloat2.reset(0.0f);
            
            break;
        }
        case LayerData::STATIC:{break;}
        case LayerData::TWO_PT_SWAY:{
            break;
        }
        default:break;
    }
}

void MediaObject::onAnim2Finish(ofxAnimatable::AnimationEvent & event)
{
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:{ break;}
        case LayerData::TRAVERSING_2_POINT:{break;}
        case LayerData::ROTATING:{break;}
        case LayerData::STEM:{break;}
        case LayerData::BOBBING:{ break; }
        case LayerData::TRAVERSING_3_POINT:{
            
            
            setAnimationState(AnimationState::BRIEF_PAUSE);
			pause.setDuration(ofRandom(pauseDurationLow, pauseDurationHigh));
            pause.animateFromTo(0.0f, 1.0f);
            
            break;
        }
        case LayerData::STATIC:{break;}
        case LayerData::TWO_PT_SWAY:{break;}
        default:break;
    }
}

void MediaObject::onPauseFinish(ofxAnimatable::AnimationEvent & event)
{
    switch(animType)
    {
        case LayerData::IMAGE_SEQUENCE:
        case LayerData::TRAVERSING_2_POINT:{
            if(sequentialObj)
            {
                sendPlayNextObject();
                setAnimationState(MediaObject::STOPPED);
            }
            else
            {
                animFloat1.animateFromTo(0.0f, 1.0f); 
            }
            
            break;
        }
        case LayerData::ROTATING:{break;}
        case LayerData::STEM:{break;}
        case LayerData::BOBBING:{ break; }
        case LayerData::TRAVERSING_3_POINT:{
            
            
            if(animState == AnimationState::BRIEF_PAUSE && !animFloat2.val())
            {
                animFloat2.animateFromTo(0.0f, 1.0f);
                setAnimationState(MediaObject::PLAYING);
            }
            else if(sequentialObj)
            {
                sendPlayNextObject();
                setAnimationState(MediaObject::STOPPED); 
            }
            else
            {
                animFloat1.animateFromTo(0.0f, 1.0f);
                setAnimationState(MediaObject::PLAYING);
            }
            
            break;
        }
        case LayerData::STATIC:{break;}
        case LayerData::TWO_PT_SWAY:{break;}
        default:break;
    }

}


#pragma mark DEBUG

void MediaObject::setDebug(bool _debug)
{
    debug = _debug; 
}

void MediaObject::setMeshDebug(bool _meshDebug)
{
    meshDebug = _meshDebug;
}

#pragma mark CALLBACKS

void MediaObject::onKeyPressed(ofKeyEventArgs & args)
{
    switch(args.key)
    {
        case 'M': { debug = !debug; break; }
        default: break;
    }
}

#pragma mark MESH

void MediaObject::setAmplitude(int index, float*amplitude)
{
    amplitudes[index] = amplitude; 
}
