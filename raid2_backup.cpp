/*Sudocode 2015
Code for Raid II
Team Name - Peacecoders 
Team Members- Abhinav Agarwalla, Kumar Abhinav, Arnav Jain
*/


#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <unistd.h>
#include <opencv2/highgui/highgui.hpp>
#include <utility>
#define mk make_pair
#define pb push_back
#define pii pair<int, int>
#define resource_to_radius_ratio 250 
#define discover_radius 20
#define PI 3.14

// uncovered area around army = 1.5*ourRadius
// spy is abandones if the distance between its centre and enemy centre <= 20 
// visibility radius is 20 
// if our resource is less tha 10 .. quit over


using namespace std;
using namespace cv;

//global variables :
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


void pathPlanner(int &numEnemy , int ourResource) {

//moving at 45

	string answer ;
 int direction =1;
 int stepSize = 40;  //40
  int safeDist = ourResource/resource_to_radius_ratio ;
  int dx , dy ;
  int pos_x , pos_y ;
 float  theta = 20*(PI/180) ;
// ofstream file1("output.txt");
   pos_x = spyPos.first ; pos_y = spyPos.second ;
  initialPos.first = spyPos.first ; initialPos.second = spyPos.second ;
  
 while((numEnemy)&&!((pos_y>800)&&(pos_x==0))) {  //execute while the right side is completely uncovered
  dx = stepSize*cos(theta) ; dy = stepSize*sin(theta) ;
  pos_x += direction*dx ; pos_y += direction*dy ;
 
 if(isSafe(pos_x,pos_y)) {
   	answer  = moveSpy(pos_x,pos_y);     
 	    
 	    //file1<<"("<<pos_y<<" "<<pos_x<<") , " ; // comment later
    
        
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

      //   file1<<endl<<"NEW SPY ASSIGNED"<<"  "<<pos_y<<" "<<pos_x<<endl ;

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
 	    
 	  //  file1<<"("<<pos_y<<" "<<pos_x<<") , " ; // comment later
    
        
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
        // file1<<endl<<"NEW SPY ASSIGNED"<<"  "<<pos_y<<" "<<pos_x<<endl ;

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
	//Mat image=imread("/home/Documents/Sudocode_new/sudocode/download.jpg",CV_LOAD_IMAGE_COLOR);
  setvbuf(stdout, NULL, _IONBF, 0);
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
