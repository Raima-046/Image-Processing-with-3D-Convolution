# Image-Processing-with-3D-Convolution

# 3D Convolution Image Processing

This project implements 3D convolution operations using C++ and OpenCV. It includes functions for blurring, edge detection, and feature extraction on 3D grayscale images, with careful handling of image borders using zero-padding.





## Installation

Install OpenCV library. You can download it from https://opencv.org/releases/

```bash 
  cd https://github.com/Raima-046/Image-Processing-with-3D-Convolution
```
Download and run the executables

- https://github.com/Raima-046/Image-Processing-with-3D-Convolution/blob/main/3DConvolution.cpp

- https://github.com/Raima-046/Image-Processing-with-3D-Convolution/blob/main/3DConvolution.h



    
## Usage/Example

1. **Load a 3D Image:**
   - Ensure your 3D grayscale image is in the correct format and located in the project directory.
   - Use the provided function in your code to load the image. For example:
     ```cpp
     // Example of loading a 3D image
     Mat img = imread("path/to/your/image.file", IMREAD_GRAYSCALE);
     ```

2. **Apply 3D Convolution:**
   - For blurring:
     ```cpp
     // Example of applying blurring convolution
     applyBlurringConvolution(img);
     ```
   - For edge detection:
     ```cpp
     // Example of applying edge detection convolution
     applyEdgeDetectionConvolution(img);
     ```
   - For feature extraction:
     ```cpp
     // Example of applying feature extraction convolution
     applyFeatureExtractionConvolution(img);



## Features

- 3D Image Loading
- Blurring Convolution
- Edge Detection Convolution
- Feature Extraction Convolution
- Border Handling
- Image Saving


## Screenshots

***Blurred Image:***

![Blurred Image](https://github.com/Raima-046/Image-Processing-with-3D-Convolution/assets/118756849/b9d001ea-8429-4b79-b902-19d612064538)

***Edge Detected Image:***

![EdgeDetectedImage](https://github.com/Raima-046/Image-Processing-with-3D-Convolution/assets/118756849/f51f09ea-3e0f-4b14-9428-be4896aa6d5e)

***Feature Extracted Image:***

![FeatureExtractedImage](https://github.com/Raima-046/Image-Processing-with-3D-Convolution/assets/118756849/f90f9792-5062-43ee-806e-7868058714d0)


## Contributing

Contributions are always welcome!

See `contributing.md` for ways to get started.



## Documentation

[Documentation](https://docs.opencv.org/4.x/index.html)

