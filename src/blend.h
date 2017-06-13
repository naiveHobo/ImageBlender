/*
 * blend.h
 *
 *  Created on: 29-Jan-2017
 *      Author: naivehobo
 */

#ifndef SRC_BLEND_H_
#define SRC_BLEND_H_

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/stitching/detail/blenders.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;
using namespace cv::detail;

Mat alphaBlend1(Mat terr){
	Mat src = terr.clone();
	float alpha;
	int i, j;
	for(i=81;i<163;i+=81){
		Mat seam1 = src(Rect(i-10, 0, 10, 243));
		Mat seam2 = src(Rect(i, 0, 10, 243));
		for(j=0, alpha=0.0;j<10;j++, alpha+=0.1){
			Mat s1 = seam1(Rect(j, 0, 1, 243));
			Mat s2 = seam2(Rect(j, 0, 1, 243));
			Mat seamBlend;
			addWeighted(s1, 1-alpha, s2, alpha, 0.0, seamBlend);
			seamBlend.copyTo(src(Rect(i-5+j, 0, 1, 243)));
		}
	}
	transpose(src, src);
	for(i=81;i<163;i+=81){
		Mat seam1 = src(Rect(i-10, 0, 10, 243));
		Mat seam2 = src(Rect(i, 0, 10, 243));
		for(j=0, alpha=0.0;j<10;j++, alpha+=0.1){
			Mat s1 = seam1(Rect(j, 0, 1, 243));
			Mat s2 = seam2(Rect(j, 0, 1, 243));
			Mat seamBlend;
			addWeighted(s1, 1-alpha, s2, alpha, 0.0, seamBlend);
			seamBlend.copyTo(src(Rect(i-5+j, 0, 1, 243)));
		}
	}
	transpose(src, src);
	return src;
}

Mat alphaBlend2(Mat terr){
	Mat src = terr.clone();
	float alpha;
	int i, j;
	for(i=81;i<163;i+=81){
		Mat seam1 = src(Rect(i-20, 0, 20, 243));
		Mat seam2 = src(Rect(i, 0, 20, 243));
		for(j=0, alpha=0.0;j<20;j+=2, alpha+=0.1){
			Mat s1 = seam1(Rect(j, 0, 2, 243));
			Mat s2 = seam2(Rect(j, 0, 2, 243));
			Mat seamBlend;
			addWeighted(s1, 1-alpha, s2, alpha, 0.0, seamBlend);
			seamBlend.copyTo(src(Rect(i-10+j, 0, 2, 243)));
		}
	}
	transpose(src, src);
	for(i=81;i<163;i+=81){
		Mat seam1 = src(Rect(i-20, 0, 20, 243));
		Mat seam2 = src(Rect(i, 0, 20, 243));
		for(j=0, alpha=0.0;j<20;j+=2, alpha+=0.1){
			Mat s1 = seam1(Rect(j, 0, 2, 243));
			Mat s2 = seam2(Rect(j, 0, 2, 243));
			Mat seamBlend;
			addWeighted(s1, 1-alpha, s2, alpha, 0.0, seamBlend);
			seamBlend.copyTo(src(Rect(i-10+j, 0, 2, 243)));
		}
	}
	transpose(src, src);
	return src;
}

