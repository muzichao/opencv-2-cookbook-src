#ifndef CAMERACALIBRATOR_H
#define CAMERACALIBRATOR_H

#include <vector>
#include <iostream>

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>

class CameraCalibrator
{
    // �����
	// λ����������ĵ�
    std::vector<std::vector<cv::Point3f>> objectPoints;

	// ���������
    std::vector<std::vector<cv::Point2f>> imagePoints;

    // �������
    cv::Mat cameraMatrix;
    cv::Mat distCoeffs;

    // �궨��ʽ
    int flag;

    // ����ͼ��ȥ����
    cv::Mat map1, map2;
    bool mustInitUndistort;

public:
    CameraCalibrator() : flag(0), mustInitUndistort(true) {};

    // ������ͼ����ȡ�ǵ�
    int addChessboardPoints(const std::vector<std::string> &filelist, cv::Size &boardSize);

    // ���ӳ�����Ͷ�Ӧ��ͼ���
    void addPoints(const std::vector<cv::Point2f> &imageCorners, const std::vector<cv::Point3f> &objectCorners);

    // ����궨
    double calibrate(cv::Size &imageSize);

    // ��������궨��ʽ
    void setCalibrationFlag(bool radial8CoeffEnabled = false, bool tangentialParamEnabled = false);

    // �궨��ȥ��ͼ���еĻ���
    cv::Mat CameraCalibrator::remap(const cv::Mat &image);

    // Getters
    cv::Mat getCameraMatrix()
    {
        return cameraMatrix;
    }
    cv::Mat getDistCoeffs()
    {
        return distCoeffs;
    }
};

#endif // CAMERACALIBRATOR_H
