#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
RNG rng(12345);
std::string ext(".jpg");
typedef struct node 
{ 
    int x; 
    int y; 
    struct node *next; 
}node;

struct sort_pred {
    bool operator()(const std::pair<cv::Point2f,int> &left, const std::pair<cv::Point2f,int> &right) {
        return left.second < right.second;
    }
};

node *rear,*front;
int Radius[20];

char shapes[20][20];
char colors[20][20];

int isEmpty(){ 
    if(front==NULL) 
        return 1; 
    else return 0; 
} 
void enqueue(int x,int y){ 
    node* t; 
    t=(node*)malloc(sizeof(node)); 
    t->x=x; 
    t->y=y; 
    t->next=NULL; 
    if(isEmpty()){ 
        front=t; 
        rear=front; 
    } 
    else{ 
            rear->next=t; 
            rear=t; 
            rear->next=NULL; 
    } 
} 
void queuefront(int *x,int *y){ 
    *x=front->x; 
    *y=front->y; 
    return; 
} 
void dequeue(){ 
    node* t; 
    if(isEmpty()){ 
        printf("Queue Underflow\n"); 
        return; 
    } 
    t=front; 
    front=t->next; 
    free(t); 
}

void removesmallblob(cv::Mat binimg,int **visited,int *count){
    int i,j;
    for(i=0;i<binimg.rows;i++)
            for(j=0;j<binimg.cols;j++)
                if(visited[i][j]==*count){
                    visited[i][j]=0;
                    binimg.at<uchar>(i,j)=0;
                }
                return;
}

int** blobDetection(cv::Mat binimg,int *count){ 
    int i,j,k,l,x,y; 
    *count=0; 
    int blobsize=500;
    int rows = binimg.rows; 
    int cols = binimg.cols; 
    int **visited; 
    visited = (int**)malloc(rows*sizeof(int*)); 
    for(i=0;i<rows;i++) 
        visited[i]=(int*)malloc(cols*sizeof(int)); 
    for(i=0;i<rows;i++) 
        for(j=0;j<cols;j++) 
            visited[i][j]=-1; 
      
    for(i=1;i<rows-1;i++) 
        for(j=1;j<cols-1;j++){ 
            if(binimg.at<uchar>(i,j)==255){    
                if(visited[i][j]==-1){ 
                    if(blobsize>400){
                    *count=*count +1; 
                    blobsize=0;
                    
                    }
                    else{
                        removesmallblob(binimg,visited,count);
                        blobsize=0;
                 
                    }
                
                    
                    front=NULL; 
                    enqueue(i,j); 
                    queuefront(&x,&y); 
                    while(!isEmpty()){ 
                        queuefront(&x,&y); 
                        dequeue(); 
  
                        for(k=x-1;k<=x+1;k++) 
                            for(l=y-1;l<=y+1;l++){ 
                                if((k>0)&&(k<rows)&&(l>0)&&(l<cols))
                                 if(binimg.at<uchar>(k,l)==255 && visited[k][l]==-1){ 
                                        enqueue(k,l); 
                                        visited[k][l]=*count; 
                                        blobsize++;
                                    } 
                                 } 
                        visited[x][y]=*count; 
                        } 
  
                    }   
                } 
            } 
        return visited; 
}

node findcentroid(cv::Mat img,int blobno,int **visited){

        int tx=0,ty=0,n=0;
        float cx,cy;
        node centroid;
        centroid.next=NULL;
        int i,j;
        for(i=0;i<img.rows;i++)
            for(j=0;j<img.cols;j++)
                if(visited[i][j]==blobno){
                    tx+=i;
                    ty+=j;
                    n++;                    
                }

        centroid.x=tx/n;
        centroid.y=ty/n;
        return centroid;
}

