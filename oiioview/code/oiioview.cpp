/* Author: Goutham Pacha Ravi <gpachar@clemson.edu>
References: 
	1) Code Samples provided by Dr. Joshua Levine, specially glsquare.cpp and rgbcube.cpp
			URL: http://people.cs.clemson.edu/~levinej/courses/6040/
	2) Mouse Events and Keyboard Handling techniques: 
			URL: http://www.glprogramming.com/red/
				 http://www.glprogramming.com/blue/
	3) OpenImageIO documentation
			URL: https://github.com/OpenImageIO/oiio/blob/master/src/doc/openimageio.pdf

Usage: 
1) One parameter mode: ./oiioview cube.ppm
will simply read and display the image file named cube.ppm
2) Two parameter mode: ./oiioview cube.ppm newcube.png
will read and display cube.ppm and also make a new image file named newcube.png.

*/

#include "inc/oiioview.h"

/*********************************************************
 *  This function takes a file path as argument<filename>
 *  and checks whether it exists. It returns true if it does,
 *  false if it doesn't
 *  Input: filename (std::string)
 *  Return: true/false (boolean)
 *********************************************************/
/* Function to check whether a given path is valid */
 bool file_exists(const std::string& filename) {
 	std::ifstream ifile(filename.c_str());
 	return ifile;
 }



myImage image;

//The viewport height and width
int WIDTH;
int HEIGHT;
std::vector<std::string> filenames;
int curIndex = 0;
int flipFlag = 0;

std::vector<unsigned char> copiedImage;


/*********************************************************
 *  This function allows flipping of the given image or
 *  displaying its red, green or blue channels in isolation
 *  Input: choice of function (int)
 *  Return: -
 *********************************************************/
void myImage::changeColor(int choice) {

	//Are we only flipping??
	if(choice != 4) {	
		copiedImage.clear();
		if(readImage(filenames[curIndex], getWriteFlag())==0) {

		 		WIDTH = getWidth(); 
		 		HEIGHT = getHeight();
		 		glutReshapeWindow(WIDTH,HEIGHT);
	 	}
 	}


 	int s = WIDTH*HEIGHT*4;
	//Copy image that is being displayed to OriginalImage
	//size_t s = sizeof(unsigned char*) pixels;
	copiedImage.reserve(WIDTH*HEIGHT*4);

	for (int i = 0; i<s; i++) {

		copiedImage.push_back(pixels[i]);//originalImage[i] = ;
	}



	switch(choice) {
		case 1: for(int x = 0; x < s-4; x = x+4) {
					copiedImage.at(x+1) = 0;
					copiedImage.at(x+2) = 0;
				 } break;
		case 2: for(int x = 0; x < s-4; x = x+4) {
					copiedImage.at(x) = 0;
					copiedImage.at(x+2) = 0;
				 } break;
		case 3: for(int x = 0; x < s-4; x = x+4) {
					copiedImage.at(x) = 0;
					copiedImage.at(x+1) = 0;
				 } break;

		case 4: int c = 0;

				int fele = (HEIGHT*WIDTH*4) - WIDTH*4;
				while(fele >= 0) {
				for(int y = 0; y<WIDTH*4; y++) {
						copiedImage.at(c++) = pixels[y+fele];
					}
					fele -= WIDTH*4;
				}
				
				break;

	}

	delete [] pixels;

	pixels = new unsigned char[s];

	for (int i = 0; i<s; i++) {

		pixels[i] = copiedImage.at(i);//originalImage[i] = ;
	}

	//pixels = &copiedImage[0];
	glutPostRedisplay();

}



/*********************************************************
 *  Constructor to myImage. Initializes the writeFlag to -1
 *  and sets the out_filename to "" 
 *  Input: -
 *  Return: -
 *********************************************************/
 myImage::myImage() : writeFlag(-1) {
	//TODO - Fill in some commands here 
 	out_filename = "";
 }

