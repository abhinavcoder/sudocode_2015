/*Sudocode 2015
Code for Raid I
Team Name- ****** 
Team Members- Abhinav Agarwalla, Kumar Abhinav, Arnav Jain
*/

#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <strings.h>
#include <stdlib.h>
#include <queue>
#include <math.h>


#define pb push_back
#define mk make_pair
#define linear           //linear / chessboard / spiral
using namespace std;

/* isSafe function takes 3 parameters .
	The first are the coordinates of the point 
	and the 3rd is the grid size.
	It retuns true is the coordinate point is 
	in the jungle.
*/
int counter = 0 ;
int start = 0 , maxy = 0 ;

queue < pair<int , int> > spiralQueue ;

int even=0;
bool isSafe(int row, int col, int **grid,int n){
	// n denotes the grid size

	if((row>=0)&&(row<n)&&(col>=0)&&(col<n)&&(grid[row][col]==-1))
		return true ;
	else
		return false ;
}

/* getAllWeakPoints function gets all the weak points of a bunker
	if one weak point coordinate is provided to the function as
	all the the weakpoints are adjacent. 
	We use a queue of a pair , pair <int , int > represents the coordinate
	of the grid. 
	pair.first -> gives the x coordinate
	pair.second -> gives the y coordinate
	We will use Breadth First-Search to get all weak points.

1. we have to change the bfs technique : since we know the allowed pattern thus why waste query 
    example if we know that the grid is moving linear then why check it in the diagonal direction 
2. using function for pushing and poping
3. searching for the square or rectangle pattern should be optimised
*/

// the sides of the rectangle is considered as general horizontal / vertical search until a corner is achieved to trigger the condition
string getQuery(int queryX , int queryY)
{
	string ans ;
	cout<<queryX<<" "<<queryY<<endl;
	fflush(stdout);
    cin>>ans;
    return ans ;
}

pair<int, int> getFirstWeakPoint(int **grid, int gridSize) //using listing 
{  
	int n = gridSize ;
	string answer;

	#ifdef chessboard
	int e = 2 ;
	while(e--){
	  	for(int i = 0 ; i < gridSize ; i++) {
  			for(int j = 0  ; j < gridSize ; j++) {
  		//cout << i << " " << j << endl;
  			if(grid[i][j]==-1 && (i+j + e )%2==1)
  			{   
  				answer = getQuery(i,j);
  				if(answer=="YES")
  				     { counter++; start = i ;
  				     	 return mk(i,j);}
  		    else
            	  grid[i][j] = 0 ; 
  			}
      }
    }
   }

   #endif
  #ifdef linear
  for(int i = 0  ; i < gridSize ; i++) {
  	for(int j = 0  ; j < gridSize ; j++) {
  		//cout << i << " " << j << endl;
  		if(grid[i][j]==-1)
  		{   
  			answer = getQuery(i,j);
  			if(answer=="YES")
  		     { counter++;return mk(i,j);}
  		    else
              grid[i][j] = 0 ;
  		}
      
  		else
  			continue ;
  		
  		if(grid[gridSize-i-1][gridSize-j-1] ==-1)
  		{
  			answer = getQuery(gridSize-i-1,gridSize-j-1);
  			if(answer=="YES")
  		     return mk(gridSize-i-1,gridSize-j-1);
  		    else
              grid[gridSize-i-1][gridSize-j-1] = 0 ;
  		}
  	}
  }
     #endif
    
  	 #ifdef spiral   //incomplete
  		int centre = n/2 ;
  		static int k = -1 ;
  		while(k<=n/2+1) {
  		  if(spiralQueue.size()==0){
  		  	k++ ;
  		    for(int i = -k;i<=k;i++){
  		        for(int j=-k;j<=k;j++){
  				  if(isSafe(centre+i,centre+j,grid,n)&&(abs(i)==k||abs(j)==k)) {
  					spiralQueue.push(mk(centre+i,centre+j)) ;
  			   	  }
  			    }
  		    }  
  		  }
  		  while(spiralQueue.size()!=0) {
  		  	pair <int ,int> spiralPoint ;
  		  	spiralPoint = spiralQueue.front();
  		  	spiralQueue.pop();
  		  	if(isSafe(spiralPoint.first,spiralPoint.second,grid,n)) {
  		  		answer = getQuery(spiralPoint.first,spiralPoint.second);
  		  		if(answer=="YES")
  		  			return mk(spiralPoint.first,spiralPoint.second);
  		  		else
  		  			grid[spiralPoint.first][spiralPoint.second] = 0 ;
  		  	}
  		  }
  	    }
  	 #endif
        
// cout<<endl<<"Mission accomplished ";
 return mk(0,0);
     
}