Mat featherBlend1(Mat terr){
	Mat src = terr.clone();
	float alpha;
	int i, j, k;
	for(int c=0;c<2;c++){
		for(i=81;i<165;i+=81){
			Mat seam1 = src(Rect(i-10, 0, 10, 243));
			Mat seam2 = src(Rect(i, 0, 10, 243));
			for(j=0, k=9, alpha=0.0;j<10;j++, k--, alpha+=0.05){
				Mat s1 = seam1(Rect(j, 0, 1, 243));
				Mat s2 = seam2(Rect(k, 0, 1, 243));
				Mat seamBlend;
				addWeighted(s1, 1-alpha, s2, alpha, 0.0, seamBlend);
				seamBlend.copyTo(src(Rect(i-10+j, 0, 1, 243)));
				addWeighted(s1, alpha, s2, 1-alpha, 0.0, seamBlend);
				seamBlend.copyTo(src(Rect(i+k, 0, 1, 243)));
			}
		}
		transpose(src, src);
		for(i=81;i<165;i+=81){
			Mat seam1 = src(Rect(i-10, 0, 10, 243));
			Mat seam2 = src(Rect(i, 0, 10, 243));
			for(j=0, k=9, alpha=0.0;j<10;j++, k--, alpha+=0.05){
				Mat s1 = seam1(Rect(j, 0, 1, 243));
				Mat s2 = seam2(Rect(k, 0, 1, 243));
				Mat seamBlend;
				addWeighted(s1, 1-alpha, s2, alpha, 0.0, seamBlend);
				seamBlend.copyTo(src(Rect(i-10+j, 0, 1, 243)));
				addWeighted(s1, alpha, s2, 1-alpha, 0.0, seamBlend);
				seamBlend.copyTo(src(Rect(i+k, 0, 1, 243)));
			}
		}
		transpose(src, src);
	}
	return src;
}

Mat featherBlend2(Mat terr){
	Mat src = terr.clone();
	float alpha;
	int i, j, k;
	for(int c=0;c<2;c++){
		for(i=81;i<165;i+=81){
			Mat seam1 = src(Rect(i-20, 0, 20, 243));
			Mat seam2 = src(Rect(i, 0, 20, 243));
			for(j=0, k=19, alpha=0.0;j<20;j++, k--, alpha+=0.025){
				Mat s1 = seam1(Rect(j, 0, 1, 243));
				Mat s2 = seam2(Rect(k, 0, 1, 243));
				Mat seamBlend;
				addWeighted(s1, 1-alpha, s2, alpha, 0.0, seamBlend);
				seamBlend.copyTo(src(Rect(i-20+j, 0, 1, 243)));
				addWeighted(s1, alpha, s2, 1-alpha, 0.0, seamBlend);
				seamBlend.copyTo(src(Rect(i+k, 0, 1, 243)));
			}
		}
		transpose(src, src);
		for(i=81;i<165;i+=81){
			Mat seam1 = src(Rect(i-20, 0, 20, 243));
			Mat seam2 = src(Rect(i, 0, 20, 243));
			for(j=0, k=19, alpha=0.0;j<20;j++, k--, alpha+=0.025){
				Mat s1 = seam1(Rect(j, 0, 1, 243));
				Mat s2 = seam2(Rect(k, 0, 1, 243));
				Mat seamBlend;
				addWeighted(s1, 1-alpha, s2, alpha, 0.0, seamBlend);
				seamBlend.copyTo(src(Rect(i-20+j, 0, 1, 243)));
				addWeighted(s1, alpha, s2, 1-alpha, 0.0, seamBlend);
				seamBlend.copyTo(src(Rect(i+k, 0, 1, 243)));
			}
		}
		transpose(src, src);
	}
	return src;
}

Mat multiband(Mat image1, Mat image2){
	Mat image1s, image2s;
	image1.convertTo(image1s, CV_16S);
	image2.convertTo(image2s, CV_16S);
	Mat mask1(image1s.size(), CV_8U);
	mask1(Rect(0, 0, mask1.cols/2, mask1.rows)).setTo(255);
	mask1(Rect(mask1.cols/2, 0, mask1.cols - mask1.cols/2, mask1.rows)).setTo(0);
	Mat mask2(image2s.size(), CV_8U);
	mask2(Rect(0, 0, mask2.cols/2, mask2.rows)).setTo(0);
	mask2(Rect(mask2.cols/2, 0, mask2.cols - mask2.cols/2, mask2.rows)).setTo(255);
	MultiBandBlender blender(false, 5);
	blender.prepare(Rect(0, 0, max(image1s.cols, image2s.cols), max(image1s.rows, image2s.rows)));
	blender.feed(image1s, mask1, Point(0,0));
	blender.feed(image2s, mask2, Point(0,0));
	Mat result_s, result_mask;
	blender.blend(result_s, result_mask);
	Mat result; result_s.convertTo(result, CV_8UC3);
	return result;
}

