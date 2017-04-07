#include "tinyxml2.h"
#include <stdio.h>
#include <stdlib.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include<sstream>
#include<fstream>


using namespace cv;
using namespace std;
using namespace tinyxml2;

float id,occupied,x, y, w, h, d,pt=1,nt=1,wd,ht;
Mat M, rot, cropped,img;
String str1;
string str;
String posimg;
String negimg;
String posfimg;
String negfimg;
String xml;

int main() {
	ofstream posfile;
	ofstream negfile;

	posfile.open("annotation.txt", ios_base::app);
	negfile.open("negative.txt", ios_base::app);

	posimg = "C:\\Users\\Raj Shah\\Documents\\Visual Studio 2015\\Projects\\Demo\\Demo\\Posimg\\";
	negimg = "C:\\Users\\Raj Shah\\Documents\\Visual Studio 2015\\Projects\\Demo\\Demo\\negimg\\";

	posfimg = "E:\\Python3.6\\cv-hw2\\Posimg\\";
	negfimg = "E:\\Python3.6\\cv-hw2\\negimg\\";

	str = "2013-02-26_14_59_35";
	str1 = "C:\\Users\\Raj Shah\\Documents\\Visual Studio 2015\\Projects\\Demo\\Demo\\PKLot\\parking1b\\cloudy\\2013-02-26\\" + str;
	img = imread(str1+".jpg");
	xml = str1 + ".xml";
	const char* xmlname = xml.c_str();
	XMLDocument doc;
	XMLError eres = doc.LoadFile(xmlname);
	//XMLError eres = doc.LoadFile(argv[1]);
	XMLNode* root = doc.FirstChildElement("parking");
	XMLElement* space = root->FirstChildElement("space");
	
	for (XMLElement* space = root->FirstChildElement("space"); space!=NULL; space = space->NextSiblingElement())
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

		//cout << x << "," << y << "," << w << "," << h << "," << d << endl;
		
		RotatedRect rRect = RotatedRect(Point2f(x, y), Size2f(w,h),d);

		/*Point2f vertices[4];
		rRect.points(vertices);
		for (int i = 0; i < 4; i++)
			line(img, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0));
			*/

		Rect brect = rRect.boundingRect();
		//rectangle(img, brect, Scalar(255, 0, 0));

		
		imshow("rectangles", img);
		getRectSubPix(img, Size(brect.width, brect.height), Point(x,y), cropped);
		//getRectSubPix(img, Size(w,h), Point(x,y), cropped);
		imshow("Cropped", cropped);
		
		
		space->QueryFloatAttribute("occupied", &occupied);
		if (occupied == 1) {
			stringstream ss;
			stringstream ps;
			string name = "pos_"+str+"_";
			string type = ".jpg";
			
			ss <<posimg<< name << (pt) << type;
			string posname = ss.str();
			ss.str("");
			
			ps << posfimg << name << (pt) << type;
			string posfname = ps.str();
			ps.str("");

			cout << posname << endl;
			imshow("cropped", cropped);
			imwrite(posname, cropped);
			pt += 1;
			posfile << posfname+" 1 0 0 ";
			posfile << brect.width;
			posfile << " ";
			posfile << brect.height;
			posfile << "\n";
			//cin.ignore();

		}
		if (occupied == 0) {
			stringstream ss;
			stringstream ns;
			string name = "neg_"+str+"_";
			string type = ".jpg";
			
			ss << negimg << name << (nt) << type;
			string negname = ss.str();
			ss.str("");
			
			ns << negfimg << name << (nt) << type;
			string negfname = ns.str();
			ns.str("");
			
			cout << negname << endl;
			imshow("cropped", cropped);
			imwrite(negname, cropped);
			nt += 1;
			negfile << negfname;
			negfile << "\n";
		}

}
	posfile.close();
	negfile.close();	
}