void getAllWeakPoints(int **grid, pair<int, int> weakPoints, int gridSize){
	cout<<endl<<"searching for the pattern"<<endl;
	grid[weakPoints.first][weakPoints.second]=1;
	queue < pair <int , int > > doubtPoints , cornerPoints;
	pair< int, int > point, adjPoint;
	int firstcond=0;
	doubtPoints.push(weakPoints);
	string answer;
    int destx = 0 , desty =0 ;
	while(!doubtPoints.empty()){
		point= doubtPoints.front();
//		cout<<"top point : "<<point.first<<" "<<point.second<<endl;
		doubtPoints.pop();
		//cout<<"condition :::: "<<condition;
            for(int i=-1 ; i <=1;i++){
			for(int j=-1;j<=1;j++){
                adjPoint= mk(point.first +i, point.second +j);    //takes the kernel point around it 3*3
               if(i*j==0) { 
                if(isSafe(adjPoint.first, adjPoint.second,grid, gridSize)){
					//send query for coordinates x and y : adjPoint.first , adjPoint.second 
					//if(i*j==0&&firstcond==0&&doubtPoints.size()!=0)  //complete it ................................>>>>>>............>>>>>>>....................................
					answer = getQuery(adjPoint.first,adjPoint.second); 
					if(answer == "YES"){
						{ doubtPoints.push(adjPoint);
						 // if(adjPoint.second > maxy) maxy = adjPoint.second ;
						   counter++ ;
						}
						grid[adjPoint.first][adjPoint.second]=1;

					}
					else {
						grid[adjPoint.first][adjPoint.second]=0;
					}
				}
			   }
			}
		}   
    }
}

void getSymmetricPoints(int **grid, pair<int, int > firstWeakPoint, int fieldSize, int numberBunkers) {
    int x= firstWeakPoint.first;
    int y= firstWeakPoint.second;
    //int num=numberBunkers;
    int basex , basey ;
    string answer;
 //   if(x==y || x==(-y)){2w
    int counter = 2 ;
      while(counter--) {
        basex = x ; 
        basey = y ;
        y=fieldSize-1-y;
        cout<<endl<<"searching for y-reflection"<<endl;
       if(grid[basex][y]==-1){
        answer = getQuery(x,y);
        grid[basex][y]=0;
        if(answer=="YES"){
          numberBunkers++;
          grid[basex][y]=1;
            getAllWeakPoints(grid, mk(basex,y) ,fieldSize);
        }
      }
        x=fieldSize-1-x;       //changed
        cout<<endl<<"searching for x-reflection"<<endl;
       if(grid[x][basey]==-1) {
        grid[x][basey]=0;
        answer= getQuery(x,basey);
        if(answer=="YES"){
            numberBunkers++;
            grid[x][basey]=1;
            getAllWeakPoints(grid, mk(x,basey) ,fieldSize);
        }
      }
      
      cout<<endl<<"searching for both x-y reflection"<<endl;
       if(grid[x][y]==-1){
        answer = getQuery(x,y);
        grid[x][y]=0;
        if(answer=="YES"){
          numberBunkers++;
          grid[x][y]=1;
            getAllWeakPoints(grid, mk(x,y) ,fieldSize);
        }
      }
        x = firstWeakPoint.second ;
        y = firstWeakPoint.first ;


        cout<<endl<<"Searching for diagonal reflection"<<endl;

      }
    }

int main(){

/*  @fieldSize - size of the field i.e n.
	@totalBunkers - total number of bunkers i.e. m
	
	@grid - the 2D array of the field.
	-1 means we don't know anything about that position.
	0 means that cant be a weak point
	1 means it is a weak point
	i.e. YES was returned querying for that coordinate. */
	
	int fieldSize, totalBunkers;
//	cout<<"enter the fieldsize and totalBunkers"<<endl;
	cin >> fieldSize >> totalBunkers;
	int **grid;

	grid=(int **)malloc(sizeof(int *)*(fieldSize+2));

	for(int i=0 ; i<fieldSize+2 ; i++){
			grid[i]=(int *)malloc(sizeof(int)*(fieldSize+2));
		for(int j=0 ; j<fieldSize+2 ; j++)
			grid[i][j]=-1;
	}

	//testing 
  //  getAllWeakPoints(grid,mk(3,3),fieldSize) ;
  
	//@numberBunkers - represent the number of bunkers found till now 
int numberBunkers=0;

    pair<int, int> firstWeakPoint;
    while(numberBunkers != totalBunkers){ 
        firstWeakPoint = getFirstWeakPoint(grid,fieldSize);
        getAllWeakPoints(grid,firstWeakPoint,fieldSize);
        getSymmetricPoints(grid, firstWeakPoint, fieldSize, numberBunkers);
        numberBunkers++;
	}
    
//	cout<<"Mission accomplished";
	return 0;	
}