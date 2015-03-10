#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <unistd.h>
#include <opencv2/highgui/highgui.hpp>
#include <utility>
#include <fstream>
#include <map>
#include <vector>
#include <queue>


#define mk make_pair
#define pb push_back
#define pii pair<int, int>
#define resource_to_radius_ratio 250 
#define discover_radius 20
#define PI 3.14
#define ll long long int
#define pii pair<int, int>

// uncovered area around army = 1.5*ourRadius
// spy is abandones if the distance between its centre and enemy centre <= 20 
// visibility radius is 20 
// if our resource is less tha 10 .. quit over

using namespace std;
using namespace cv;

typedef struct enemy{
  int centre_x;
  int centre_y;
  double radius;
}enemy;
//global variables :
template< typename FirstType, typename SecondType >
struct PairComparator {
  bool operator()( const pair<FirstType, SecondType>& p1, const pair<FirstType, SecondType>& p2 ) const 
    {  if( p1.first < p2.first ) return true;
        if( p2.first < p1.first ) return false;
        //return p1.second < p2.second;
    }
};

typedef pair < int , pair <int, int> > P;
struct Order
{
     bool operator()(const P &a,const P &b) const
    {
        return a.first > b.first  ; //&& a.second.first < b.first.first;
    }
};

priority_queue<pair<int,pair<int,int> >, vector<pair<int,pair<int,int> > >, Order> q,r;

pair <int ,int > spyPos , armyPos , initialPos ; // first is vertically down distance and second is left side distance
string moveSpy(int r , int c) {
   string ans ;
   printf("MOVE SPY\n %d %d\n",r,c) ;
 	cin>>ans;
   return ans ;
}

void moveArmy(int r , int c) {
	string ans ;
	printf("MOVE ARMY\n %d %d\n",r,c) ;
	cin>>ans;
 //  return ans ;
}
void abandonSpy() {
	string ans ;
    printf("ABANDON SPY\n");
	cin>>ans;
  // return ans ;
}
void assignSpy() {
	string ans ;
	printf("ASSIGN SPY\n");
	cin>>ans;
 //  return ans ;
}

Mat snapShot(pii point){
	//moveSpy(point.first, point.second);
	std::stringstream ss;
	ss << "/proc/" << getpid() << "/fd/0";
	Mat m = cv::imread(ss.str());
	return m;
}

bool isSafe(int r , int c) {
	if((r>=0&&r<=600)&&(c>=0&&c<=800))
		return true ;
	return false ;
}

// 0- black
// 1- green
// 2- yellwo
// 3- cyan
// 4- red
// 5 -blue;
// 6 -magenta
int getColor(int b,int g,int r){
  int tol=10,b,g,r;
  if(b<25 && g <25 && r <25)
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
  return 1;
}
/*
Mat filterImage(Mat image){
  int a[7], ans, b,g,r;
  Mat img=image.clone();
  for(int i=2;i <image.rows-3 ; i++){
    for(int j=2;j<image.cols-3 ;j++){
        for(int i=; i < 7;i++)
          a[i]=0;
        for(int l=-2; i <3 ;l++){
          for(int k=-2; k<3 ;k++){
              b = image.at<cv::Vec3b>(i+l,j+k)[0];
              g = image.at<cv::Vec3b>(i+l,j+k)[1];
              r = image.at<cv::Vec3b>(i+l,j+k)[2];
              a[getColor(b,g,r)]++;
          }
        }        
        ans=0;
        max=0;
        for(int i=1; i < 7;i++){
          if(a[i]>max){
            max=a[i];
            ans=i;
          }
        }
    }
  }
  return img;
}
*/
vector<int> runMST(vector < pair <int, int > > points){
  ll **d;
  int s=points.size();
  d=(ll **)malloc(sizeof(ll *)*s);
  for(int i=0; i < s ;i++){
    d[i]=(ll *)malloc(sizeof(ll)*s);
  }
  map<pii, int > M; 
  int x=0;
  for(int i=0;i<s;i++){
    M.insert (make_pair(points[i], x/s) );
     for(int j=0;j<s;j++){
        d[x/s][x%s]= (ll)(sqrt((((points[i].first - points[j].first))*((points[i].first- points[j].first))) +(((points[i].second- points[j].second))*((points[i].second- points[j].second)))));
        x++;
      }   
  }
  int *parent=(int *)malloc(sizeof(int)*s);
  //0 for head

  ll *vis=(ll *)malloc(sizeof(ll)*s);
  x=0;
  for(int i=0;i<s;i++){
    if(i==0){
        parent[x]=0;
        vis[x]=0;
        r.push(make_pair(vis[x], make_pair(points[i].first,points[i].second)));
    }
    else{
      parent[x]=-1;
      vis[x]=999999999;
      q.push(make_pair(vis[x], make_pair(points[i].first,points[i].second)));
    }
    x++;
  }
  P node;
  vector<int> toRet;
  while(!q.empty()){
    node =q.top();
    int x=M.find(make_pair(node.second.first, node.second.second))->second; 
    toRet.push_back(x);
    for(int l=0;l<s;l++){
      if(points[l].first==node.second.first && points[l].second== node.second.second);
        continue;
      if(vis[l] > d[x][l])
      {
        vis[l]=d[x][l];
        parent[l]=x;
      }
    }
  }
  return toRet; 
}

