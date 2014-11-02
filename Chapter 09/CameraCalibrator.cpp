#include "CameraCalibrator.h"

// ������ͼ����ȡ�ǵ�
int CameraCalibrator::addChessboardPoints(
    const std::vector<std::string> &filelist,
    cv::Size &boardSize)
{
    // �����ϵĵ����������
    std::vector<cv::Point2f> imageCorners;
    std::vector<cv::Point3f> objectCorners;

    // 3D�����еĵ�:
    // ����������ϵ�г�ʼ�����̽ǵ�
    // ��Щ��λ�� (X,Y,Z)= (i,j,0)
    for (int i = 0; i < boardSize.height; i++)
    {
        for (int j = 0; j < boardSize.width; j++)
        {
            objectCorners.push_back(cv::Point3f(i, j, 0.0f));
        }
    }

    // 2Dͼ���еĵ�:
    cv::Mat image; // ������������ͼ��
    int successes = 0;

    // ѭ������ͼƬ
    for (int i = 0; i < filelist.size(); i++)
    {
        // ��ͼ��
        image = cv::imread(filelist[i], 0);

        // �õ����̽ǵ�
        bool found = cv::findChessboardCorners(image, boardSize, imageCorners);

        // ��ȡ�����ؾ���
        cv::cornerSubPix(image, imageCorners,
                         cv::Size(5, 5),
                         cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::MAX_ITER +
                                          cv::TermCriteria::EPS,
                                          30,        // ��������Ŀ
                                          0.1));     // ��С����

        // ����ǵ���Ŀ����ҪҪ����ô������������
        if (imageCorners.size() == boardSize.area())
        {
            // ���һ���ӽ��е�ͼ��㼰������
            addPoints(imageCorners, objectCorners);
            successes++;
        }

        // ���ƽǵ�
		// found �Ѿ��ҵ��ǵ�
        cv::drawChessboardCorners(image, boardSize, imageCorners, found);

		// ��ʾ
        cv::imshow("Corners on Chessboard", image);
        cv::waitKey(100);
    }

    return successes;
}

// ��ӳ��������Ӧ��ͼ���
void CameraCalibrator::addPoints(const std::vector<cv::Point2f> &imageCorners, const std::vector<cv::Point3f> &objectCorners)
{
    // 2Dͼ���
    imagePoints.push_back(imageCorners);

    // ��Ӧ3D�����еĵ�
    objectPoints.push_back(objectCorners);
}

// ���б궨��������ͶӰ���
double CameraCalibrator::calibrate(cv::Size &imageSize)
{
    // �������½���ȥ����
    mustInitUndistort = true;

    //�����ת��ƽ��
    std::vector<cv::Mat> rvecs, tvecs;

    // ��ʼ�궨
    return
        calibrateCamera(objectPoints, // 3D��
                        imagePoints,  // ͼ���
                        imageSize,    // ͼ��ߴ�
                        cameraMatrix, // ͼ��ߴ�
                        distCoeffs,   // ������������
                        rvecs, tvecs, // ��ת��ƽ��
                        flag);        // ����ѡ��
    //                  ,CV_CALIB_USE_INTRINSIC_GUESS);

}

// �궨��ȥ��ͼ���еĻ���
cv::Mat CameraCalibrator::remap(const cv::Mat &image)
{
    cv::Mat undistorted;

    if (mustInitUndistort)   // ÿ�α궨ֻ��Ҫ��ʼ��һ��
    {
        cv::initUndistortRectifyMap(
            cameraMatrix,  // ����õ����������
            distCoeffs,    // ����õ��Ļ������
            cv::Mat(),     // ��ѡ��rectification����(�˴�Ϊ��)
            cv::Mat(),     // ��������undistorted������������
            cv::Size(640, 480), // image.size(), undistorted����ĳߴ�
            CV_32FC1,      // ���ӳ��ͼ�������
            map1, map2);   // x�����y����ӳ�亯��

        mustInitUndistort = false;
    }

    // Ӧ��ӳ�亯��
    cv::remap(image, undistorted, map1, map2,
              cv::INTER_LINEAR); // ��ֵ����

    return undistorted;
}


// Set the calibration options
// 8radialCoeffEnabled should be true if 8 radial coefficients are required (5 is default)
// tangentialParamEnabled should be true if tangeantial distortion is present
void CameraCalibrator::setCalibrationFlag(bool radial8CoeffEnabled, bool tangentialParamEnabled)
{
    // Set the flag used in cv::calibrateCamera()
    flag = 0;
    if (!tangentialParamEnabled) flag += CV_CALIB_ZERO_TANGENT_DIST;
    if (radial8CoeffEnabled) flag += CV_CALIB_RATIONAL_MODEL;
}