/*********************************************************
 *  Member function writeImage takes no arguments, but reads
 *  the output file_name from the member variable of myImage
 *  and writes the image into the output file specified
 *  Input: -
 *  Return: integer (0 if successful, -1 if not)
 *********************************************************/
 int myImage::writeImage() {

 	if(out_filename == "") {
 		std::cout<<"No filename has been set to write output"<<std::endl;
 		return -1;
 	}

 	char* oFileName = &out_filename[0];
 	int nchannels = 4;


 	int cur_window_width = glutGet(GLUT_WINDOW_WIDTH);
 	int cur_window_height = glutGet( GLUT_WINDOW_HEIGHT );

 	if(cur_window_height < 0) {
 		std::cout<<"Here is the problem";
 		std::cout<<"Cur window height";
 	} else {
 		std::cout<<"Cur window height "<<cur_window_height;
 		std::cout<<" dims "<<cur_window_width * cur_window_height * nchannels;
 	}

 	std::cout<<std::endl<<"Window width "<<cur_window_width;

	std::vector<unsigned char> opixels(cur_window_width * cur_window_height * nchannels);
	std::vector<unsigned char> flipped(cur_window_width * cur_window_height * nchannels);

	
	//Read the entire image at once
	glReadPixels(0,0,cur_window_width,cur_window_height , GL_RGBA,  GL_UNSIGNED_BYTE,  &opixels[0]);


	//Flipping the image
	int c = 0;

		int fele = (HEIGHT*WIDTH*4) - WIDTH*4;
		while(fele >= 0) {
		for(int y = 0; y<WIDTH*4; y++) {
				flipped.at(c++) = opixels.at(y+fele);
			}
			fele -= WIDTH*4;
		}






	

	
 	ImageOutput *out = ImageOutput::create (oFileName);
 	if (!out) {
 		std::cout<<"Could not write the image into "<<oFileName<<std::endl;
 		std::cout<<"Error from OpenImageIO : "<<geterror();
 		return -1;
 	}
 	ImageSpec spec (xRes, yRes, nchannels, TypeDesc::UINT8);
 	out->open (oFileName, spec);

	out->write_image(TypeDesc::UINT8, &flipped[0]);


 	out->close ();

 	delete out;


 	return 0;
 }



/*********************************************************
 *  Member function readImage takes as arguments, the name
 *  of the file to be read and a writeFlag value set in the
 *  main method. It sets the writeFlag member variable to 
 *  a valid value if the image opens fine
 *  Input: filename (std::string) and whether or not writing is permitted (int)
 *  Return: 0 if successful, -1 if not. (integer)
 *********************************************************/
 int myImage::readImage(std::string filename, int writeFlag) {

 	ImageInput *image = ImageInput::open (filename);
 	if (!image) {
 		std::cout<<std::endl<<"Could not open the image "<<filename<<" . Is this a valid image file?"<<std::endl;
 		std::cout<<std::endl<<"Error message from OpenImageIO: "<<OpenImageIO::geterror()<<std::endl;
 		return -1;
 	}
 	const ImageSpec &image_spec = image->spec();
 	xRes = image_spec.width;
 	yRes = image_spec.height;
 	channels = image_spec.nchannels;
 	pixels = new unsigned char[xRes*yRes*channels];

	//Flipping the image read for OpenGL's sake
 	int scanlinesize = xRes * channels * sizeof(pixels[0]);
 	image->read_image (TypeDesc::UINT8,
	(char *)pixels+(yRes-1)*scanlinesize, // offset to last
	AutoStride, // default x stride
	-scanlinesize, // special y stride
	AutoStride); // default z stride


	// //Printing ImageSpecs
 // 	std::cout<<std::endl<<"Image x resolution: "<<xRes<<std::endl;
 // 	std::cout<<std::endl<<"Image y resolution: "<<yRes<<std::endl;
 // 	std::cout<<std::endl<<"Image channels: "<<channels<<std::endl;

	//If channels are 3, convert data to rgba - Set Alpha to 1. 
 	if(channels == 3) {
 		unsigned char* pixels2 = new unsigned char[xRes*yRes*4];
 		int j = 0;
 		for(int i = 0; i < (xRes*yRes*4 - 4); i=i+4) {
 			pixels2[i] = pixels[j];
 			pixels2[i+1] = pixels[j+1];
 			pixels2[i+2] = pixels[j+2];
			pixels2[i+3] = 255; //Alpha Channel - Opaque
			j = j+3;
		}
		delete [] pixels;
		pixels = pixels2;
	}
	
	image->close();
	delete image;
	return 0;

}



/*********************************************************
 *  Destructor freeing up the allocated space from pointer
 *  data member
 *  Input: -
 *  Return: -
 *********************************************************/
 myImage::~myImage() {
	//Release the image, let it go.
 	std::cout<<std::endl<<"Destroyed";
 	//delete [] pixels;
 }


