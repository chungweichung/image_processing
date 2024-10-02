#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <string>

using namespace cv;
using namespace std;
class Zoom{
    private:
        Vec3b bilinearInterpolation(Mat img,double x,double y){ 
            int x1=floor(x);
            int x2=min(ceil(x),double(img.cols-1));
            int y1=floor(y);
            int y2=min(ceil(y),double(img.rows-1));
            int channel=img.channels();
            double pixel11,pixel12,pixel21,pixel22;
            double dx=x-x1,dy=y-y1;
            Vec3b result;
            for (int i=0;i<channel;i++){
                pixel11=img.at<Vec3b>(y1,x1)[i];
                pixel12=img.at<Vec3b>(y1,x2)[i];
                pixel21=img.at<Vec3b>(y2,x1)[i];
                pixel22=img.at<Vec3b>(y2,x2)[i];
                result[i]=static_cast<uchar>((1-dx)*(1-dy)*pixel11+dx*(1-dy)*pixel12+
                (1-dx)*dy*pixel21+dx*dy*pixel22);
            }
            return result;
        }
    Mat nearestZoom(int width,int height){
        Mat zoomedImg(height,width,img.type());
        int oldWidth=img.cols,oldHeight=img.rows;
        for (double i =0;i<height;i++){
            for(double j=0;j<width;j++){      
                zoomedImg.at<Vec3b>(i,j)=img.at<Vec3b>(round(i*oldHeight/height),round(j*oldWidth/width));
            }
        }
    return zoomedImg;
    }
    Mat bilinearZoom(int width,int height){
        Mat zoomedImg(height,width,img.type());
        int oldWidth=img.cols,oldHeight=img.rows;
        for (int i =0;i<height;i++){
            for(int j=0;j<width;j++){
                zoomedImg.at<Vec3b>(i,j)=bilinearInterpolation(img,(double)(j)*oldWidth/width,(double)(i)*oldHeight/height);
            }
        }
        return zoomedImg;
    }
    public:
        Mat img;
        Zoom(Mat img){
            this->img=img;
        };
        Mat toSize(int height,int width,string type){
            if(type=="nearest"){
                return nearestZoom(width,height);
            }else{
                return  bilinearZoom(width,height);
            }

        }
};

int main()
{
    Mat img = imread("lena.jpg");
    if(img.empty()){
        cout << "open img failed " << endl;
    }

    //cv::imshow("Example", img);
    Mat nearestZoomed = Zoom(img).toSize(1024, 1000, "nearest");
    Mat bilinearZoomed = Zoom(img).toSize(1000, 1024, "bilinear");
    imwrite("./nearest.jpg", nearestZoomed);
    imwrite("./bilinear.jpg", bilinearZoomed);
    cv::waitKey(0);

    destroyWindow("Example");
    
    return 0;
}
