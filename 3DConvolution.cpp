#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     Storing 3D image in 3d array
void store3DImage(Mat img, int*** arr) {

    int d1Row = img.rows, d2Col = img.cols;
    //depth is the 3rd dimension of the #D array
    int d3Depth = 1;  //in grayscale img there is only one channel representing the intensity or brightness of each pixel not like colors images that have multiple colors representing their pixels eg rgb

    //loop for rows and colums iterates over each pixel of the img
    for (int i = 0; i < d1Row; i++) {
        for (int j = 0; j < d2Col; j++) {
            //access the intensity value using img.at<uchar>(i, j) and store it in the appropriate position in the 3D array
            for (int k = 0; k < d3Depth; k++) {
                //this function returns the intensity value at the specified pixel location at position (i, j)
                arr[i][j][k] = static_cast<int>(img.at<uchar>(i, j));
            }
        }
    }
    //this function reads the grayscale intensity values from the input image and stores them in a 3D integer array
}


//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     Printing the 3d image 
void print3DImage(int*** arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < 1; k++) {
                cout << arr[i][j][k];
            }
            cout << endl;
        }
        cout << endl;
    }
    //thsi function iterates over each element of the 3D array, printing the intensity value of each pixel in the grayscale img
}


//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     Apply 3D Convolution for Blurring
void apply3DConvolution(int*** arr, int rows, int cols) {
    //a kernel is a mask to blur the image it revolves over the image step by step an blurs the image 

    //defining a 3x3x3 gaussian kernel
    float kernel[3][3][3] = {
    {   {1.f / 64, 2.f / 64, 1.f / 64},
        {2.f / 64, 4.f / 64, 2.f / 64},
        {1.f / 64, 2.f / 64, 1.f / 64}
    },
    {
        {2.f / 64, 4.f / 64, 2.f / 64},
        {4.f / 64, 8.f / 64, 4.f / 64},
        {2.f / 64, 4.f / 64, 2.f / 64}
    },
    {
        {1.f / 64, 2.f / 64, 1.f / 64},
        {2.f / 64, 4.f / 64, 2.f / 64},
        {1.f / 64, 2.f / 64, 1.f / 64}
    }
    };

    //temporary array to store the blurred image
    int*** tempArr_blur = new int** [rows];
    for (int i = 0; i < rows; ++i) {
        tempArr_blur[i] = new int* [cols];
        for (int j = 0; j < cols; ++j) {
            tempArr_blur[i][j] = new int[1]; //assuming one channel for grayscale image
        }
    }

    // Applying the blurring kernel
    //iterate over each pixel in the image
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            float blurSum = 0.0;

            //pply convolution with the kernel
            for (int ki = -1; ki <= 1; ++ki) {
                for (int kj = -1; kj <= 1; ++kj) {
                    for (int kk = -1; kk <= 1; ++kk) {
                        // Zero-padding for image borders
                        //each position in the kernel the corespondin position in the image is calculated by 
                        //adding the current pixel coordinates i,j with the offset provided 
                        //by the kernel position ki,kj,kn
                        int ni = i + ki;
                        int nj = j + kj;
                        int nk = kk;
                        // these lines ensure that the indices ni and nj are within the valid range 
                        //of the image array to avoid accessing pixels outside the array bounds
                        ni = max(0, min(ni, rows - 1));
                        nj = max(0, min(nj, cols - 1));

                        blurSum += arr[ni][nj][0] * kernel[ki + 1][kj + 1][kk + 2];
                    }
                }
            }

            //store the result in the blurred array, rounding to the nearest integer
            tempArr_blur[i][j][0] = static_cast<int>(blurSum + 0.5f);
        }
    }

    //copy the blurred array back to the original array
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            //assign the blurred pixel value from the temporary array to the corresponding position in the original array
            arr[i][j][0] = tempArr_blur[i][j][0];
        }
    }

    //dealloacate mem
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            delete[] tempArr_blur[i][j];
        }
        delete[] tempArr_blur[i];
    }
    delete[] tempArr_blur;
}


