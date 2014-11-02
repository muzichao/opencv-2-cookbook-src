#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

#include "CameraCalibrator.h"

using namespace std;
using namespace cv;

int main()
{
    namedWindow("Image");
    Mat image;
    vector<string> filelist; // �ļ����б�

    // ��������ͼ���ļ����б�
    for (int i = 1; i <= 20; i++)
    {
        stringstream str;
		// �����ļ����ַ���
        str << "E:/��������/���/openCV/opencv-2-cookbook-src-master/images/chessboards/chessboard" << setw(2) << setfill('0') << i << ".jpg";
        cout << str.str() << endl;

        filelist.push_back(str.str()); // ���ַ���str.str()ѹ������
        image = imread(str.str(), 0); // ��ʾ�ļ�
        imshow("Image", image);

        waitKey(100);
    }

    // �����궨����
    CameraCalibrator cameraCalibrator;

    // ���̽ǵ㣨��������Ե��
    Size boardSize(6, 4);

	// filelist : �ļ����б�
	// boardSize : ���̽ǵ����
	// ������ͼ����ȡ�ǵ�
    cameraCalibrator.addChessboardPoints(filelist, boardSize);

    // ����궨
    //  cameraCalibrator.setCalibrationFlag(true,true);
    cameraCalibrator.calibrate(image.size());

    // ͼ��ȥ����
    image = imread(filelist[6]);
    Mat uImage = cameraCalibrator.remap(image);

    // ��ʾ�������
    Mat cameraMatrix = cameraCalibrator.getCameraMatrix();
    cout << " Camera intrinsic: " << cameraMatrix.rows << "x" << cameraMatrix.cols << endl;
    cout << cameraMatrix.at<double>(0, 0) << " " << cameraMatrix.at<double>(0, 1) << " " << cameraMatrix.at<double>(0, 2) << endl;
    cout << cameraMatrix.at<double>(1, 0) << " " << cameraMatrix.at<double>(1, 1) << " " << cameraMatrix.at<double>(1, 2) << endl;
    cout << cameraMatrix.at<double>(2, 0) << " " << cameraMatrix.at<double>(2, 1) << " " << cameraMatrix.at<double>(2, 2) << endl;

    imshow("Original Image", image);
    imshow("Undistorted Image", uImage);

    waitKey();
    return 0;
}
