#include "stdafx.h"
#include "op_hdf5.h"
#include <vector>
using namespace std;
op_hdf5::op_hdf5(string filename){
	this->filename = filename;
	hid_t file_id = H5Fcreate (filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	herr_t status = H5Fclose (file_id);
}
void op_hdf5::writedata(data_hdf5<float> data, char* datasetname){
	hid_t file_id = H5Fopen (filename.c_str, H5F_ACC_RDWR, H5P_DEFAULT);
	hid_t dataspace_id = H5Screate_simple (data.rank, data.dims, NULL); 
	hid_t dataset_id = H5Dcreate2 (file_id, datasetname, H5T_IEEE_F32BE, dataspace_id,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	herr_t status = H5Sclose (dataspace_id);
    status = H5Dclose (dataset_id);
	status = H5Fclose (file_id);
}
void op_hdf5::writedata(vector<Mat> vmat, char *datasetname){
	data_hdf5<float> data;
	trans_data(vmat, data);
    writedata(data, datasetname);
	data.release();
	delete []data.data;
}

bool trans_data(vector<Mat> vmat, data_hdf5<float> data){
	int len = vmat.size();
	if( len <= 0 )
		return false;
	int cols = vmat[0].cols;
	int rows = vmat[0].rows;
	int size = cols * rows;
	float * dat = new float[size*len];
	for( int i = 0; i < len; i ++ ){
		for( int y = 0; y < rows; y ++ ){
			for( int x = 0; x < cols; x ++ ){
			    dat[i*size+y*cols+x] = vmat[i].at<float>(y,x);
			}
		}
	}
	int *dimsin = new int[3];
	dimsin[0] = len;
	dimsin[1] = rows;
	dimsin[2] = cols;
	data.init(dat,dimsin,3);
	return true;
}