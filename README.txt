============================
Computer Vision Assignment 2
============================
HOW TO RUN?
1)Open Source.cpp, which is the main file for detection.Add the tinyxml2.cpp and tinyxml2.h to the project as source and header files.
2)On line 34 of Source.cpp give the filename without extension.
  To ease this process there is a text document with name TEST_IMAGES_HAAR and TEST_IMAGES_LBP from which you can copy the filename and before that append it with the location where
  PKLot directory is stored on your PC.
3)Build and Run the program
4)Select the SAME PARKING LOT as given on the line 34 of Source.cpp
5) 1--> Parking lot 1a, 2-->Parking lot 1b, 3-->Parking lot 2
6)Select the Feature Type
  1-->HAAR, 2-->LBP
7)You can see the results for true positives,False Positives, Accuracy, Empty Parking Spots on the Command Window.

==========================================================================================================================================

The text files HAARTrainingDetails.txt and LBPTrainingDetails.txt give the values of N HR FA after training for given number of stages.
The folder HAAR-P1n2 consists of vector file and posiive and negative Samples.
This samples were created using Parsing.cpp file present in the folder.