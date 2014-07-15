#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "op_folder.h"
#include "op_hdf5.h"
#include "op_rand.h"
using namespace std;
using namespace cv;
/*
template<typename  T>
T min(T t1, T t2){
	return t1>t2?t2:t1;
}

template<typename  T>
T max(T t1, T t2){
	return t1<t2?t2:t1;
}
*/
void laye1_out(Mat in, vector<Mat> &vout)
{
	int kernel_size = 41;
	Mat kernel[4];
	for( int n = 0; n< 4; n ++){
        Mat out;
		kernel[n] = Mat::ones(kernel_size,kernel_size,CV_32F);
		filter2D(in, out,-1, kernel[n]);
		vout.push_back(out);
	}
	cout<<in.cols<<" "<<in.rows<<endl;
	cout<<vout[0].cols<<" "<<vout[0].rows<<endl;
}

void constrast_norm(vector<Mat> in, vector<Mat> vout, int batch_size = 16)
{
	int chan = in.size();
	Mat IntMap = Mat::zeros(in[0].rows+1, in[0].cols+1, CV_32F);
	Mat temp;// = Mat::zeros(in[0].rows+1, in[0].cols+1, CV_32F);
	for( int i = 0; i < chan; i ++ )
	{
		integral(in[i], temp, CV_32F);
		IntMap = IntMap + temp;
	}

	Mat out;
	for( int i = 0; i < chan; i ++ )
	{ 
		out = Mat::zeros(in[0].rows,in[0].cols, CV_32F);
		vout.push_back(out);
	}

	Mat normalfactor = Mat::zeros(in[0].rows, in[0].cols, CV_32F);
	int half_size1 = batch_size/2;
	int half_size2 = batch_size - half_size1 - 1;
	for( int i = 0; i < in[0].rows; i ++ )
	{
		int up = max(0,i-half_size1);
		int down = min(in[0].rows-1,i+half_size2)+1;
		int winsize1 = down - up;
		for( int j = 0; j < in[0].cols; j ++ )
		{
			int left = max(0, j-half_size1);
			int right = min(in[0].cols-1,j+half_size2)+1;
			int winsize2 = right - left;
			normalfactor.at<float>(i,j) = IntMap.at<float>(down,right) + 
				IntMap.at<float>(up,left)-IntMap.at<float>(down,left) -
				IntMap.at<float>(up,right);
			normalfactor.at<float>(i,j) = normalfactor.at<float>(i,j)/(winsize1*winsize2)*
				batch_size * batch_size;
			for( int k = 0; k < chan; k ++ )
			{
				vout[k].at<float>(i,j) = in[k].at<unsigned char>(i,j)/normalfactor.at<float>(i,j);
			}
		}
	}
}

bool gene_template(const int type, String in_folder, String outfilename, int randsample_num = 1000)
{
	switch(type)
	{
	case 1:
		// random sampling
		int batch_size[4] = {4,8,12,16};
		CBrowseDir CBD;
		//设置要遍历的目录
	    if (!CBD.SetInitDir(in_folder))
	    {
		    puts("目录不存在。");
		    return false;
	    }	
	    //开始遍历
	    CBD.BeginBrowse("*.*");
		vector<string> vfile = CBD.GetVfile();
		vector<Mat> vrandsample;
		int filenum = vfile.size();
		class op_hdf5 op_data(outfilename);
		class op_rand rand_generator;
		for( int n = 0; n < 4; n ++ ){
			vrandsample.clear();
			for( int k = 0; k < randsample_num; k ++ ){
				int starty,startx;
				int fileNo = rand_generator.gene_rand_num(filenum);

				// read the featurefile
				int cols, rows, featchan;
				Mat feature;


				rand_generator.gene_rand_box(rows, cols, batch_size[n], starty,startx);
				Mat subfeature;
				subfeature.create(batch_size[n]*batch_size[n], feature.cols, CV_32F);
			    for( int tempy = 0; tempy < batch_size[n]; tempy ++ ){
					for( int tempx = 0; tempx < batch_size[n]; tempx ++ ){
						for( int ch = 0; ch< featchan; ch ++ ){
						    subfeature.at<float>(tempy*batch_size[n]+tempx,ch) = 
								feature.at<float>((starty+tempy)*batch_size[n]+startx+tempx, ch);
						}
					}
				}
				vrandsample.push_back(subfeature);
			}	
			char temp[200];
			temp[0] = 0;
			strcat_s(temp,"randsamp_");
			int len = strlen(temp);
			itoa(batch_size[n], temp+len, 10);
			op_data.writedata(vrandsample, temp);
		}
		break;
	}
	return true;
}



void estimate_alpha()
{
	
}
void process_template(const int type, Mat featin, int rows, int cols, vector<Mat> vrandtemp, int patchsize, Mat & featout)
{
	switch(type)
	{
	case 1:
		int chanin = featin.cols;
		int ptnum = featin.rows;
		int randtempnum = vrandtemp.size();
		vector<Mat> vfeatin;
		for( int n = 0; n < chanin; n ++ ){
			Mat temp = featin(Rect(n, 0, 1, ptnum));
			vfeatin.push_back(temp.reshape(1, rows));
		}
		featout.create(ptnum, randtempnum, CV_32F);
		for( int n = 0; n < randtempnum; n ++ )
		{
			Mat kernel = vrandtemp[n];
			Mat out = Mat::zeros(rows, cols, CV_32F);
			for( int m = 0; m < chanin; m ++ )
			{
				Mat outtemp;
				Mat kerneltemp = kernel(Rect(m,0,1, patchsize*patchsize)).reshape(1,patchsize);

				filter2D(vfeatin[m], outtemp,-1, kerneltemp);
				out = out + outtemp;
			}
			out.reshape(1, ptnum).copyTo(featout(Rect(n,0,1,ptnum)));
		}		
		break;
	}
}
void process_sunit(vector<Mat> in, float beta, Mat alpha1, Mat alpha0, vector<Mat> out)
{
	int chan = in.size();
	Mat temp1;
	Mat temp0;
	pow(alpha1,beta,temp1);
	pow(alpha0,beta,temp0);
	Mat gamma = 1.0/alpha0-1.0/alpha1;
	Mat T;
	log(alpha1/alpha0,T);

	for( int i = 0; i < chan; i ++ )
	{
		pow(abs(in[i]),beta,temp0);
		out.push_back(temp0*gamma.at<float>(i,0)-T.at<float>(i,0));
	}
}

void process_cunit()
{
}
