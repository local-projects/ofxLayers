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
	pause.setDuration(0.001);
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

MediaObject::AnimationState MediaObject::getAnimationState(){
	return animState;

};

void MediaObject::reset(){

	triggerStop(true);
	animFloat1.reset(0);
	animFloat2.reset(0);
	pause.reset(0);
	setAnimationState(STOPPED);
	pos = A_Pos;
	firstPlay = true;
	shouldPauseWhenReachingEnd = false;
	if(isSequentialObject()){
		if(isFirstSequentialObject()) makeAnimationVisble();
		if(isSecondSequentialObject()) makeAnimationInvisible();
	}else{
		makeAnimationVisble();
	}
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

	//if(animState == PLAYING || animState == BRIEF_PAUSE || animState == SHOULD_STOP){
	switch(animType){
		case LayerData::IMAGE_SEQUENCE:
			break;

		case LayerData::TRAVERSING_2_POINT:
			pos = A_Pos + (B_Pos - A_Pos)*animFloat1.val();
			break;

		case LayerData::ROTATING:
			break;

		case LayerData::STEM:
			if(animFloat1.isAnimating() && animFloat1.getPercentDone() <= 0.005){ //randomize duration of stems on every loop to have more randomness
				animFloat1.setDuration(duration1 * ofRandom(0.8, 1.0f/0.8f));
				int ran = ofGetFrameNum()%9;
				switch(ran){ //some change that we will change anim curve
					case 0: animFloat1.setCurve(EASE_IN_EASE_OUT); break;
					case 1: animFloat1.setCurve(TANH); break;
					case 2: animFloat1.setCurve(SWIFT_GOOGLE); break;
					case 3: animFloat1.setCurve(SMOOTHER_STEP); break;
					case 4: animFloat1.setCurve(EASE_IN_OUT_BACK); break;
					default: break;
				};
			}
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
	//}

	if((animState == PLAYING || animState == SHOULD_STOP) && imgSequence && !imgSequence->isPlaying()){
		//correct wrong state leak, if we have movie and its not playing, we are not playing
		//ofLogNotice("MediaObject") << "state leak correction on " << UID << " , we are now stopped";
		if(!isSequentialObject()){
			animState = STOPPED;
			ofLogNotice("MediaObject") << "stopping " << UID;
		}else{
			bool isFirst = isFirstSequentialObject();
			bool isSecond = isSecondSequentialObject();
			if(!isFirst && isSecond && animState == SHOULD_STOP){
				animState = STOPPED;
				ofLogNotice("MediaObject") << "stopping " << UID << " first: " << isFirst << " second: " << isSecond;
			}else{
				//ofLogNotice("MediaObject") << "stopping " << UID << " SPECIAL CASE! first: " << isFirst << " second: " << isSecond;
			}
		}
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

			const float val = animFloat1.val(); // range is [0..1]
			float v2 = (val - 0.5) * 2; // remap range to [-1,1]
			//Create Values
			float value0 = (*amplitudes[0]) * v2;
			float value1 = (*amplitudes[1]) * v2;
			float value2 = (*amplitudes[2]) * v2;
			float value3 = (*amplitudes[3]) * v2;
			float value4 = (*amplitudes[4]) * v2;
			float value5 = (*amplitudes[5]) * v2;
			float value6 = (*amplitudes[6]) * v2;
			float value7 = (*amplitudes[7]) * v2;

			//Add positions
			ofVec3f tl = ofVec3f(A_Pos.x + offset.x + value2, A_Pos.y + offset.y + value3);
			ofVec3f tr = ofVec3f(A_Pos.x + offset.x + tex.getWidth() + value4, A_Pos.y + offset.y + value5 );

			ofVec3f ml = ofVec3f(A_Pos.x + offset.x + value0, A_Pos.y + offset.y + tex.getHeight()/2 + value1);
			ofVec3f mr = ofVec3f(ofVec3f(A_Pos.x + offset.x + tex.getWidth() + value6, A_Pos.y + offset.y + tex.getHeight()/2 + value7));

			ofVec3f br = ofVec3f(ofVec3f(A_Pos.x + offset.x + tex.getWidth(), A_Pos.y + offset.y + tex.getHeight()));
			ofVec3f bl = ofVec3f(ofVec3f(A_Pos.x + offset.x , A_Pos.y + offset.y + tex.getHeight()));

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

				ofSetColor(ofColor::brown);
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
			ofVec3f tl = ofVec3f(A_Pos.x + offset.x + value0 , A_Pos.y + offset.y + value1 );
			ofVec3f tr = ofVec3f(A_Pos.x + offset.x + tex.getWidth() + value2, A_Pos.y + offset.y + value3 );
			ofVec3f br = ofVec3f(ofVec3f(A_Pos.x + offset.x + tex.getWidth(), A_Pos.y + offset.y + tex.getHeight()));
			ofVec3f bl = ofVec3f(ofVec3f(A_Pos.x + offset.x, A_Pos.y + offset.y + tex.getHeight()));

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
	if(animState == PLAYING) textColor = ofColor::red;
	if(animState == STOPPED) textColor = ofColor::magenta;
	if(animState == BRIEF_PAUSE) textColor = ofColor::gray;
	if(animState == SHOULD_STOP) textColor = ofColor::orange;


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

	int lines = 7;
	string msg =
	UID + "\n" +
	"Layer: " + ofToString(layer) + "  Zone: " + zoneUID + "\n" +
	"pos: " + ofToString(pos.x) + ", " + ofToString(pos.y) + "\n" +
	"State: " + toString(animState) + "\n" +
	"Type: " + toString(animType);

	if(pause.isAnimating()){
		msg += "\nPause Dur: " + ofToString(pause.getDuration(),1);
		msg += "  pct: " + ofToString(pause.getPercentDone() * 100,1);
		lines++;
	}
	if(animFloat1.isAnimating()){
		msg += "\nAnimFloat1 Dur: " + ofToString(animFloat1.getDuration(),1);
		msg += "  pct: " + ofToString(animFloat1.getPercentDone() * 100,1) + "  val: " + ofToString(animFloat1.val(), 2);
		lines++;
	}
	if(animFloat2.isAnimating()){
		msg += "\nAnimFloat2 Dur: " + ofToString(animFloat2.getDuration(),1);
		msg += "  pct: " + ofToString(animFloat2.getPercentDone() * 100,1);
		lines++;
	}

	if(imgSequence){
		msg += "\nFr: " + ofToString(imgSequence->getPlaybackFramerate(),0) + "fps";
		msg += "  Visible: " + string(animationVisble ? "YES" : "NO");
		lines++;
	}
	if(imgSequence && animationProgress >= 0.0f){
		msg += "\nPct: " + ofToString(100.0f * animationProgress, 1) + "%";
		msg += "  ImgSeq: " + string(imgSequence->isPlaying() ? "PLAY" : "STOP");
		lines++;
	}
	bool sequential = isSequentialObject();
	if(sequential){
		msg += "\nSequential! nextObj: " + getNextOrPreviousObject();
		lines++;
	}
	ofDrawBitmapStringHighlight(msg, pos.x, pos.y + tex.getHeight() - 10 * lines, sequential ? ofColor(22,22,55) : ofColor::black, textColor);
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

void MediaObject::sendPlayNextObject(){

	ofxNotificationCenter::Notification mnd;

	ofLogNotice("MediaObject") << "sendPlayNextObject() " << UID;
	if(nextUID.size()){
		mnd.data["next"] = nextUID;
		//ofLogNotice("MediaObject") << UID << " send play next object " << nextUID;
	}else{
		mnd.data["next"] = prevUID;
		//ofLogNotice("MediaObject") << UID << " send play next object " << prevUID;
	}
	mnd.data["shouldPause"] = shouldPauseWhenReachingEnd;

	ofxNotificationCenter::one().postNotification(LayerIDManager::one().playNextObject, mnd);
	if(imgSequence){
		imgSequence->seekToFrame(0);
		imgSequence->update(0);
	}
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
			animFloat1.pause();
			animFloat2.pause();
			shouldPauseWhenReachingEnd = false;
			break;

		case AnimationState::SHOULD_STOP:
			shouldPauseWhenReachingEnd = true;
			break;

		case AnimationState::PLAYING:
			makeAnimationVisble();
			shouldPauseWhenReachingEnd = false;
			break;

		case AnimationState::BRIEF_PAUSE:
			if(animType == LayerData::TWO_PT_SWAY || animType == LayerData::STEM){
				animFloat1.setRepeatType(LOOP_BACK_AND_FORTH_ONCE);
			}
			break;

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
			if(animState == AnimationState::STOPPED){ //dont re-trigger animation timers (ugh) if we are to play but we were already playing, otherwise the img sequence and the anim timer get off-sync
				animFloat1.animateFromTo(0.0f, 1.0f);
			}
		}else{
			animFloat1.resume();
		}
	}
};

void MediaObject::triggerPlay(bool shouldStopAfterDone){

	ofLogNotice("MediaObject") << "Toggle object-" << UID << " to playing. frame:" << ofGetFrameNum();

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
				float delay = ofRandom(0.1f, 4.0f);
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

	if(shouldStopAfterDone){
		setAnimationState(AnimationState::SHOULD_STOP);
	}else{
		setAnimationState(AnimationState::PLAYING);
	}
}


string MediaObject::toString(AnimationState s){
	switch(s){
		case STOPPED: return "STOPPED";
		case PLAYING: return "PLAYING";
		case BRIEF_PAUSE: return "BRIEF_PAUSE";
		case SHOULD_STOP: return "SHOULD_STOP";
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

void MediaObject::triggerStop(bool stopRightNow)
{

	if(stopRightNow){

		setAnimationState(AnimationState::STOPPED);

	}else{ //some anim types are endless, so we stop they right now
		//anims that have an img sequence or have a set path, will stop when anim is done or path is all walked.

		switch (animType) {
			case LayerData::IMAGE_SEQUENCE:
			case LayerData::TRAVERSING_2_POINT:
			case LayerData::TRAVERSING_3_POINT:
				setAnimationState(AnimationState::SHOULD_STOP);
				break;

			case LayerData::STATIC:
			case LayerData::ROTATING:
			case LayerData::STEM:
			case LayerData::BOBBING:
			case LayerData::TWO_PT_SWAY:
				setAnimationState(AnimationState::STOPPED);
				break;

			default:
				break;
		}
	}
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

	//ofLogNotice("MediaObject") << "onAnim1Finish " << UID;
	switch(animType)
	{
		case LayerData::IMAGE_SEQUENCE:
		case LayerData::TRAVERSING_2_POINT:
		{
			if(animState == PLAYING || animState == SHOULD_STOP){

				if(isSequentialObject() && isSecondSequentialObject()){ // 2nd part of seq anim, we don't pause at all
					pause.setDuration(0.0);
				}else{
					pause.setDuration(ofRandom(pauseDurationLow, pauseDurationHigh));
				}
				pause.animateFromTo(0.0f, 1.0f);
				setAnimationState(AnimationState::BRIEF_PAUSE);
			}
			break;
		}
		case LayerData::ROTATING:
		{

			if(animState == AnimationState::PLAYING){
				animFloat1.animateFromTo(0.0f, 1.0f);
			}
			if(animState == SHOULD_STOP){
				reset();
			}
			break;
		}
		case LayerData::STEM:{
			if(animState == AnimationState::PLAYING){
				animFloat1.animateFromTo(0.0f, 1.0f);
			}
			if(animState == SHOULD_STOP){
				reset();
			}
			break;
		}
		case LayerData::BOBBING:
		{
			//ofLogNotice() << "animFloat1.val(): " << animFloat1.val();

			if(animState == AnimationState::PLAYING && animFloat1.val() == 1){
				animFloat1.animateFromTo(1.0f, 0.0f);
			}
			else if (animFloat1.val() == 0.0)
			{
				animFloat1.animateFromTo(0.0f, 1.0f);
			}

			if(animState == SHOULD_STOP){
				reset();
			}

			break;
		}
		case LayerData::TRAVERSING_3_POINT:
		{
			//for this type, if we are SHOULD_STOP, we stop at the end of the 2nd animation
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
	//ofLogNotice("MediaObject") << "onAnim2Finish " << UID;
	switch(animType)
	{
		case LayerData::IMAGE_SEQUENCE:
			break;
		case LayerData::TRAVERSING_2_POINT:
			break;
		case LayerData::ROTATING:
			break;
		case LayerData::STEM:
			break;
		case LayerData::BOBBING:
			break;

		case LayerData::TRAVERSING_3_POINT:
			if(animState == PLAYING){
				setAnimationState(AnimationState::BRIEF_PAUSE);
				pause.setDuration(ofRandom(pauseDurationLow, pauseDurationHigh));
				pause.animateFromTo(0.0f, 1.0f);
			}
			if(animState == SHOULD_STOP){
				reset();
			}
			break;

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

			if(sequentialObj){ //compound obj, 2 MO linked
				sendPlayNextObject();
				setAnimationState(MediaObject::STOPPED);

			}else{ //simple obj

				if(shouldPauseWhenReachingEnd){
					setAnimationState(MediaObject::SHOULD_STOP);
				}else{
					setAnimationState(MediaObject::PLAYING);
				}
				animFloat1.animateFromTo(0.0f, 1.0f);
			}
			break;
		}
		case LayerData::ROTATING:{break;}
		case LayerData::STEM:{break;}
		case LayerData::BOBBING:{ break; }
		case LayerData::TRAVERSING_3_POINT:{

			if(animState == AnimationState::BRIEF_PAUSE && !animFloat2.val()){
				animFloat2.animateFromTo(0.0f, 1.0f);
				if(shouldPauseWhenReachingEnd){
					setAnimationState(MediaObject::SHOULD_STOP);
				}else{
					setAnimationState(MediaObject::PLAYING);
				}
			}
			else if(sequentialObj)
			{
				sendPlayNextObject();
				setAnimationState(MediaObject::STOPPED);
			}
			else
			{
				animFloat1.animateFromTo(0.0f, 1.0f);
				setAnimationState(MediaObject::PLAYING); //TODO: unclear what happens here
				ofLogError("MediaObject") << "onPauseFinish() THIS CASE NEEDS CHECKING! TODO!";
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
