/*
 * score.h
 *
 *  Created on: 01-Feb-2017
 *      Author: naivehobo
 */

#ifndef SRC_SCORE_H_
#define SRC_SCORE_H_

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;
using namespace cv;

float S1_score(vector<float> predictLabel, vector<float> trueLabel){
	int confusionMatrix[4][4] = {{0}, {0}, {0}, {0}};
	int weightAvg[4] = {0}, wtAvg;
	float precision = 0, recall = 0, score;
	for(unsigned int i=0;i<trueLabel.size();i++){
		int p = (int)predictLabel[i] - 1;
		int t = (int)trueLabel[i] - 1;
		weightAvg[t] = 1;
		if(predictLabel[i] == trueLabel[i])
			confusionMatrix[p][p]++;
		else
			confusionMatrix[p][t]++;
	}
	for(int i=0;i<4;i++){
		int predTot = 0, trueTot = 0;
		float p, r;
		for(int j=0;j<4;j++){
			predTot += confusionMatrix[i][j];
			trueTot += confusionMatrix[j][i];
		}
		if(!(predTot==0 || trueTot==0)){
			p = (float)confusionMatrix[i][i]/predTot;
			r = (float)confusionMatrix[i][i]/trueTot;
			precision += p;
			recall += r;
		}
	}
	wtAvg = weightAvg[0]+weightAvg[1]+weightAvg[2]+weightAvg[3];
	precision = precision/wtAvg;
	recall = recall/wtAvg;
	score = (2*recall*precision)/(precision+recall);
	return score;
}

float S2_score(vector<Mat> terrains, Mat uTerr){
	Mat u(terrains[0].size(), terrains[0].type());
	int rows = terrains[0].rows;
	int cols = terrains[0].cols;
	float score[3] = {0};
	for(int y=0;y<cols;y++){
		for(int x=0;x<rows;x++){
			vector<Vec3b> pix;
			float mean[3] = {0};
			float var[3] = {0};
			float std[3];
			float cov[3];
			for(int i=0;i<terrains.size();i++){
				pix.push_back(terrains[i].at<cv::Vec3b>(x, y));
				for(int j=0;j<3;j++)
					mean[j] += terrains[i].at<Vec3b>(x, y)[j];
			}
			for(int j=0;j<3;j++)
				mean[j] = mean[j]/terrains.size();
			for(int i=0;i<pix.size();i++){
				for(int j=0;j<3;j++)
					var[j] += ((pix[i][j] - mean[j])*(pix[i][j] - mean[j]));
			}
			for(int j=0;j<3;j++){
				var[j] = var[j]/terrains.size();
				std[j] = sqrt(var[j]);
				cov[j] = (std[j]/mean[j]);
				if(cov[j]<10000)
					score[j] += cov[j];
			}
			Vec3b p;
			for(int j=0;j<3;j++)
				p[j] = cov[j]*100;
			u.at<Vec3b>(x, y) = p;
		}
	}
	Mat c;
	cvtColor(uTerr, c, CV_BGR2GRAY);
	cvtColor(u, u, CV_BGR2GRAY);
	namedWindow("Unprocessed Grayscale");
	imshow("Unprocessed Grayscale", c);
	waitKey(0);
	namedWindow("Processed Grayscale");
	imshow("Processed Grayscale", u);
	waitKey(0);
	destroyWindow("Unprocessed Grayscale");
	destroyWindow("Processed Grayscale");
	for(int i=0;i<3;i++)
		score[i] = score[i]/(rows*cols);
	return ((score[0]+score[1]+score[2])/3);
}

float S3_score(vector<Mat> terrains, Mat uTerr){
	float score = 0;
	Mat grid = imread("export2.png");
	Mat u = uTerr.clone();
	grid.convertTo(grid,CV_32FC3,1.0/255.0);
	u.convertTo(u, CV_32FC3, 1/255.0);
	u = u.mul(grid);
	for(unsigned int i=0;i<terrains.size();i++){
		Mat t = terrains[i].clone();
		t.convertTo(t, CV_32FC3, 1/255.0);
		t = t.mul(grid);
		Mat s = abs(u - t);
		Scalar m = mean(s);
		float mean = (m[0]+m[1]+m[2])/3;
		score += mean;
	}
	score = score/terrains.size();
	return score;
}

float totalScore(vector<float> S1, vector<float> S2, vector<float> S3){
	float s1 = 0, s2 = 0, s3 = 0;
	for(int i=0;i<S1.size();i++){
		s1 += S1[i];
		s2 += S2[i];
		s3 += S3[i];
	}
	s1 = s1/S1.size();
	s2 = s2/S2.size();
	s3 = s3/S3.size();
	return (s1+s2+s3)/3;
}


#endif /* SRC_SCORE_H_ */
