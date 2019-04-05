#include "ofApp.h"


void ofApp::setup(){

	CustomApp::setup();

	// LISTENERS
	ofAddListener(screenSetup.setupChanged, this, &ofApp::setupChanged);
	ofAddListener(RUI_GET_OF_EVENT(), this, &ofApp::remoteUIClientDidSomething);


	// PARAMS
	RUI_NEW_GROUP("PARAMS");
	RUI_SHARE_PARAM(p1, -1, 1);

	ofLogNotice() << "hello!";
    
    assetMan = new AssetManager();
    assetMan->setup();
    
    layers = new ofxLayerManager();
    layers->setup();
    
}


void ofApp::update(){

	float dt = 1./60.;
}


void ofApp::draw(){
    
    //auto &tex;
    
    for(int i = 0; i < layers->getNumLayers(); i++)
    {
        auto *lay = layers->getLayer(i);
        
        for(int j = 0; j < lay->getNumMediaObjects(); j++)
        {
            string mediaObj_UID = lay->getMediaObjectUID(j);
            
            auto *mediaObj = lay->getObject(mediaObj_UID);
            auto *img = assetMan->getImage(mediaObj_UID);
            
            img->draw(mediaObj->getPos().x, mediaObj->getPos().y);
        }
    }
}


void ofApp::keyPressed(int key){

	if(key == 'w'){
		screenSetup.cycleToNextScreenMode();
	}

	if(key == 'L'){
		ofxSuperLog::getLogger()->setScreenLoggingEnabled(!ofxSuperLog::getLogger()->isScreenLoggingEnabled());
	}
}


void ofApp::keyReleased(int key){

}


void ofApp::mouseMoved(int x, int y ){

}


void ofApp::mouseDragged(int x, int y, int button){

}


void ofApp::mousePressed(int x, int y, int button){

}


void ofApp::mouseReleased(int x, int y, int button){

}


void ofApp::windowResized(int w, int h){

}


void ofApp::gotMessage(ofMessage msg){

}


void ofApp::dragEvent(ofDragInfo dragInfo){
	
}


//////// CALLBACKS //////////////////////////////////////

void ofApp::setupChanged(ofxScreenSetup::ScreenSetupArg &arg){
	ofLogNotice()	<< "ofxScreenSetup setup changed from " << screenSetup.stringForMode(arg.oldMode)
	<< " (" << arg.oldWidth << "x" << arg.oldHeight << ") "
	<< " to " << screenSetup.stringForMode(arg.newMode)
	<< " (" << arg.newWidth << "x" << arg.newHeight << ")";
}


//define a callback method to get notifications of client actions
void ofApp::remoteUIClientDidSomething(RemoteUIServerCallBackArg &arg){
	switch (arg.action) {
		case CLIENT_CONNECTED: cout << "CLIENT_CONNECTED" << endl; break;
		case CLIENT_DISCONNECTED: cout << "CLIENT_DISCONNECTED" << endl; break;
		case CLIENT_UPDATED_PARAM: cout << "CLIENT_UPDATED_PARAM: "<< arg.paramName << " - ";
			arg.param.print();
			break;
		case CLIENT_DID_SET_PRESET: cout << "CLIENT_DID_SET_PRESET" << endl; break;
		case CLIENT_SAVED_PRESET: cout << "CLIENT_SAVED_PRESET" << endl; break;
		case CLIENT_DELETED_PRESET: cout << "CLIENT_DELETED_PRESET" << endl; break;
		case CLIENT_SAVED_STATE: cout << "CLIENT_SAVED_STATE" << endl; break;
		case CLIENT_DID_RESET_TO_XML: cout << "CLIENT_DID_RESET_TO_XML" << endl; break;
		case CLIENT_DID_RESET_TO_DEFAULTS: cout << "CLIENT_DID_RESET_TO_DEFAULTS" << endl; break;
		default:
			break;
	}
}
