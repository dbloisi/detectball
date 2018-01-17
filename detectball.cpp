/*
*  detectball
*
*  Detect a black and white ball in images
*  Copyright 2017 Domenico Daniele Bloisi
*
*  This file is part of the tutorial
*  "How to Use OpenCV for Ball Detection" by Domenico D. Bloisi
*  http://profs.scienze.univr.it/~bloisi/tutorial/balldetection.html
*  and it is distributed under the terms of the
*  GNU Lesser General Public License (Lesser GPL)
*
*  detectball is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  detectball is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with createpos.  If not, see <http://www.gnu.org/licenses/>.
*
*  This file contains a C++ OpenCV based implementation for
*  detecting a black and white ball in images as explained
*  in "How to Use OpenCV for Ball Detection" by Domenico D. Bloisi
*  http://profs.scienze.univr.it/~bloisi/tutorial/balldetection.html
*
*  Please, cite the above web page if you use createpos.
*
*  detectball has been written by Domenico D. Bloisi
*
*  Please, report suggestions/comments/bugs to
*  domenico.bloisi@gmail.com
*
*/

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#if defined(_MSC_VER)
#include <tchar.h>
#include <strsafe.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#elif defined(__GNUC__) || defined(__GNUG__)
#include <dirent.h>
#endif

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( string& img_filename );

/** Global variables */
String cascade_name = "ball_cascade.xml";
CascadeClassifier cascade;

string window_name = "SPQR TEAM - ball detection with LBP";

int main( int argc, const char** argv )
{
	string dirname;
	cout << "SPQR TEAM - ball detection with LBP" << endl;
	cout << endl;	

	if (argc == 2) {
		dirname.assign(argv[1]);
	}
	else {
		cout << "Usage is:" << endl;
		cout << argv[0] << "<dir name>" << endl;
		cout << endl;
		return EXIT_FAILURE;
	}
	
	//-- 1. Load the cascade
	cout << "Loading the cascade " << cascade_name << "...";
	cout.flush();
	if (!cascade.load(cascade_name)) {
		cout << endl;
		cout << "--(!)Error loading CASCADE: " << cascade_name << endl;
		return EXIT_FAILURE;
	}
	else {
		cout << "[OK]" << endl;
	}

#if defined(_MSC_VER)

	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError = 0;

	// Check that the input path plus 3 is not longer than MAX_PATH.
	// Three characters are for the "\*" plus NULL appended below.

	StringCchLength(dirname.c_str(), MAX_PATH, &length_of_arg);

	if (length_of_arg > (MAX_PATH - 3))
	{
		_tprintf(TEXT("\nDirectory path is too long.\n"));
		exit(EXIT_FAILURE);
	}

	_tprintf(TEXT("\nTarget directory is %s\n\n"), dirname.c_str());

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopy(szDir, MAX_PATH, dirname.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	// Find the first file in the directory.

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		exit(EXIT_FAILURE);
	}

	// List all the files in the directory with some info about them.
	string img_filename;
	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			continue;
		}
		else
		{
			PTSTR pszFileName = ffd.cFileName;
			std::string name(pszFileName);
			img_filename = dirname + "\\" + name;
			detectAndDisplay(img_filename);
		}
	} while (FindNextFile(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES)
	{
		exit(EXIT_FAILURE);
	}

	FindClose(hFind);

#elif defined(__GNUC__) || defined(__GNUG__)

	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(dirname.c_str())) != NULL) {
		string img_filename;
		while ((ent = readdir(dir)) != NULL) {			
			img_filename = dirname + "/" + ent->d_name;
			detectAndDisplay(img_filename);
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		perror("");
		return EXIT_FAILURE;
	}

#endif
	return EXIT_SUCCESS;
}


/**
* @function detectAndDisplay
*/
void detectAndDisplay(string& img_filename)
{
	Mat frame;
	frame = imread(img_filename, CV_LOAD_IMAGE_COLOR);
	if (!frame.data) {
		cout << "Unable to read input frame: " << img_filename << endl;
		exit(EXIT_FAILURE);
	}
    
	Mat resized_frame(Size(320, 240), CV_8UC3);
	resize(frame, resized_frame, resized_frame.size());
	frame = resized_frame.clone();

    std::vector<Rect> balls;
    Mat frame_gray;

    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    cascade.detectMultiScale(frame_gray, balls, 1.1, 5, 8, Size(16, 16));

   Mat gui_frame = frame.clone();
   for( unsigned int i = 0; i < balls.size(); i++ )
   {
      Point center( balls[i].x + cvRound(balls[i].width*0.5), cvRound(balls[i].y + balls[i].height*0.5) );
      circle(gui_frame, center, cvRound(balls[i].width*0.5), Scalar( 255, 0, 255 ), 2, 8, 0 );
	  //Mat ballROI = frame_gray( balls[i] );
   } 

   //-- Show what you got
   imshow( window_name, gui_frame );
   int key = cvWaitKey(0);
}