/*********************************************************
 *  This is the display callback method that (flips and)
 *  displays the image in a window
 *  Input: -
 *  Return: -
 *********************************************************/
 void displayImage(){

 	glPixelZoom(1.0, 1.0);
 	glRasterPos2i(0,0);

 	glDrawPixels(image.getWidth(),image.getHeight(),GL_RGBA, GL_UNSIGNED_BYTE, image.getImageData());

 	int current_width = glutGet(GLUT_WINDOW_WIDTH);
 	int current_height = glutGet(GLUT_WINDOW_HEIGHT);
 	if(current_width != WIDTH || current_height != HEIGHT) {
 		int XAspectRatio = WIDTH/current_width;
 		int YAspectRatio = HEIGHT/current_height;

 		if(XAspectRatio <= 1 && YAspectRatio <=1) {
 			glPixelZoom(XAspectRatio, YAspectRatio);
 			glRasterPos2i(0,0);
 			glViewport(0,0,current_width,current_height);
 			glDrawPixels(image.getWidth(),image.getHeight(),GL_RGBA, GL_UNSIGNED_BYTE, image.getImageData());

 		}

 		glutSwapBuffers();
 	}

  //std::cout<<"The window height is:"<<glutGet(GLUT_WINDOW_HEIGHT)<<std::endl;
  //std::cout<<"The window width is:"<<glutGet(GLUT_WINDOW_WIDTH)<<std::endl;

  // flush the OpenGL pipeline to the viewport
 	glFlush();
 }


 
/*********************************************************
 *  This is the keyboard callback method that responds to  
 *  keypress events detected by OpenGL on any of its windows
 *  w or W writes the image if an output filename is specified
 *  q or Q or ESC exits the program
 *  Input: GLUT - key pressed and coordinates x,y
 *  Return: -
 *********************************************************/
 void handleKey(unsigned char key, int x, int y){

 	switch(key){
    case 'w':		// 'w' - save the image
    case 'W':
    std::cout<<"Keypress detected...\n";
    if(image.getWriteFlag()==1) {
    	std::cout<<"The image will be written into "<<image.getOFileName()<<std::endl;
    	if(image.writeImage()==0) {
    		std::cout<<std::endl<<image.getOFileName()<<" Succesfully written"<<std::endl;
    	}

    } else {
    	std::cout<<"No output filename specified"<<std::endl;
    }


    break;

    case 'o':
    case 'O': if(image.readImage(filenames[curIndex], image.getWriteFlag())==0) {

 				WIDTH = image.getWidth(); 
 				HEIGHT = image.getHeight();
 				glutReshapeWindow(WIDTH,HEIGHT);
 			}
 			glutPostRedisplay();
 			break;


 	case 'R':
    case 'r': image.changeColor(1);    break;

    case 'G':
    case 'g': image.changeColor(2);    break;

    case 'B':
    case 'b': image.changeColor(3);    break;

    case 'I':
    case 'i': image.changeColor(4); break;

    case 'q':		// q - quit
    case 'Q':
    case 27:		// esc - quit
	    image.deleteImage();
	    exit(0);
    //return;

    default:		// not a valid key -- just ignore it
    return;
}
}