void getcolor(cv::Mat imgYUV, int blobno ,node centroid[20]){

    int tol = 5;
    int i=centroid[blobno].x;
    int j= centroid[blobno].y;
    int b = imgYUV.at<cv::Vec3b>(i,j)[0];
    int g = imgYUV.at<cv::Vec3b>(i,j)[1];
    int r = imgYUV.at<cv::Vec3b>(i,j)[2];
    if(b > 200 && g <25 && r < 25){
        strcpy(colors[blobno],"blue");
        return;
    }
    if(b < 25 && g >200 && r < 25){
        strcpy(colors[blobno],"green");
        return;
    } 
    if(b < 25 && g < 25 && r > 200){
        strcpy(colors[blobno],"red");
        return;
    }  
    std::cout << "Recalibrate";
    strcpy(colors[blobno],"none");
    return;
}

void showblob(cv::Mat binary, int** visited, int blobno){

    cv::Mat blobimg = binary.clone();
    for(int i=0;i<binary.rows; i++){
        for(int j=0;j<binary.cols;j++){
            if(visited[i][j]==blobno){
                blobimg.at<uchar>(i,j) = 255;
            }
            else{
                blobimg.at<uchar>(i,j) = 0;
            }
        }
    }
    cv::namedWindow("blob");
    cv::imshow("blob", blobimg);
    cv::waitKey(0);
}

int getColor(int b,int g,int r){
  int tol=60;
  if(b<50 && g <50 && r <50)
    return 0;
  if(((b>77-tol) && (b<77+tol)) && ((g >177-tol) && (g<177+tol)) && ((r <35+tol) && (r>35-tol)))
    return 0;
  if(((b>0-tol) && (b<0+tol)) && ((g >242-tol) && (g<242+tol)) && ((r <254+tol) && (r>254-tol)))
    return 0;
  if(((b>254-tol) && (b<254+tol)) && ((g >0-tol) && (g<0+tol)) && ((r <253+tol) && (r>253-tol)))
    return 0;
  if(((b>36-tol) && (b<36+tol)) && ((g >27-tol) && (g<27+tol)) && ((r <237+tol) && (r>237-tol)))
    return 0;
  if(((b>204-tol) && (b<204+tol)) && ((g >71-tol) && (g<71+tol)) && ((r <63+tol) && (r>63-tol)))
    return 0;
  if(((b>250-tol) && (b<250+tol)) && ((g >250-tol) && (g<250+tol)) && ((r <0+tol) && (r>0-tol)))
    return 0;
  if(b>230 && g>230 && b>230)return 0;
  return 1;
}

float verifyCircle(cv::Mat dt, cv::Point2f center, float radius, std::vector<cv::Point2f> & inlierSet)
{
 unsigned int counter = 0;
 unsigned int inlier = 0;
 float minInlierDist = 2.0f;
 float maxInlierDistMax = 100.0f;
 float maxInlierDist = radius/25.0f;
 if(maxInlierDist<minInlierDist) maxInlierDist = minInlierDist;
 if(maxInlierDist>maxInlierDistMax) maxInlierDist = maxInlierDistMax;

 // choose samples along the circle and count inlier percentage
 for(float t =0; t<2*3.14159265359f; t+= 0.05f)
 {
     counter++;
     float cX = radius*cos(t) + center.x;
     float cY = radius*sin(t) + center.y;

     if(cX < dt.cols)
     if(cX >= 0)
     if(cY < dt.rows)
     if(cY >= 0)
     if(dt.at<float>(cY,cX) < maxInlierDist)
     {
        inlier++;
        inlierSet.push_back(cv::Point2f(cX,cY));
     }
 }

 return (float)inlier/float(counter);
}

