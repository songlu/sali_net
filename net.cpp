// ConsoleApplication3.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include "net_func.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "op_folder.h"
#include "HDF5_test.h"
using namespace std;
using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	hdf5_test5();
	//hdf5_test1();
	Mat mat = Mat::ones(4,4,CV_32F);
	mat.at<float>(2,3) = 3;
	FileStorage fs("./vocabulary.xml",FileStorage::WRITE);
	fs<<"vocabulary"<<mat;
	fs.release();
	op_folder_test();
    const char* imagename = "lena.jpg";

    //���ļ��ж���ͼ��
	Mat img = imread(imagename,CV_LOAD_IMAGE_GRAYSCALE);
	Mat add1 = Mat::ones(10,11,CV_32F);
	Mat add2 = Mat::ones(10,11,CV_32F);
	add2 = add2 * 2;
	add1 = add1 + add2;
	vector<Mat> vout;
	//laye1_out(img, vout);
	vector<Mat> in;
	in.push_back(img);
	vector<Mat> out;
	constrast_norm(in, out);
    //�������ͼ��ʧ��
    if(img.empty())
    {
        fprintf(stderr, "Can not load image %s\n", imagename);
        return -1;
    }

    //��ʾͼ��
    imshow("image", img);

    //�˺����ȴ�������������������ͷ���
    waitKey();

    return 0;
}