/*********************************************************
 *  This is the mouse callback method that responds to  
 *  mouse click event detected by OpenGL on any of its windows
 *  clicking the left mouse button prompts the writing of the
 *  rgba values read from the image being displayed
 *  Input: GLUT mouse event - button pressed, value and x,y coordinates
 *  Return: -
 *********************************************************/
 void handleMouse(int btn, int val, int x, int y) {

 	if (btn == GLUT_LEFT_BUTTON)
 	{

 		unsigned char pixels[1 * 1 * 4];
 		glReadPixels(x,glutGet( GLUT_WINDOW_HEIGHT ) - y, 1 , 1, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

 		std::cout << "x,y( "<<abs(x)<<" , "<<abs(y)<<") "<<"rgba( " << (int)pixels[0]<<" , "<<(int)pixels[1]<<" , "<<(int)pixels[2]<<" , "<<(int)pixels[3]<<" )";
 		std::cout << std::endl;
 	}

 }


/*********************************************************
 *  This is an additional keyboard callback method that responds to  
 *  event triggered by pressing the left or right keys
 *  Input: GLUT event - key pressed, x, y coordinates
 *  Return: -
 *  
 *********************************************************/
 void handleArrowKeys(int key, int x, int y) {

 	if(filenames.size() == 1) {
 		std::cout<<std::endl<<"Specify more filenames to navigate"<<std::endl;
 		return;
 	} 

 	switch(key) {
 		case GLUT_KEY_LEFT :
 		std::cout<<"Left Keypressed"<<std::endl;
 		if(curIndex > 0){
 			curIndex--;
 		} else if(curIndex == 0) {
				//Already at the first picture, so do we rotate?
				//yes
 			curIndex = filenames.size()-1;
				//no
				//do nothing
				//return;
 		}
 		break;

 		case GLUT_KEY_RIGHT :
 		std::cout<<"Right Keypressed"<<std::endl;

 		if(curIndex < filenames.size()-1){
 			curIndex++;
 		} else if(curIndex == filenames.size()-1) {
				//Already at the first picture, so do we rotate?
				//yes
 			curIndex = 0;
				//no
				//do nothing
				//return;
 		}
 		break;

 	}
 	std::cout<<"Opening file "<<filenames[curIndex]<<std::endl;
 	if(!file_exists(filenames[curIndex])) {
 		std::cout<<std::endl<<"The input file specified "<<filenames[curIndex]<<" does not exist."<<std::endl;
 		return;
 	}
 	if(image.readImage(filenames[curIndex], image.getWriteFlag())==0) {

 		WIDTH = image.getWidth(); 
 		HEIGHT = image.getHeight();
 		glutReshapeWindow(WIDTH,HEIGHT);
 	}
 	glutPostRedisplay();
 }


/*********************************************************
 *  The main method
 *  Accepts 1, 2 or more filenames as argument (2 or more
 *  would mean last filename is the output file)
 *  Return - int (success or failure)
 *********************************************************/

 int main(int argc, char** argv) {


 	int writeFlag = 0;
 	int rotateFlag = 0;


 	if(argc==1) {
 		std::cout<< "Atleast one parameter is expected. Usage: ./oiioview filename.extension"<<std::endl;
 		return -1;
 	} else if(argc==2) {
 		writeFlag = 0;
		 filenames.push_back(argv[1]);

 	} else {
 		writeFlag = 1;
 		rotateFlag = 1;
 		std::cout<<"You have entered "<<argc-1<<" arguments. "<<argv[argc-1]<<" will be used as filename for the output file generated when you press w/W during Image Display."<<std::endl;
 	}


	//Get all the input filenames

	for(int c = 1; c < argc-1; c++) { 
 		filenames.push_back(argv[c]);
 	}


 	std::string input_file = filenames[curIndex];
 	std::string output_file = "";

 	if(writeFlag) {
 		output_file = argv[argc-1];
 		image.setOFileName(output_file);
 		image.setWriteFlag(writeFlag);
 	}


	//Check if input file exists
 	if(!file_exists(input_file)) {
 		std::cout<<std::endl<<"The input file specified "<<input_file<<" does not exist."<<std::endl;
 		return -1;
 	}



	//Open the image and read its specs
 	if(image.readImage(input_file, writeFlag)==0) {

 		WIDTH = image.getWidth(); 
 		HEIGHT = image.getHeight();


 		glutInit(&argc, argv);

		// create the graphics window, giving width, height, and title text
 		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
 		glutInitWindowSize(WIDTH, HEIGHT);
 		glutCreateWindow(input_file.data());

		// set up the callback routines to be called when glutMainLoop() detects
		// an event
		glutDisplayFunc(displayImage);	  // display callback
		glutKeyboardFunc(handleKey);	  // keyboard callback
		glutMouseFunc(handleMouse); //Mouse callback
		glutSpecialFunc(handleArrowKeys); //for left,right arrow keys
		//glutReshapeFunc(handleReshape);

		// define the drawing coordinate system on the viewport
		// lower left is (0, 0), upper right is (WIDTH, HEIGHT)
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, WIDTH, 0, HEIGHT);

		// specify window clear (background) color to be opaque white
		glClearColor(1, 1, 1, 1);

		// Routine that loops forever looking for events. It calls the registered 
		// callback routine to handle each event that is detected
		glutMainLoop();
	}
	return 0;
}




