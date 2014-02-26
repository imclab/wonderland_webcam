#include "testApp.h"
using namespace cv;

//--------------------------------------------------------------
void testApp::setup()
{

    camWidth = 1280;
    camHeight = 720;
    threshNum = 3;
    
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(60);
    
    vidGrabber.initGrabber(camWidth, camHeight);
    
    tex.setTextureWrap(GL_REPEAT, GL_REPEAT);
    tex.allocate(camWidth,camHeight,GL_RGB,GL_RGBA);
    tbIm.allocate(camWidth,camHeight,OF_IMAGE_COLOR_ALPHA);
    
    cvIm.allocate(camWidth, camHeight);
    for(int i = 0; i < threshNum; i++)
    {
        cvGrIm[i].allocate(camWidth,camHeight);
    }

    coordX = 50;
    coordY = 50;
    float coordYf = (float) coordY;
    float coordXf = (float) coordX;
    float widthf = (float) camWidth;
    float heightf = (float) camHeight;
    coordLength = coordX * coordY;
    ratioCoordX = widthf / (coordXf-1.0);
    ratioCoordY = heightf / (coordYf-1.0);
    
    vertexCoords.resize(coordLength);
    texCoords.resize(coordLength);
    
    for(int y = 0; y < coordY; y++)
    {
        for(int x = 0; x < coordX; x++)
        {
            vertexCoords.at((y*(coordX))+x).set(ratioCoordX*x,ratioCoordY*y,0.0);
            texCoords.at((y*(coordX))+x).set(ratioCoordX*x,ratioCoordY*y);
        }
    }
    
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    mesh.addTexCoords(texCoords);
    mesh.addVertices(vertexCoords);
    
    //ADD INDICIES TO MATRIX FOR TRIANGLE STRIP MESH DRAWING ORDER
    //code based on code from here
    //http://www.learnopengles.com/android-lesson-eight-an-introduction-to-index-buffer-objects-ibos/
    
    for(short y = 0; y < coordY-1; y++)
    {
        if( y > 0)
        {
            mesh.addIndex(y*coordY);
        }
        
        for(short x = 0; x < coordX; x++)
        {
            mesh.addIndex((y*coordY)+x);
            mesh.addIndex(((y+1)*coordY)+x);
        }
        if(y < coordY -2)
        {
            mesh.addIndex(((y+1)*coordY)+(coordX - 1));
        }
    }
    
    for(int i = 0; i < 9; i++)
    {
        cout << " - ";
        cout<< mesh.getTexCoord(i);
    }
    
        meshCopy = mesh;
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    vidGrabber.update();
    if (vidGrabber.isFrameNew())
    {
        cvIm.setFromPixels(vidGrabber.getPixels(), 1280, 720);
        
        for(int i = 0; i < threshNum; i++)
        {
            cvGrIm[i] = cvIm;
            cvGrIm[i].threshold((i+1)*(255/(threshNum+1)));
            cvContour[i].findContours(cvGrIm[i], (1280*720)/10000, (1280*720)/50, 6, true,false);
        }
        for(int j = 0; j < camWidth; j++)
        {
            for(int k = 0; k < camHeight; k++)
            {
                tbIm.setColor(j,k,vidGrabber.getPixelsRef().getColor(j, k));
            }
        }
        tbIm.reloadTexture();
        tex = tbIm.getTextureReference();
    }
    
    
    
    // Get the mouse location - it must be relative to the center of our screen because of the ofTranslate() command in draw()
    
    bool stretch = true;
    mesh = meshCopy;
    for(int a = 0; a < threshNum; a++)
    {
    for (int i = 0; i < cvContour[a].nBlobs; i++)
    {
        ofVec3f mouse;
        mouse.set(cvContour[a].blobs[i].centroid.x, cvContour[a].blobs[i].centroid.y);
    
        // Loop through all the vertices in the mesh and move them away from the mouses
        for (int j=1; j<coordY-1; j++)
        {
            for (int k=1; k<coordX-1; k++)
            {
               int indexM  = (j*coordY) + k;
            ofVec3f vertex = mesh.getVertex(indexM);
            float distanceToMouse = mouse.distance(vertex);
            
            // Scale the displacement based on the distance to the mouse
            // Small distance to mouse should yield a small displacement

            float displacement;
            if(stretch)
            {
                displacement = ofMap(distanceToMouse, 0, 350, 15.0, 0, true);
            }
            else
            {
                displacement = ofMap(distanceToMouse, 0, 350, 15.0, 0, true);
            }

            
            // Calculate the direction from the mouse to the current vertex
            ofVec3f direction = vertex - mouse;
            
            // Normalize the direction so that it has a length of one
            // This lets us easily change the length of the vector later
            direction.normalize();
            
            // Push the vertex in the direction away from the mouse and push it
            // a distance equal to the value of the variable displacement
            ofVec3f displacedVertex = vertex + displacement*direction;
            mesh.setVertex(indexM, displacedVertex);
            }
        }
         stretch = !stretch;
    }
    }

    
}

//--------------------------------------------------------------
void testApp::draw()
{

    // ofEnableNormalizedTexCoords();
    //cvIm.draw(0,0);
    //cvContour.draw(0,0,1280,720);

    tex.bind();
    mesh.draw();
    tex.unbind();
    
    
    for(int a = 0; a < threshNum; a++)
    {
        for (int i = 0; i < cvContour[a].nBlobs; i++)
        {
            ofEllipse(cvContour[a].blobs[i].centroid.x, cvContour[a].blobs[i].centroid.y, 10, 10);
        }
    cvContour[a].draw(0,0,1280,720);
    }
    
    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