void pathPlanner(int &numEnemy , int ourResource) {

//moving at 45

	string answer ;
 int direction =1;
 int stepSize = 40;  //40
  int safeDist = ourResource/resource_to_radius_ratio ;
  int dx , dy ;
  int pos_x , pos_y ;
 float  theta = 20*(PI/180) ;
 ofstream file1("output.txt");
   pos_x = spyPos.first ; pos_y = spyPos.second ;
  initialPos.first = spyPos.first ; initialPos.second = spyPos.second ;
  
 while((numEnemy)&&!((pos_y>800)&&(pos_x==0))) {  //execute while the right side is completely uncovered
  dx = stepSize*cos(theta) ; dy = stepSize*sin(theta) ;
  pos_x += direction*dx ; pos_y += direction*dy ;
 
 if(isSafe(pos_x,pos_y)) {
   	answer  = moveSpy(pos_x,pos_y);     
 	    
 	    file1<<"("<<pos_y<<" "<<pos_x<<") , " ; // comment later
    
        
   if(answer=="DONE") {
   	puts("SNAPSHOT\n"); cin>>answer;
   	 spyPos.first = pos_x ; spyPos.second = pos_y ;
    //check the snapshot ..if enemy radius < ..attack
   }
   else {
   puts("SNAPSHOT\n"); cin>>answer ;
   numEnemy--;
    assignSpy() ;
          moveArmy(pos_x,pos_y) ;
         /*  spyPos.second  =	pos_y = pos_y*0.85 - stepSize*sin(theta) ;//+ (pos_x)*tan(theta) + stepSize ;
       	 	spyPos.first =  pos_x =  - stepSize*cos(theta)  ;
       	 	direction = abs(direction) ;*/
       	 	spyPos.second  =	pos_y = pos_y + direction*safeDist*cos(theta) ;
       	 	spyPos.first   =    pos_x =  pos_x + direction*safeDist*sin(theta) ;

         file1<<endl<<"NEW SPY ASSIGNED"<<"  "<<pos_y<<" "<<pos_x<<endl ;

   }
 	
 	//write what to do : return to main
 	//check the snapshot ..if enemy radius < ..attack
}
else 
 {
       if(((pos_x>600)||(pos_x<0))) {
       	 if(pos_x>600) {
       	 	pos_x -= direction*dx ; pos_y -= direction*dy ; // movung a step back 
       	 	pos_y = pos_y + (pos_x-600)*tan(theta) + 1.5*stepSize ;
       	 	pos_x = 600 ;   //formly it was just 600 
       	 
       	    direction=-1*direction ;
       	 }
       	 else if(pos_x<0) {
       	 	pos_x -= direction*dx ; pos_y -= direction*dy ;  //moving a step back 
       	 	pos_y = pos_y + (pos_x)*tan(theta) + stepSize ;
       	 	pos_x = 0  ;
       	 	direction=-1*direction ;
       	 }
  
        }	

  }

 }
 abandonSpy();  //no need to do this .. can be done without that
 assignSpy();
 //
 // theta = theta/2 ;  //can uncomment 
 //option 1 :
 pos_x = 600 ; pos_y = initialPos.second*2 ;
 direction = -1 ;
 //option 2 : 
 //pos_x = initialPos.first ; pos_y = initialPos.second ;
 //direction = 1 ;
 while((numEnemy)&&!((pos_y<0)&&(pos_x==600))) {  //execute while the right side is completely uncovered

  dx = stepSize*cos(theta) ; dy = stepSize*sin(theta) ;
  pos_x += direction*dx ; pos_y += direction*dy ;
 
 if(isSafe(pos_x,pos_y)) {
   	answer  = moveSpy(pos_x,pos_y);     
 	    
 	    file1<<"("<<pos_y<<" "<<pos_x<<") , " ; // comment later
    
        
   if(answer=="DONE") {
   	puts("SNAPSHOT\n"); cin>>answer;
   	 spyPos.first = pos_x ; spyPos.second = pos_y ;
    //check the snapshot ..if enemy radius < ..attack
   }
   else {
   	numEnemy--;
   puts("SNAPSHOT\n"); cin>>answer ;
    assignSpy() ;
        moveArmy(pos_x,pos_y) ;
            spyPos.second  =	pos_y = pos_y*1.5+ stepSize*sin(theta) ;//+ (pos_x)*tan(theta) + stepSize ;
       	 	spyPos.first =  pos_x =  581 + stepSize*cos(theta)  ;
       	 	direction = -1*abs(direction) ;
         file1<<endl<<"NEW SPY ASSIGNED"<<"  "<<pos_y<<" "<<pos_x<<endl ;

   }
 	
 	//write what to do : return to main
 	//check the snapshot ..if enemy radius < ..attack
 }
 else 
 {
       if(((pos_x>600)||(pos_x<0))/*&&(poy_y<800)*/) {
       	 if(pos_x>600) {
       	 	pos_x -= direction*dx ; pos_y -= direction*dy ; // movung a step back 
       	 	pos_y = pos_y - (pos_x-600)*tan(theta) - 1.5*stepSize ;
       	 	pos_x = 600 ;   //formly it was just 600 
       	 
       	    direction=-1*direction ;
       	 }
       	 else if(pos_x<0) {
       	 	pos_x -= direction*dx ; pos_y -= direction*dy ;  //moving a step back 
       	 	pos_y = pos_y - (pos_x)*tan(theta) - stepSize ;
       	 	pos_x = 0  ;
       	 	direction=-1*direction ;
       	 }
  
        }	

  }

 }
 abandonSpy();
 puts("SNAPSHOT\n"); cin>>answer;
}
int main() {
    Mat image=imread("/home/Documents/Sudocode_new/sudocode/download.jpg",CV_LOAD_IMAGE_COLOR);
  int i=100, j=100;
  int b = image.at<cv::Vec3b>(i,j)[0];
  int g = image.at<cv::Vec3b>(i,j)[1];
  int r = image.at<cv::Vec3b>(i,j)[2];
  cout << b << " " << g << " " << r << endl;
  return 0;

	int ourResource, numEnemy, costAssignSpy;
    float travelCostSpy, travelCostArmy ;
    
      scanf("%d",&ourResource);
      scanf("%d",&numEnemy);
      scanf("%d",&costAssignSpy);
      scanf("%f%f",&travelCostSpy,&travelCostArmy) ;
    

	int **grid= (int **)malloc(sizeof(int)*600);
	for(int i=0;i<600;i++){
		grid[i]=(int *)malloc(sizeof(int)*800);
		for(int j=0;j<800;j++)
			grid[i][j]= -99999;
	}
  
    // get snapshot and using the function find the centre pixel of army and update it in the armyPos pair 
       armyPos.first = 3 ; armyPos.second = 180 ;   
       spyPos.first = armyPos.first ; spyPos.second = armyPos.second;
       assignSpy();
       string ans ;
       puts("SNAPSHOT\n"); cin>>ans;
       pathPlanner(numEnemy,ourResource);
    /*
	image= snapShot(mk(2,2));
	imshow("opencvtest",image);
    waitKey(0);
     */
	return 0;	
 
  }