//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~    Apply 3D Convolution for Edge Detection
void apply3DConvolutionEdgeDetection(int*** arr, int rows, int cols) {
    //Prewitt operator is similar to the Sobel operator and is used for detecting vertical and horizontal edges in images.

    //sobel filter
    int kernel[3][3] = {
        {1,0,-1},
        {2,0,-2},
        {1,0,-1},
    };

    //allocating mem for the temporary array to store the edge-detected image
    int*** tempArr_edge = new int** [rows];
    for (int i = 0; i < rows; ++i) {
        tempArr_edge[i] = new int* [cols];
        for (int j = 0; j < cols; ++j) {
            tempArr_edge[i][j] = new int[1]; //one channel for grayscale image
        }
    }

    // Apply the 3D edge detection kernel
    // Iterate over each pixel in the image
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            float edge_sum = 0.0;

            // Apply convolution with the kernel
            for (int ki = -1; ki <= 1; ++ki) {
                for (int kj = -1; kj <= 1; ++kj) {
                    for (int kn = -1; kn <= 1; ++kn) {
                        // Zero-padding for image borders
                        //each position in the kernel the corespondin position in the image is calculated by 
                        //adding the current pixel coordinates i,j with the offset provided 
                        //by the kernel position ki,kj,kn
                        int ni = i + ki;
                        int nj = j + kj;
                        int nn = kn;
                        // these lines ensure that the indices ni and nj are within the valid range 
                        //of the image array to avoid accessing pixels outside the array bounds
                        ni = max(0, min(ni, rows - 1));
                        nj = max(0, min(nj, cols - 1));

                        edge_sum += arr[ni][nj][0] * kernel[ki + 1][kj + 1]; // Convolution
                    }
                }
            }

            // Store the result in the edge-detected array
            //this function ensures that the result is cast to an unsigned char (8-bit) data type and saturates 
            //it within the range of 0 to 255. If the result is less than 0, it's set to 0, and if it's greater 
            //than 255, it's set to 255.
            tempArr_edge[i][j][0] = saturate_cast<uchar>(edge_sum);
        }
    }

    //copy the edge-detected array back to the original array
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            arr[i][j][0] = tempArr_edge[i][j][0];
        }
    }

    //deallocate memory for the temporary array
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            delete[] tempArr_edge[i][j];
        }
        delete[] tempArr_edge[i];
    }
    delete[] tempArr_edge;
}


//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   Apply 3D Convolution for Feature Extraction
void apply3DConvolutionFeatureExtraction(int*** arr, int rows, int cols) {
    //the high negative weight in the center of the kernel enhances the contrast between the central
    //pixel and its neighbors, making it more effective at detecting features with abrupt intensity changes

    //defining 3x3 kernel for feature extraction
    float kernel[3][3][3] = {
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 1, 0}, {1, -6, 1}, {0, 1, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
    };
    //The zero weights are added in all directions around the central weight to preserve the average 
    //intensity of the image along the x, y, and z axes.

    //allocate memory for the temporary array to store the feature-extracted image
    int*** tempArr_feature = new int** [rows];
    for (int i = 0; i < rows; ++i) {
        tempArr_feature[i] = new int* [cols];
        for (int j = 0; j < cols; ++j) {
            tempArr_feature[i][j] = new int[1]; //assuming one channel for grayscale image
        }
    }

    //Apply the 3D feature extraction kernel
    //iterating over each pixel in the image excluding the borders
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            float feature_sum = 0.0;

            //Apply convolution with the kernel
            //thses  loops iterate over each element of the kernel
            for (int ki = -1; ki <= 1; ki++) {
                for (int kj = -1; kj <= 1; kj++) {
                    for (int kn = -1; kn <= 1; kn++) {
                        // Zero-padding for image borders
                        //each position in the kernel the corespondin position in the image is calculated by 
                        //adding the current pixel coordinates i,j with the offset provided 
                        //by the kernel position ki,kj,kn
                        int ni = i + ki;
                        int nj = j + kj;
                        int nn = kn;
                        // these lines ensure that the indices ni and nj are within the valid range 
                        //of the image array to avoid accessing pixels outside the array bounds
                        ni = max(0, min(ni, rows - 1));
                        nj = max(0, min(nj, cols - 1));
                        //multiplying the intensity value of the image pixel at position (ni, nj, 0) 
                        //with the corresponding kernel value kernel[ki + 1][kj + 1][1]
                        feature_sum += arr[ni][nj][0] * kernel[ki + 1][kj + 1][1];
                    }
                }
            }

            //this function ensures that the result is cast to an unsigned char (8-bit) data type and saturates 
            //it within the range of 0 to 255. If the result is less than 0, it's set to 0, and if it's greater 
            //than 255, it's set to 255.
            tempArr_feature[i][j][0] = saturate_cast<uchar>(feature_sum);
        }
    }

    // Copy the feature-extracted array back to the original array
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            arr[i][j][0] = tempArr_feature[i][j][0];
        }
    }

    // Deallocate memory for the temporary array
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            delete[] tempArr_feature[i][j];
        }
        delete[] tempArr_feature[i];
    }
    delete[] tempArr_feature;
}


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
