#include "ZoomSDKRendererDelegate.h"


ZoomSDKRendererDelegate::ZoomSDKRendererDelegate() {
    // For X11 Forwarding
    XInitThreads();

    if (!m_cascade.load("/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml"))
        Log::error("failed to load cascade file");

    m_faces.reserve(2);
    m_socketServer.start();
}

void ZoomSDKRendererDelegate::onRawDataFrameReceived(YUVRawDataI420 *data)
{

	std::cout << "onRawDataFrameReceived." << std::endl;

	std::cout << "width." << data->GetStreamWidth() << std::endl;
	std::cout << "height." << data->GetStreamHeight() << std::endl;

  SaveToRawYUVFile(data);
}

void ZoomSDKRendererDelegate::SaveToRawYUVFile(YUVRawDataI420* data) {

	// Open the file for writing
	std::ofstream outputFile("out/video.yuv", std::ios::out | std::ios::binary | std::ios::app);
	if (!outputFile.is_open())
	{
		std::cout << "Error opening file." << std::endl;
		return;
	}
	// Calculate the sizes for Y, U, and V components
	size_t ySize = data->GetStreamWidth() * data->GetStreamHeight();
	size_t uvSize = ySize / 4;



	// Write Y, U, and V components to the output file
	outputFile.write(data->GetYBuffer(), ySize);
	outputFile.write(data->GetUBuffer(), uvSize);
	outputFile.write(data->GetVBuffer(), uvSize);


	// Close the file
	outputFile.close();
	outputFile.flush();
	//cout << "YUV420 buffer saved to file." << endl;
}

void ZoomSDKRendererDelegate::writeToFile(const string &path, YUVRawDataI420 *data)
{

	std::ofstream file(path, std::ios::out | std::ios::binary | std::ios::app);
	if (!file.is_open())
        return Log::error("failed to open video output file: " + path);

	file.write(data->GetBuffer(), data->GetBufferLen());

	file.close();
	file.flush();
}

void ZoomSDKRendererDelegate::setDir(const string &dir)
{
    m_dir = dir;
}

void ZoomSDKRendererDelegate::setFilename(const string &filename)
{
    m_filename = filename;
}
