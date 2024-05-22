#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "3DConvolution.h"
#include <iostream>

using namespace cv;
using namespace std;

//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   MAIN FUNCTION
int main() {

    cout << "                          RAIMA TARIQ           22I-1145             QUESTION 03" << endl << endl;
    
    //obj
    Mat img;

    //---------------------------------->>>>>>> LOADING GRAYSCALE IMAGE

    //path for image
    img = imread("C:/Users/e touch computer/OneDrive/Desktop/raima/dataAsg/opencvAgain/opencvAgain/image-1.tiff", IMREAD_GRAYSCALE);

    //if there's an error in opening the image
    if (img.empty()) {
        cout << "Could not open or find the image!" << endl;
        exit(0);
    }


    //displaying the dimensions
    cout << "                               ----------->>>> DIMENSIONS <<<<-----------" << endl << endl;
    cout << "Image rows: " << img.rows << endl;
    cout << "Image columns: " << img.cols << endl;

    //storing the rows and columns of the img in the variables
    int d_rows = img.rows, d_cols = img.cols;

    //dma for 3darray
    int*** arr = new int** [d_rows];
    for (int i = 0; i < d_rows; ++i) {
        arr[i] = new int* [d_cols];
        for (int j = 0; j < d_cols; ++j) {
            //only one channel in grayscale
            arr[i][j] = new int[1];
        }
    }

    //prints the content of the image
   // print3DImage(arr, d_rows, d_cols);

    //FUNCTION CALL TO STORE THE 3D IAMGE
    store3DImage(img, arr);

    //--------------------------------->>>>>>>> STORING GRAYSCALE IMG

    //name of window
    namedWindow("Grayscale Image", WINDOW_AUTOSIZE);

    //TO DISPLAY THE GRAYSCALE IMG USING ITS OBJ
    imshow("Grayscale Image", img);

    //---------------------------------->>>>>>> APPLYING 3D CONVOLUTION FOR BLURRING

    //name of window
    namedWindow("Blurred Image", WINDOW_AUTOSIZE);

    //ftn call
    apply3DConvolution(arr, d_rows, d_cols);

    // obj for blurred img
    // Mat is the OpenCV class representing a matrix i.e. an image
    //CV_8UC1 specifies the type of the image, where CV_8UC1 stands for an 8-bit single-channel grayscale image
    //cv is for opencv
    //8u  specifies the depth of the image data. 8U indicates an 8-bit unsigned integer type, meaning each pixel value ranges from 0 to 255.
    //c1 defines the number of channels

    //obj fr the blurred image 
    Mat blurr(d_rows, d_cols, CV_8UC1);

    for (int i = 0; i < d_rows; ++i) {
        for (int j = 0; j < d_cols; ++j) {
            //assign the intensity value of the pixel from the 3D array to the corresponding position in the Mat object at i and j
            blurr.at<uchar>(i, j) = arr[i][j][0];
        }
    }

    //dippalying the image
    imshow("Blurred Image", blurr);

    //storing it as png
    imwrite("Blurred Image.png", blurr);

    //---------------------------------->>>>>>> APPLYING 3D CONVOLUTION FOR EDGE DETECTION

    //name of window
    namedWindow("Edge-detected Image", WINDOW_AUTOSIZE);

    //ftn call
    apply3DConvolutionEdgeDetection(arr, d_rows, d_cols);

    //obj for edge detection
    Mat edgeDet(d_rows, d_cols, CV_8UC1);


    for (int i = 0; i < d_rows; ++i) {
        for (int j = 0; j < d_cols; ++j) {
            // Assign the intensity value of the pixel from the 3D array to the corresponding position in the Mat object
            edgeDet.at<uchar>(i, j) = arr[i][j][0];
        }
    }

    //displaying in window
    imshow("Edge-detected Image", edgeDet);

    //storing as png file
    imwrite("EdgeDetectedImage.png", edgeDet);


    //---------------------------------->>>>>>> APPLYING 3D CONVOLUTION FOR FEATURE EXTRACTION

    //name of window
    namedWindow("Feature-extracted Image", WINDOW_AUTOSIZE);

    //ftn call
    apply3DConvolutionFeatureExtraction(arr, d_rows, d_cols);

    //obj for the feature extraction 
    Mat featExt(d_rows, d_cols, CV_8UC1);

    for (int i = 0; i < d_rows; ++i) {
        for (int j = 0; j < d_cols; ++j) {
            // Assign the intensity value of the pixel from the 3D array to the corresponding position in the Mat object
            featExt.at<uchar>(i, j) = arr[i][j][0];
        }
    }

    //displaying in window
    imshow("Feature-extracted Image", featExt);

    //storing as png file
    imwrite("FeatureExtractedImage.png", featExt);

    //wait for a key press to exit
    waitKey(0);

    //close all window
    destroyAllWindows();


    //deallocation of the 3d array
    for (int i = 0; i < d_rows; ++i) {
        for (int j = 0; j < d_cols; ++j) {
            delete[] arr[i][j];
        }
        delete[] arr[i];
    }
    delete[] arr;

}
