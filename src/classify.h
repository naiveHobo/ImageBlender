/*
 * classify.h
 *
 *  Created on: 03-Feb-2017
 *      Author: naivehobo
 */

#ifndef SRC_CLASSIFY_H_
#define SRC_CLASSIFY_H_

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/ml/ml.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <dirent.h>

using namespace cv;
using namespace std;

void classify(vector<Mat> imageTest, float labels[]){
	ml::SVM svm;
	ml::svm.load("/home/naivehobo/Desktop/svm.xml");

	int i=0;
	int cnt=0;
	int j = 0;
	int tc[4] = {0}, sc[4] = {0};

	for(i=0;i<imageTest.size();i++){
		imageTest[i].convertTo(imageTest[i], CV_32FC1, 1.0/255.0);
		float c = svm.predict(imageTest[i]);
		sc[(int)labels[i]-1]++;
		if(labels[i]==c){
			cnt++;
			tc[(int)c-1]++;
		}
	}
	cout<<"Accuracy: "<<(cnt*100.0)/imageTest.size()<<'%';
	cout<<"\nGrass Accuracy: "<<(tc[0]*100.0)/sc[0]<<'%';
	cout<<"\nMud Accuracy: "<<(tc[1]*100.0)/sc[1]<<'%';
	cout<<"\nWood Accuracy: "<<(tc[2]*100.0)/sc[2]<<'%';
	cout<<"\nConcrete Accuracy: "<<(tc[3]*100.0)/sc[3]<<'%';
}

void trainSVM(Mat images, Mat label){
	CvParamGrid CvParamGrid_C(pow(2.0,-5), pow(2.0,15), pow(2.0,2));
	CvParamGrid CvParamGrid_gamma(pow(2.0,-15), pow(2.0,3), pow(2.0,2));
	if (!CvParamGrid_C.check() || !CvParamGrid_gamma.check())
	    cout<<"The grid is NOT VALID."<<endl;
	CvSVMParams paramz;
	paramz.kernel_type = CvSVM::RBF;
	paramz.svm_type = CvSVM::C_SVC;
	paramz.term_crit = cvTermCriteria(CV_TERMCRIT_ITER,100,0.000001);
	CvSVM svm;
	svm.train_auto(images, label, Mat(), Mat(), paramz,10, CvParamGrid_C, CvParamGrid_gamma, CvSVM::get_default_grid(CvSVM::P), CvSVM::get_default_grid(CvSVM::NU), CvSVM::get_default_grid(CvSVM::COEF), CvSVM::get_default_grid(CvSVM::DEGREE), true);
	paramz = svm.get_params();
	cout<<"gamma:"<<paramz.gamma<<endl;
	cout<<"C:"<<paramz.C<<endl;
	svm.save("/home/naivehobo/Desktop/svm.xml");
}

void getTrainingData(){
	vector<float> label;
	vector<Mat> imageTrain;
	string inputDirectory = "/home/naivehobo/Desktop/training";
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
		imageTrain.push_back(rawImage.reshape(1,rawImage.rows*rawImage.cols*rawImage.channels()).t());
		if(_dirent->d_name[0]=='g')
			label.push_back(1.0);
		else if(_dirent->d_name[0]=='m')
			label.push_back(2.0);
		else if(_dirent->d_name[0]=='w')
			label.push_back(3.0);
		else if(_dirent->d_name[0]=='c')
			label.push_back(4.0);
	}
	closedir(directory);
	int num_files = imageTrain.size();
	int imArea = imageTrain[0].cols*imageTrain[0].rows*imageTrain[0].channels();
	float labels[num_files];
	for(int i=0;i<label.size();i++)
		labels[i] = label[i];
	Mat training_mat(num_files, imArea, CV_32FC1);
	Mat label_mat(num_files, 1, CV_32FC1, labels);
	for(int i=0;i<num_files;i++){
		imageTrain[i].convertTo(imageTrain[i],CV_32FC1, 1.0/255.0);
		imageTrain[i].copyTo(training_mat(Rect(0, i, imArea, 1)));
	}
	trainSVM(training_mat, label_mat);
}

#endif /* SRC_CLASSIFY_H_ */
