#include <iostream>
#include <stdio.h>
#include <cstring>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
RNG rng(12345);

typedef struct node 
{ 
    int x; 
    int y; 
    struct node *next; 
}node;

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

int getstatus(cv::Mat img, cv::Mat binimg,int blobno,int **visited,node centroid[20]){

    cv::Mat blobimg = binimg.clone();
    cv::Mat outlineblobimg(binimg.rows, binimg.cols,CV_8UC3,0);
    int i,j,ncontours,mindist,maxdist;
    float distance,ratio;

    node centroidtemp;

    mindist=500;
    maxdist=0;
    int flag = false;
    int counter=0, size=0;
    for(int i=3;i<binimg.rows-3;i++){
        for(int j=3;j<binimg.cols-3;j++){
            if(visited[i][j]!=blobno){
                blobimg.at<uchar>(i,j)=0;
            }
            else{
                //check for a kernel, and if maximum are black, increment counter
                int countb=0;
                for(int m=i-3;m<i+3 && flag==false;m++){
                    for(int n=j-3;n<j+3 && flag==false;n++){
                        int r = img.at<cv::Vec3b>(m,n)[0];
                        int g = img.at<cv::Vec3b>(m,n)[1];
                        int b = img.at<cv::Vec3b>(m,n)[2];

                        if( r < 20 && g < 20 && b < 20){
                            countb++;
                        }

                    }
                }
                //cout << " " << countb << " ";
                if(countb >= 5){
                    counter++;
                    flag = true;
                }

                size++;
            }
        }
    }
 //   cout << "\n" << counter << "  " << size;
 //   cout << " " << (1.0*counter/size)*100.0;
    //cv::Canny(blobimg,outlineblobimg,100,200,3);
    //centroidtemp = centroid[blobno];



    if(!flag){

        std::vector<cv::Vec3f> circles;
        cv::HoughCircles(blobimg, circles, CV_HOUGH_GRADIENT,1,60,200,20,0,0);

        for( size_t i = 0; i < circles.size(); i++ ) 
        {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);

            std::vector<cv::Vec3f> circles2;
            cv::Mat roi = img(Range(centroid[blobno].x-radius-20,centroid[blobno].x+radius+20),
                Range(centroid[blobno].y-radius-20,centroid[blobno].y+radius+20));

            cv::Mat binaryROI(roi.rows, roi.cols, CV_8UC1, 0);
            cv::cvtColor( roi, binaryROI, CV_BGR2GRAY);


            cv::HoughCircles(binaryROI, circles2, CV_HOUGH_GRADIENT,1,60,200,20,0,0);

            cv::namedWindow("Region");
            cv::imshow("Region", roi);
            cv::waitKey(0);

            for(int j=0;j<circles2.size();j++){
             //   cout << circles2[j];
            
                Point center2(cvRound(circles2[j][0] + centroid[blobno].x-radius-20), 
                    cvRound(circles2[j][1] + centroid[blobno].y-radius-20));
                int radius2 = cvRound(circles2[j][2]);
                cv::circle( img, center2, 3, Scalar(0,255,255), -1);
                cv::circle( img, center2, radius, Scalar(0,0,255), 1 );
                cout<<" radius2 "<<radius2<<" ";
                Radius[i] = radius2;
            
            }  
        }

        cv::namedWindow("Blob Image",0);
        cv::imshow("Blob Image",blobimg);
        cv::waitKey(0);

        cv::namedWindow("Blob Image",0);
        cv::imshow("Blob Image",img);
        cv::waitKey(0);
        return 1;
    }
    else{
        return 0;
    }
    //cvDestroyWindow("Blob Image");
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

int main(){

    cv::Mat img = cv::imread("discovered.jpg");
    if(img.empty())exit(1);
    cv::namedWindow("image",0);
    cv::imshow("image",img);
    cv::waitKey(0);

    cv::Mat gray,canny, binary;
    int count=0, **visited;
    cv::cvtColor( img, gray, CV_BGR2GRAY );
    cv::Canny(gray, canny, 200, 20);
    //cv::threshold(gray, binary, 100, 255, THRESH_BINARY );
    cv::adaptiveThreshold( gray, binary, 255, ADAPTIVE_THRESH_MEAN_C, 
        THRESH_BINARY, 9, 0);
    visited = blobDetection(binary, &count);
    cout<<"COunt "<<count<<endl;

    cv::imshow("image",gray);
    cv::waitKey(0);

    cv::imshow("image",binary);
    cv::waitKey(0);

    node centroid[20];
    bool truth[20]={0};
    for(int i=1;i<=count;i++){
        centroid[i] = findcentroid(binary, i, visited);

   // std::cout << "Centroid Values - " << centroid[i].x << " " << centroid[i].y << endl;
        truth[i] = getstatus(img, binary, i, visited, centroid);
    if(truth[i]){
        cout << "Radius - " << Radius[i]<< " :: Centroid Values - " << centroid[i].x << " " << centroid[i].y << endl;;
    }
    }

    cv::imshow("image",gray);
    cv::waitKey(0);

    cv::imshow("image",binary);
    cv::waitKey(0);

    cv::imshow("image",canny);
    cv::waitKey(0);

    cv::imshow("image",img);
    cv::waitKey(0);

}
