#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     storing 3D image in 3d array
void store3DImage(Mat img, int*** arr) {
    int d1Row = img.rows, d2Col = img.cols;
    //depth is the 3rd dimension of the 3D array
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


//    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~     printing the 3d image 
void print3DImage(int*** arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            for (int k = 0; k < 1; k++) {
                cout << arr[i][j][k] ;
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

    //defining a 3x3 kernel
    //dividing by 64 so it stays normalized and the brightness is maintained
    float kernel[3][3][3] = {
    {{1.f / 64, 2.f / 64, 1.f / 64}, {2.f / 64, 4.f / 64, 2.f / 64}, {1.f / 64, 2.f / 64, 1.f / 64}},
    {{2.f / 64, 4.f / 64, 2.f / 64}, {4.f / 64, 8.f / 64, 4.f / 64}, {2.f / 64, 4.f / 64, 2.f / 64}},
    {{1.f / 64, 2.f / 64, 1.f / 64}, {2.f / 64, 4.f / 64, 2.f / 64}, {1.f / 64, 2.f / 64, 1.f / 64}}
    };

    // Temporary array to store the blurred image
    int*** tempArr_blur = new int** [rows];
    for (int i = 0; i < rows; ++i) {
        tempArr_blur[i] = new int* [cols];
        for (int j = 0; j < cols; ++j) {
            tempArr_blur[i][j] = new int[1]; // Assuming one channel for grayscale image
        }
    }

    // Applying the blurring kernel
    // Iterate over each pixel in the image
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            float blur_sum = 0.0;

            // Apply convolution with the kernel
            //kernel elements ko iterate kar raha hai 
            for (int ki = -1; ki <= 1; ++ki) {
                for (int kj = -1; kj <= 1; ++kj) {
                    for (int kk = -1; kk <= 1; ++kk) {
                        // Zero-padding for image borders
                        //input img k elements that are corresponding to the kernel 
                        int ni = i + ki;
                        int nj = j + kj;
                        int nk = kk;
                        ni = max(0, min(ni, rows - 1));      //min: ni doesnot exceed row -1 , max: ni is not negative
                        nj = max(0, min(nj, cols - 1));

                        blur_sum += arr[ni][nj][0] * kernel[ki + 1][kj + 1][kk + 2];
                    }
                }
            }

            //store the result in the blurred array, rounding to the nearest integer
            tempArr_blur[i][j][0] = static_cast<int>(blur_sum + 0.5f);
        }
    }

    // Copy the blurred array back to the original array
    for (int i = 1; i < rows - 1; ++i) {
        for (int j = 1; j < cols - 1; ++j) {
            // Assign the blurred pixel value from the temporary array to the corresponding position in the original array
            arr[i][j][0] = tempArr_blur[i][j][0];
        }
    }

    // Deallocate memory for the temporary array
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
    float kernel[3][3] = {
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
                        int ni = i + ki;
                        int nj = j + kj;
                        int nn = kn;
                        ni = max(0, min(ni, rows - 1));
                        nj = max(0, min(nj, cols - 1));

                        edge_sum += arr[ni][nj][0] * kernel[ki + 1][kj + 1]; // Convolution
                    }
                }
            }

            // Store the result in the edge-detected array
            tempArr_edge[i][j][0] = saturate_cast<int>(edge_sum);
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
            tempArr_feature[i][j][0] = saturate_cast<int>(feature_sum);
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
