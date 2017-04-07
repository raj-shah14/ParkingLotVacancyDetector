#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "tinyxml2.h"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
using namespace tinyxml2;


float id, occupied, w, h, d, x, y,dst;
float spot = 0;
float act = 0;
float falpos = 0;
float truneg = 0;
CascadeClassifier car_cascade;

string window_name = "Capture - Car detection";
Mat frame;
String filename;
String xml;
int minNei, lot, tot_spot,thresh,mx,ftype,temp=0,mi;

/** @function main */
int main()
{
	cout << "Which Parking lot is to be selected: 1-->Parkinglot 1a , 2-->Parkinglot 1b , 3-->Parkinglot 2" << endl;
	cin >> lot;
	cout << "Select the Feature Type: 1-->HAAR , 2-->LBP" << endl;
	cin >> ftype;

	filename = "C:\\Users\\Raj Shah\\Documents\\Visual Studio 2015\\Projects\\Demo\\Demo\\PKLot\\parking2\\cloudy\\2012-09-28\\2012-09-28_12_26_15";
	xml = filename + ".xml";
	const char* xmlname = xml.c_str();


	//HAAR
	if (lot == 1 && ftype==1) {
		if (!car_cascade.load("cascadeH10P1n2.xml")) { printf("--(!)Error loading\n"); return -1; };
		mx = 60;
		mi = 45;
		tot_spot = 28;
		thresh = 50;
		minNei = 20;
	}
	if (lot == 2 && ftype == 1) {
		if (!car_cascade.load("cascadeH10P1n2.xml")) { printf("--(!)Error loading\n"); return -1; };
		mx = 60;
		mi = 45;
		tot_spot = 40;
		thresh = 40;
		minNei = 15;
	}
	if (lot == 3 && ftype == 1) {
		if (!car_cascade.load("cascadeH10P1n2.xml")) { printf("--(!)Error loading\n"); return -1; }
		mx = 35;
		mi = 30;
		tot_spot = 100;
		thresh = 25;
		minNei =12;
	}

	//LBP
	if (lot == 1 && ftype == 2) {
		if (!car_cascade.load("cascadeL17P1n2.xml")) { printf("--(!)Error loading\n"); return -1; };
		mx = 60;
		mi = 30;
		tot_spot = 28;
		thresh = 50;
		minNei = 3;
	}
	if (lot == 2 && ftype == 2) {
		if (!car_cascade.load("cascadeL20P1b.xml")) { printf("--(!)Error loading\n"); return -1; };
		mx = 60;
		mi = 30;
		tot_spot = 40;
		thresh = 40;
		minNei = 3;
	}
	if (lot == 3 && ftype == 2) {
		if (!car_cascade.load("cascadeL17P1n2.xml")) { printf("--(!)Error loading\n"); return -1; }
		mx = 60;
		mi = 30;
		tot_spot = 100;
		thresh = 25;
		minNei = 3;
	}

	
	frame = imread(filename+".jpg");
	XMLDocument doc;
	XMLError eres = doc.LoadFile(xmlname);
	XMLNode* root = doc.FirstChildElement("parking");
	XMLElement* space = root->FirstChildElement("space");
	RotatedRect rRect;
	//Rect brect;
	
	vector<Rect> cars;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect Cars
	car_cascade.detectMultiScale(frame_gray, cars, 1.1,minNei, 0, Size(mi,mi), Size(mx, mx));

	for (XMLElement* space = root->FirstChildElement("space"); space != NULL; space = space->NextSiblingElement())
	{
		XMLElement* rotated = space->FirstChildElement("rotatedRect");
		XMLElement* center = rotated->FirstChildElement("center");
		XMLElement* size = rotated->FirstChildElement("size");
		XMLElement* angle = rotated->FirstChildElement("angle");

		center->QueryFloatAttribute("x", &x);
		center->QueryFloatAttribute("y", &y);
		size->QueryFloatAttribute("w", &w);
		size->QueryFloatAttribute("h", &h);
		angle->QueryFloatAttribute("d", &d);

		space->QueryFloatAttribute("occupied", &occupied);
		if (occupied == 1) {
			//cout << x << "," << y << "," << w << "," << h << endl;
			rRect = RotatedRect(Point2f(x, y), Size2f(w, h), d);
			circle(frame, Point2f(x, y), 1, Scalar(0, 0, 255), 1, 8, 0);
			space->QueryFloatAttribute("id", &id);
			act += 1;
		}
			//cout << id << endl;
		temp = 0;
			for (size_t i = 0; i < cars.size(); i++)
			{
				//rectangle(frame, Point(cars[i].x,cars[i].y), Point(cars[i].x+cars[i].width,cars[i].y+cars[i].height),Scalar(255,0,0),1,8,0 );
				rectangle(frame, cars[i], Scalar(255, 0, 0), 1, 8, 0);
				circle(frame, Point(cars[i].x + (cars[i].width*0.5), cars[i].y + (cars[i].height*0.5)), 1, Scalar(0, 255, 0), 1, 8, 0);
				//cout << cars[i] << endl;
				//dst = sqrt(abs((((brect.x + (brect.width*0.5)) - (cars[i].x + (cars[i].width*0.5)))*((brect.x + (brect.width*0.5)) - (cars[i].x + (cars[i].width*0.5)))) + (((brect.y + (brect.height*0.5)) - (cars[i].y + (cars[i].height*0.5)))*((brect.y + (brect.height*0.5)) - (cars[i].y + (cars[i].height*0.5))))));
				dst = sqrt(abs((x - (cars[i].x + (cars[i].width*0.5)))*(x - (cars[i].x + (cars[i].width*0.5))) + ((y - (cars[i].y + (cars[i].height*0.5)))*(y - (cars[i].y + (cars[i].height*0.5))))));
				//cout << dst << endl;
				if (dst <= thresh) {
					if (temp != id) {
						temp = id;
						//cout << "Parking ID Occupied:" << id << endl;
						space->QueryFloatAttribute("occupied", &occupied);
						if (occupied == 1) spot += 1;
						else falpos += 1;
					}
					break;
				}
			}
		//}
	}

	//-- Show what you got
	imshow(window_name, frame);
	cout << "Actual Parked spots:" << act << endl;
	cout <<"Detected Parked spots:"<< spot << endl;
	cout << "False Positives:" << falpos << endl;
	truneg = tot_spot - act - falpos;
	float acc = ((spot + truneg) / (tot_spot) * 100);
	cout << "Accuracy:" << acc <<"%"<< endl;
	int drem_spots = tot_spot - spot;
	int arem_spots = tot_spot - act;
	cout << "Actual remaining Parking Spots:" << arem_spots << endl;
	cout << "Detected remaining Parking Spots:" << drem_spots << endl;
	waitKey(0);
	return 0;
}