Mat multibandBlend(Mat terr){
	Mat terrain = terr.clone();
	for(int i=0;i<90;i+=81){
		Mat img1(243, 162, CV_8UC3, Scalar(0, 0, 0));
		Mat img2(243, 162, CV_8UC3, Scalar(0, 0, 0));
		terrain(Rect(i, 0, 81, 243)).copyTo(img1(Rect(0, 0, 81, 243)));
		terrain(Rect(i, 0, 81, 243)).copyTo(img1(Rect(81, 0, 81, 243)));
		terrain(Rect(i+81, 0, 81, 243)).copyTo(img2(Rect(0, 0, 81, 243)));
		terrain(Rect(i+81, 0, 81, 243)).copyTo(img2(Rect(81, 0, 81, 243)));
		Mat blend = multiband(img1, img2);
		blend.copyTo(terrain(Rect(i, 0, 162, 243)));
	}
	transpose(terrain, terrain);
	for(int i=0;i<90;i+=81){
		Mat img1(243, 162, CV_8UC3, Scalar(0, 0, 0));
		Mat img2(243, 162, CV_8UC3, Scalar(0, 0, 0));
		terrain(Rect(i, 0, 81, 243)).copyTo(img1(Rect(0, 0, 81, 243)));
		terrain(Rect(i, 0, 81, 243)).copyTo(img1(Rect(81, 0, 81, 243)));
		terrain(Rect(i+81, 0, 81, 243)).copyTo(img2(Rect(0, 0, 81, 243)));
		terrain(Rect(i+81, 0, 81, 243)).copyTo(img2(Rect(81, 0, 81, 243)));
		Mat blend = multiband(img1, img2);
		blend.copyTo(terrain(Rect(i, 0, 162, 243)));
	}
	transpose(terrain, terrain);
	return terrain;
}

class LaplacianBlending {
private:
    Mat left;
    Mat right;
    Mat blendMask;

    vector<Mat> leftLapPyr,rightLapPyr,resultLapPyr;
    Mat leftSmallestLevel, rightSmallestLevel, resultSmallestLevel;
    vector<Mat> maskGaussianPyramid; //masks are 3-channels for easier multiplication with RGB

    int levels;


    void buildPyramids() {
        buildLaplacianPyramid(left,leftLapPyr,leftSmallestLevel);
        buildLaplacianPyramid(right,rightLapPyr,rightSmallestLevel);
        buildGaussianPyramid();
    }

    void buildGaussianPyramid() {
        assert(leftLapPyr.size()>0);

        maskGaussianPyramid.clear();
        Mat currentImg;
        cvtColor(blendMask, currentImg, CV_GRAY2BGR);
        maskGaussianPyramid.push_back(currentImg); //highest level

        currentImg = blendMask;
        for (int l=1; l<levels+1; l++) {
            Mat _down;
            if (leftLapPyr.size() > l) {
                pyrDown(currentImg, _down, leftLapPyr[l].size());
            } else {
                pyrDown(currentImg, _down, leftSmallestLevel.size()); //smallest level
            }

            Mat down;
            cvtColor(_down, down, CV_GRAY2BGR);
            maskGaussianPyramid.push_back(down);
            currentImg = _down;
        }
    }

    void buildLaplacianPyramid(const Mat& img, vector<Mat>& lapPyr, Mat& smallestLevel) {
        lapPyr.clear();
        Mat currentImg = img;
        for (int l=0; l<levels; l++) {
            Mat down,up;
            pyrDown(currentImg, down);
            pyrUp(down, up, currentImg.size());
            Mat lap = currentImg - up;
            lapPyr.push_back(lap);
            currentImg = down;
        }
        currentImg.copyTo(smallestLevel);
    }

