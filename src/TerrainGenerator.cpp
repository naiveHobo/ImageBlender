/*
 * FinalProject.cpp
 *
 *  Created on: 20-Jan-2017
 *      Author: naivehobo
 */
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <numeric>
#include <stdio.h>
#include <dirent.h>
#include "blend.h"
#include "score.h"
// #include "classify.h"

using namespace cv;
using namespace std;

Mat wood;
Mat mud;
Mat grass;
Mat conc;

vector<Mat> grassTest;
vector<Mat> mudTest;
vector<Mat> woodTest;
vector<Mat> concTest;
vector<char> charTerrain;
vector<Mat> finalTerrain;
// vector<float> predictLabels;
// vector<float> trueLabels;
// vector<float> S1scores;
vector<float> S2scores;
vector<float> S3scores;


void loadInputImages(char arg[]){
	string inputDirectory(arg);
	DIR *directory = opendir(inputDirectory.c_str());
	struct dirent *_dirent = NULL;
	if(directory == NULL){
		printf("Cannot open Input Folder\n");
		return;
	}
	while((_dirent = readdir(directory)) != NULL){
		string fileName = inputDirectory + "/" + string(_dirent->d_name);
		Mat rawImage = imread(fileName.c_str());
		if(rawImage.data == NULL){
			if(_dirent->d_name[0] == '.')
				continue;
			printf("Cannot Open Image\n");
		    cout<<fileName;
		    continue;
		}
		else
			cv::resize(rawImage, rawImage, cv::Size(81, 81));
		if(_dirent->d_name[0]=='g')
			grassTest.push_back(rawImage);
		else if(_dirent->d_name[0]=='m')
			mudTest.push_back(rawImage);
		else if(_dirent->d_name[0]=='w')
			woodTest.push_back(rawImage);
		else if(_dirent->d_name[0]=='c')
			concTest.push_back(rawImage);
	}
	closedir(directory);
}

void getInput(int I){
	int r[4], s[4] = {grassTest.size(), woodTest.size(), mudTest.size(), concTest.size()};
	for(int i=0;i<4;i++){
		srand (time(NULL));
		r[i] = rand()%s[i];
	}
	namedWindow("Input");
	grass = grassTest[r[0]];
	imshow("Input", grass);
	waitKey(0);
	wood = woodTest[r[1]];
	imshow("Input", wood);
	waitKey(0);
	mud = mudTest[r[2]];
	imshow("Input", mud);
	waitKey(0);
	conc = concTest[r[3]];
	imshow("Input", conc);
	waitKey(0);
	destroyWindow("Input");
	cout<<"Test case "<<I+1<<": ";
	char ch;
	for(int i=0;i<9;i++){
		cin>>ch;
		charTerrain.push_back(ch);
	}
}

vector<Mat> getPatches(vector<Mat> terrain){
	int height = terrain[0].rows;
	int width = terrain[0].cols;
	Size patch(81, 81);
	vector<Mat> patches;
	for(int i=0;i<terrain.size();i++){
		for(int y=0; y < height; y += patch.height){
			for(int x=0; x < width; x += patch.width){
				Rect rect = Rect(x, y, patch.width, patch.height);
				patches.push_back(Mat(terrain[i], rect));
			}
		}
	}
	return patches;
}

Mat unprocTerrain(){
	int newWidth, newHeight, height, width;
	height = 81;
	width = 81;
	newWidth = 243;
	newHeight = 243;
	Mat terrain(newHeight, newWidth, CV_8UC3, Scalar(0, 0, 0));
	int left_top_x = 0;
	int left_top_y = 0;
	for(int i=0;left_top_y < newHeight;left_top_x += (width)){
		if(left_top_x >= newWidth){
			left_top_x = -width;
	        left_top_y += height;
	        continue;
	    }
		if(charTerrain[i]=='g'){
			Rect roi(left_top_x,left_top_y, grass.cols, grass.rows);
			grass.copyTo(terrain(roi));
		}
		else if(charTerrain[i]=='m'){
			Rect roi(left_top_x,left_top_y, mud.cols, mud.rows);
			mud.copyTo(terrain(roi));
		}
		else if(charTerrain[i]=='w'){
			Rect roi(left_top_x,left_top_y, wood.cols, wood.rows);
			wood.copyTo(terrain(roi));
		}
		else if(charTerrain[i]=='c'){
			Rect roi(left_top_x,left_top_y, conc.cols, conc.rows);
			conc.copyTo(terrain(roi));
		}
		i++;
	}
	return terrain;
}

// void predict(vector<Mat> imageTest){
// 	CvSVM svm;
// 	svm.load("/home/naivehobo/Desktop/svm.xml");

// 	int i=0;
// 	int cnt=0;
// 	int j = 0;
// 	int tc[4] = {0}, sc[4] = {0};

