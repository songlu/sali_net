#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
void laye1_out(Mat in, vector<Mat> &vout);
void constrast_norm(vector<Mat> in, vector<Mat> vout, int batch_size = 16);