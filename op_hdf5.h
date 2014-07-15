#include "hdf5.h"
#include "hdf5_hl.h"
#include <vector>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

template <typename T>
struct data_hdf5;

bool trans_data(vector<Mat> vmat, data_hdf5<float> data);
class op_hdf5
{
public:
	op_hdf5(string filename);
	void writedata(vector<Mat> vmat, string datasetname){
		writedata(vmat, datasetname.c_str());}
	void writedata(vector<Mat> vmat, char* datasetname);
	void writedata(data_hdf5<float> data, string datasetname){
		writedata(data, datasetname.c_str());}
	void writedata(data_hdf5<float> data, char* datasetname);
private:
	string filename;
};
template <typename T>
struct data_hdf5
{
	T * data;
	hsize_t * dims;
	int rank;
	void init(T * datain, int * dimsin, int rankin)
	{
		data = datain;
		dims = new hsize_t[rankin];
		for(int i = 0; i < rankin; i ++ ){
			dims[i] = dimsin[i];
		}
		rank = rankin;
	}
	void release(){
		delete []dims;
	}
};