    Mat reconstructImgFromLapPyramid() {
        Mat currentImg = resultSmallestLevel;
        for (int l=levels-1; l>=0; l--) {
            Mat up;

            pyrUp(currentImg, up, resultLapPyr[l].size());
            currentImg = up + resultLapPyr[l];
        }
        return currentImg;
    }

    void blendLapPyrs() {
        resultSmallestLevel = leftSmallestLevel.mul(maskGaussianPyramid.back()) +
                                    rightSmallestLevel.mul(Scalar(1.0,1.0,1.0) - maskGaussianPyramid.back());
        for (int l=0; l<levels; l++) {
            Mat A = leftLapPyr[l].mul(maskGaussianPyramid[l]);
            Mat antiMask = Scalar(1.0,1.0,1.0) - maskGaussianPyramid[l];
            Mat B = rightLapPyr[l].mul(antiMask);
            Mat blendedLevel = A + B;

            resultLapPyr.push_back(blendedLevel);
        }
    }

public:
    LaplacianBlending(const Mat& _left, const Mat& _right, const Mat& _blendMask, int _levels):
    left(_left),right(_right),blendMask(_blendMask),levels(_levels)
    {
        assert(_left.size() == _right.size());
        assert(_left.size() == _blendMask.size());
        buildPyramids();
        blendLapPyrs();
    };

    Mat blend() {
        return reconstructImgFromLapPyramid();
    }
};

Mat LaplacianBlend(Mat terr) {
	Mat terrain = terr.clone();
	for(int i=41;i<125;i+=81){
		Mat l8u(243, 80, CV_8UC3, Scalar(0, 0, 0));
		Mat r8u(243, 80, CV_8UC3, Scalar(0, 0, 0));
		terrain(Rect(i, 0, 40, 243)).copyTo(l8u(Rect(0, 0, 40, 243)));
		terrain(Rect(i, 0, 40, 243)).copyTo(l8u(Rect(40, 0, 40, 243)));
		terrain(Rect(i+40, 0, 40, 243)).copyTo(r8u(Rect(0, 0, 40, 243)));
		terrain(Rect(i+40, 0, 40, 243)).copyTo(r8u(Rect(40, 0, 40, 243)));
		Mat l; l8u.convertTo(l,CV_32F,1.0/255.0);
		Mat r; r8u.convertTo(r,CV_32F,1.0/255.0);
		Mat_<float> m(l.rows,l.cols,0.0);
		m(Range::all(),Range(0,m.cols/2)) = 1.0;
		LaplacianBlending lb(l,r,m,4);
    	Mat blend = lb.blend();
    	blend.convertTo(blend, CV_8UC3, 255.0);
    	blend.copyTo(terrain(Rect(i, 0, 80, 243)));
	}
	transpose(terrain, terrain);
	for(int i=41;i<125;i+=81){
		Mat l8u(243, 80, CV_8UC3, Scalar(0, 0, 0));
		Mat r8u(243, 80, CV_8UC3, Scalar(0, 0, 0));
		terrain(Rect(i, 0, 40, 243)).copyTo(l8u(Rect(0, 0, 40, 243)));
		terrain(Rect(i, 0, 40, 243)).copyTo(l8u(Rect(40, 0, 40, 243)));
		terrain(Rect(i+40, 0, 40, 243)).copyTo(r8u(Rect(0, 0, 40, 243)));
		terrain(Rect(i+40, 0, 40, 243)).copyTo(r8u(Rect(40, 0, 40, 243)));
		Mat l; l8u.convertTo(l,CV_32F,1.0/255.0);
		Mat r; r8u.convertTo(r,CV_32F,1.0/255.0);
		Mat_<float> m(l.rows,l.cols,0.0);
		m(Range::all(),Range(0,m.cols/2)) = 1.0;
		LaplacianBlending lb(l,r,m,4);
	   	Mat blend = lb.blend();
	   	blend.convertTo(blend, CV_8UC3, 255.0);
	   	blend.copyTo(terrain(Rect(i, 0, 80, 243)));
	}
	transpose(terrain, terrain);
	return terrain;
}

#endif /* SRC_BLEND_H_ */