inline void getCircle(cv::Point2f& p1,cv::Point2f& p2,cv::Point2f& p3, cv::Point2f& center, float& radius)
{
  float x1 = p1.x;
  float x2 = p2.x;
  float x3 = p3.x;

  float y1 = p1.y;
  float y2 = p2.y;
  float y3 = p3.y;

  // PLEASE CHECK FOR TYPOS IN THE FORMULA :)
  center.x = (x1*x1+y1*y1)*(y2-y3) + (x2*x2+y2*y2)*(y3-y1) + (x3*x3+y3*y3)*(y1-y2);
  center.x /= ( 2*(x1*(y2-y3) - y1*(x2-x3) + x2*y3 - x3*y2) );

  center.y = (x1*x1 + y1*y1)*(x3-x2) + (x2*x2+y2*y2)*(x1-x3) + (x3*x3 + y3*y3)*(x2-x1);
  center.y /= ( 2*(x1*(y2-y3) - y1*(x2-x3) + x2*y3 - x3*y2) );

  radius = sqrt((center.x-x1)*(center.x-x1) + (center.y-y1)*(center.y-y1));
}

std::vector<cv::Point2f> getPointPositions(cv::Mat binaryImage)
{
 std::vector<cv::Point2f> pointPositions;

 for(unsigned int y=0; y<binaryImage.rows; ++y)
 {
     //unsigned char* rowPtr = binaryImage.ptr<unsigned char>(y);
     for(unsigned int x=0; x<binaryImage.cols; ++x)
     {
         //if(rowPtr[x] > 0) pointPositions.push_back(cv::Point2i(x,y));
         if(binaryImage.at<uchar>(y,x) > 0) pointPositions.push_back(cv::Point2f(x,y));
     }
 }

 return pointPositions;
}

void removebackground(cv::Mat img, cv::Mat binary){
    for(int i=1;i<img.rows;i++){
        for(int j=1;j<img.cols;j++){

            int a = img.at<cv::Vec3b>(i,j)[0];
            int b = img.at<cv::Vec3b>(i,j)[1];
            int c = img.at<cv::Vec3b>(i,j)[2];
            
            int flag = getColor(a,b,c);
            if(flag==0){
                binary.at<uchar>(i,j) = 0;
            }
            else{
                binary.at<uchar>(i,j) = 255;
            }
        }
    }
}

void getblobimage(cv::Mat blobimg, cv::Mat binary, int** visited, int blobno){
    for(int i=0;i<binary.rows; i++){
        for(int j=0;j<binary.cols;j++){
            if(visited[i][j]==blobno){
                blobimg.at<uchar>(i,j) = 255;
            }
            else{
                blobimg.at<uchar>(i,j) = 0;
            }
        }
    }
}

pair < int, pair<int,int> > get_center_radius(cv::Mat canny, cv::Mat img){

    std::vector<cv::Point2f> edgePositions;

    edgePositions = getPointPositions(canny);


    // create distance transform to efficiently evaluate distance to nearest edge
    cv::Mat dt;
    cv::distanceTransform(255-canny, dt,CV_DIST_L1, 3);

    //TODO: maybe seed random variable for real random numbers.

    unsigned int nIterations = 0;

    cv::Point2f bestCircleCenter;
    vector< pair< cv::Point2f ,  int> > circletemp;
    float bestCircleRadius;
    float bestCirclePercentage = 0;
    float minRadius = 8;   // TODO: ADJUST THIS PARAMETER TO YOUR NEEDS, otherwise smaller circles wont be detected or "small noise circles" will have a high percentage of completion

    //float minCirclePercentage = 0.2f;
    float minCirclePercentage = 0.10f;  // at least 5% of a circle must be present? maybe more...

    int maxNrOfIterations = edgePositions.size();   // TODO: adjust this parameter or include some real ransac criteria with inlier/outlier percentages to decide when to stop

    int max_count=0;
    //vector<int> radiustemp;
    for(unsigned int its=0; its< maxNrOfIterations; ++its)
    {
        //RANSAC: randomly choose 3 point and create a circle:
        //TODO: choose randomly but more intelligent, 
        //so that it is more likely to choose three points of a circle. 
        //For example if there are many small circles, it is unlikely to randomly choose 3 points of the same circle.
        unsigned int idx1 = rand()%edgePositions.size();
        unsigned int idx2 = rand()%edgePositions.size();
        unsigned int idx3 = rand()%edgePositions.size();

        // we need 3 different samples:
        if(idx1 == idx2) continue;
        if(idx1 == idx3) continue;
        if(idx3 == idx2) continue;

        // create circle from 3 points:
        cv::Point2f center; float radius;
        getCircle(edgePositions[idx1],edgePositions[idx2],edgePositions[idx3],center,radius);

        // inlier set unused at the moment but could be used to approximate a (more robust) circle from alle inlier
        std::vector<cv::Point2f> inlierSet;

        //verify or falsify the circle by inlier counting:
        float cPerc = verifyCircle(dt,center,radius, inlierSet);

        // update best circle information if necessary
        if(cPerc >= bestCirclePercentage)
            if(radius >= minRadius)
        {
            
            circletemp.push_back(make_pair(center, radius));

            bestCirclePercentage = cPerc;
            bestCircleRadius = radius;
            bestCircleCenter = center;
            //cv::circle(img, bestCircleCenter,bestCircleRadius, cv::Scalar(rand()%255,rand()%255,rand()%255),1);

        }
    }
 
    sort(circletemp.begin(), circletemp.end(), sort_pred());
    int m = circletemp.size()/2;
    
    cv::circle(img, circletemp[m].first, circletemp[m].second, cv::Scalar(255,255,0),1);
    cv::namedWindow("image_temp");
    cv::imshow("image_temp",img);
    cv::waitKey(0);

    cout << "\n" << circletemp[m].second << " " << circletemp[m].first.x;

    std::pair< int , std::pair<int,int> > v;

    v = make_pair(circletemp[m].second, make_pair(circletemp[m].first.x, circletemp[m].first.y));
    return v;
}

