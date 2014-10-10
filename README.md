computer_graphics_images
========================

Image Processing with OpenImageIO and display call backs using GLUT/OpenGL

This project contains my experiments with image processing using C++. I am using OpenImageIO to handle the image read and write operations.

You will find the following content:

	1) oiioview
	-=-=-=-=-=-=-

	* Usage: 

		$ oiioview filename1.img [filename2.img] [filename3.img] ... [outputfilename.img]

	* Declares a basic image viewer that can handle any number of 3/4 channel images within formats that OpenImageIO supports - and any number provided on the command line.
	* The program also allows you to write out the image if a filename is provided and you press w or W on your keyboard
	* If you press 'r', 'g' or 'b', you can view individual color channels of the image. 
	* If you press 'i', the image is inverted. 
	* You can navigate through the images by using arrow keys.
	
