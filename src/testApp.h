#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "cv.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void meshInit();
        void meshWarp();
        void imageUpdate();
        void applyLaplacian();
    
    int threshNum;
    ofTexture tex;
    ofMesh mesh, meshCopy;
    ofFbo framebuffer;
    vector< unsigned short int > indexData;
    vector< ofVec2f > texCoords;
    vector< ofVec3f > vertexCoords;
    int coordLength, coordX, coordY, camWidth, camHeight;
    float ratioCoordX, ratioCoordY;
    
    ofVideoGrabber vidGrabber;
    ofImage tbIm;
    ofxCvColorImage	 cvIm;
    ofxCvGrayscaleImage cvGrIm[6];
    ofxCvContourFinder cvContour[6];

		
};