int main(){

    int t=108;
  while(t<=490){
/*
    std::string filename;
    filename = std::to_string(t) + ext;
    char* temp = new char[filename.length() + 1];
    strcpy(temp,filename.c_str());
    cv::Mat img = cv::imread(temp);
    delete temp;
*/
    cv::Mat img = cv::imread("image.jpg");
    if(img.empty())exit(1);
    cv::namedWindow("image",0);
    cv::imshow("image",img);
    cv::waitKey(0);

    cv::Mat gray,canny;
    cv::cvtColor(img, gray, CV_BGR2GRAY);
    cv::Mat binary(img.rows, img.cols, CV_8UC1);
    int count=0, **visited;
    node centroid[20];

    //remove un-necessary background portions of the image
    removebackground(img, binary);

    //performs blob-detection
    visited = blobDetection(binary, &count);
    cout<<"COunt "<<count<<endl;

    for(int i=1;i<=count;i++){
        centroid[i] = findcentroid(binary, i, visited);
        showblob(binary, visited, i);
    }

   // bool flag = false;
    bool flag = true ;
    int marked_blob=2;
    pair <int ,int > spyPos;
  //  spyPos = make_pair(centroid[1].x,centroid[1].y);
    spyPos = make_pair(447,336) ;
   // cout << endl << binary.rows << " " << binary.cols;
    cout << centroid[2].x << " " << centroid[2].y;
    cv::imshow("blob",binary);
    cv::waitKey(0);
    /*
    for(int i=spyPos.first-20;i<img.rows && i<spyPos.first+20 && flag==false;i++){
        for(int j=spyPos.second-20;j<img.cols && j<spyPos.second+20 && flag==false;j++){
            if(binary.at<uchar>(i,j)==255){
                    flag = true;
                    marked_blob = visited[i][j];
            }
        }
    }
    */

    if(flag==false){
        cout << "No circle found";
    }
    else{
        //seperates the blob-image from the binary image
        cv::Mat blobimg = binary.clone();
        getblobimage(blobimg, binary, visited, marked_blob);
    
        /*cv::namedWindow("blob");
        cv::imshow("blob", blobimg);
        cv::waitKey(0);*/

        cv::Canny(blobimg, canny, 200, 20);

        for(int i=1;i<canny.rows-1;i++){
            for(int j=1;j<canny.cols-1;j++){

                for(int m=i-1;m<i+1;m++){
                    for(int n=j-1;n<j+1;n++){
                        if(img.at<cv::Vec3b>(m,n)[0] < 50 && img.at<cv::Vec3b>(m,n)[1] < 50 && img.at<cv::Vec3b>(m,n)[2] < 50){
                            canny.at<uchar>(i,j) = 0;
                        }
                    }
                }
            }
        }
        /*cv::imshow("image",canny);
        cv::waitKey(0);*/

       // std::pair< int , std::pair<int,int> > v;
        //v = get_center_radius(canny, img);
        std::vector<cv::Point2f> edgePositions;

    edgePositions = getPointPositions(canny);


    // create distance transform to efficiently evaluate distance to nearest edge
    cv::Mat dt;
    cv::distanceTransform(255-canny, dt,CV_DIST_L1, 3);

    //TODO: maybe seed random variable for real random numbers.

    unsigned int nIterations = 0;

    cv::Point2f bestCircleCenter;
    vector< pair< cv::Point2f ,  int> > circletemp;
    float bestCircleRadius;
    float bestCirclePercentage = 0;
    float minRadius = 8;   // TODO: ADJUST THIS PARAMETER TO YOUR NEEDS, otherwise smaller circles wont be detected or "small noise circles" will have a high percentage of completion

    //float minCirclePercentage = 0.2f;
    float minCirclePercentage = 0.10f;  // at least 5% of a circle must be present? maybe more...

    int maxNrOfIterations = edgePositions.size();   // TODO: adjust this parameter or include some real ransac criteria with inlier/outlier percentages to decide when to stop

    int max_count=0;bool circle_detected=false;
    //vector<int> radiustemp;
    for(unsigned int its=0; its< maxNrOfIterations; ++its)
    {
        //RANSAC: randomly choose 3 point and create a circle:
        //TODO: choose randomly but more intelligent, 
        //so that it is more likely to choose three points of a circle. 
        //For example if there are many small circles, it is unlikely to randomly choose 3 points of the same circle.
        unsigned int idx1 = rand()%edgePositions.size();
        unsigned int idx2 = rand()%edgePositions.size();
        unsigned int idx3 = rand()%edgePositions.size();

        // we need 3 different samples:
        if(idx1 == idx2) continue;
        if(idx1 == idx3) continue;
        if(idx3 == idx2) continue;

        // create circle from 3 points:
        cv::Point2f center; float radius;
        getCircle(edgePositions[idx1],edgePositions[idx2],edgePositions[idx3],center,radius);

        // inlier set unused at the moment but could be used to approximate a (more robust) circle from alle inlier
        std::vector<cv::Point2f> inlierSet;

        //verify or falsify the circle by inlier counting:
        float cPerc = verifyCircle(dt,center,radius, inlierSet);

        circle_detected = true;
        // update best circle information if necessary
        if(cPerc >= bestCirclePercentage)
            if(radius >= minRadius)
        {
            
            circletemp.push_back(make_pair(center, radius));

            bestCirclePercentage = cPerc;
            bestCircleRadius = radius;
            bestCircleCenter = center;
            //cv::circle(img, bestCircleCenter,bestCircleRadius, cv::Scalar(rand()%255,rand()%255,rand()%255),1);

        }
    }

    if(circle_detected==false || circletemp.empty()){
        cout<<"No Circle Found";
    }
    else{
        sort(circletemp.begin(), circletemp.end(), sort_pred());
        int m = circletemp.size()/2;
    
        std::cout << circletemp[m].first.x << circletemp[m].second;
        //cv::circle(img, circletemp[m].first, circletemp[m].second, cv::Scalar(255,255,0),1);
        cv::namedWindow("image_temp");
        cv::imshow("image_temp",img);
        cv::waitKey(0);

        cout << "\n" << circletemp[m].second << " " << circletemp[m].first.x;

        std::pair< int , std::pair<int,int> > v;

        v = make_pair(circletemp[m].second, make_pair(circletemp[m].first.x, circletemp[m].first.y));
    }
    }
        cv::imshow("image",img);
        cv::waitKey(0);
  }

}