// 	for(i=0;i<imageTest.size();i++){
// 		imageTest[i].convertTo(imageTest[i],CV_32FC1, 1.0/255.0);
// 		float c = svm.predict(imageTest[i]);
// 		float test;
// 		if(charTerrain[j]=='g'){
// 			test = 1.0;
// 			sc[(int)test-1]++;
// 		}
// 		else if(charTerrain[j]=='m'){
// 			test = 2.0;
// 			sc[(int)test-1]++;
// 		}
// 		else if(charTerrain[j]=='w'){
// 			test = 3.0;
// 			sc[(int)test-1]++;
// 		}
// 		else if(charTerrain[j]=='c'){
// 			test = 4.0;
// 			sc[(int)test-1]++;
// 		}
// 		trueLabels.push_back(test);
// 		predictLabels.push_back(c);
// 		if(test==c){
// 			cnt++;
// 			tc[(int)c-1]++;
// 		}
// 		if(j==8)
// 			j=0;
// 		else
// 			j++;
// 	}
// 	/*
// 	cout<<"Accuracy: "<<(cnt*100.0)/imageTest.size()<<'%';
// 	cout<<"\nGrass Accuracy: "<<(tc[0]*100.0)/sc[0]<<'%';
// 	cout<<"\nMud Accuracy: "<<(tc[1]*100.0)/sc[1]<<'%';
// 	cout<<"\nWood Accuracy: "<<(tc[2]*100.0)/sc[2]<<'%';
// 	cout<<"\nConcrete Accuracy: "<<(tc[3]*100.0)/sc[3]<<'%';
// 	*/
// }

void processTerrain(Mat terrain){
	finalTerrain.push_back(featherBlend1(terrain));
	finalTerrain.push_back(featherBlend2(terrain));
	finalTerrain.push_back(alphaBlend1(terrain));
	finalTerrain.push_back(alphaBlend2(terrain));
	finalTerrain.push_back(LaplacianBlend(terrain));
	finalTerrain.push_back(alphaBlend1(finalTerrain[1]));
	finalTerrain.push_back(alphaBlend1(finalTerrain[3]));
	finalTerrain.push_back(alphaBlend1(finalTerrain[4]));
	finalTerrain.push_back(alphaBlend2(finalTerrain[0]));
	finalTerrain.push_back(alphaBlend2(finalTerrain[4]));
	finalTerrain.push_back(featherBlend1(finalTerrain[4]));
	Mat u = multibandBlend(terrain);
	finalTerrain.push_back(u);
	finalTerrain.push_back(alphaBlend1(u));
	finalTerrain.push_back(alphaBlend2(u));
	finalTerrain.push_back(featherBlend1(finalTerrain[finalTerrain.size()-1]));
	finalTerrain.push_back(featherBlend2(finalTerrain[finalTerrain.size()-1]));
	finalTerrain.push_back(featherBlend1(u));
	finalTerrain.push_back(featherBlend2(u));
}


int main(int argc, char *argv[]){
	if(argc==2)
		loadInputImages(argv[1]);
	else{
		char s[] = "/home/naivehobo/Desktop/training";
		loadInputImages(s);
	}
	for(int I=0;I<10;I++){
		Mat uTerr;
		getInput(I);
		uTerr = unprocTerrain();
		namedWindow("Unprocessed Terrain");
		imshow("Unprocessed Terrain", uTerr);
		waitKey(0);
		processTerrain(uTerr);
		namedWindow("Processed Terrain");
		for(int i=0;i<finalTerrain.size();i++){
			imshow("Processed Terrain", finalTerrain[i]);
			waitKey(0);
		}
		destroyWindow("Unprocessed Terrain");
		destroyWindow("Processed Terrain");
		vector<Mat> patches = getPatches(finalTerrain);
		vector<Mat> imTest;
		for(int i=0;i<patches.size();i++){
			Mat rawImage = patches[i].clone();
			imTest.push_back(rawImage.reshape(1, rawImage.rows*rawImage.cols*rawImage.channels()).t());
		}

		// predict(imTest);
		// S1scores.push_back(S1_score(predictLabels, trueLabels));
		S2scores.push_back(S2_score(finalTerrain, uTerr));
		S3scores.push_back(S3_score(finalTerrain, uTerr));
		// cout<<"S1 Score: "<<S1scores[I];
		cout<<"\nS2 Score: "<<S2scores[I];
		cout<<"\nS3 Score: "<<S3scores[I]<<endl;

		// predictLabels.clear();
		// trueLabels.clear();
		charTerrain.clear();
		finalTerrain.clear();
	}
	cout<<"\nTotal Score: "<<totalScore(S2scores, S3scores);
	return 0;
}
