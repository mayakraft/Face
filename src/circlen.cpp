#include <iostream>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <sys/time.h>
#include <ctime>

#include "ofMain.h"

#define sqr(a) (a)*(a)
#define DEBUG if(0)
#define INF 1E+300
using namespace std;

typedef unsigned long long timestamp_t;

static timestamp_t
get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}


class encircle
{
public:
    encircle(){;}
    encircle(double xi, double yi, double ri)
    {
        r=ri;
        c.x=xi;
        c.y=yi;
    }
    encircle(ofPoint p, double ri)
    {
        r=ri;
        c.x=p.x;
        c.y=p.y;
    }
    
    encircle(ofPoint *pt1, ofPoint *pt2, ofPoint *pt3);
    
    bool IsPerpendicular(ofPoint *pt1, ofPoint *pt2, ofPoint *pt3);
    double CalcCircle(ofPoint *pt1, ofPoint *pt2, ofPoint *pt3);
    void show()
    {
//        cout<<"Center : ";c.show();
        cout<<"   Radius : "<<r<<endl;
        
    }
    
    ofPoint c;
    double r;
    
};
double length(ofPoint a, ofPoint b);

void randomize ( vector <ofPoint> arr, int n )
{
    // Use a different seed value so that we don't get same
    // result each time we run this program
    srand ( time(NULL) );
    ofPoint temp;
    // Start from the last element and swap one by one. We don't
    // need to run for the first element that's why i > 0
    for (int i = n-1; i > 0; i--)
    {
        // Pick a random index from 0 to i
        int j = rand() % (i+1);
        
        // Swap arr[i] with the element at random index
        
        temp.x=arr[i].x;
        temp.y=arr[i].y;
        arr[i].x=arr[j].x;
        arr[i].y=arr[j].y;
        arr[j].x=temp.x;
        arr[j].y=temp.y;
    }
}


bool encircle::IsPerpendicular(ofPoint *pt1, ofPoint *pt2, ofPoint *pt3)

{
    double yDelta_a= pt2->y - pt1->y;
    double xDelta_a= pt2->x - pt1->x;
    double yDelta_b= pt3->y - pt2->y;
    double xDelta_b= pt3->x - pt2->x;
    
    
    
    // checking whether the line of the two pts are vertical
    if (fabs(xDelta_a) <= 0.000000001 && fabs(yDelta_b) <= 0.000000001){
        return false;
    }
    
    if (fabs(yDelta_a) <= 0.0000001)
    {
        return true;
    }
    else if (fabs(yDelta_b) <= 0.0000001)
    {
        return true;
    }
    else if (fabs(xDelta_a)<= 0.000000001)
    {
        return true;
    }
    else if (fabs(xDelta_b)<= 0.000000001)
    {
        return true;
    }
    else return false ;
}

encircle::encircle(ofPoint *pt1, ofPoint *pt2, ofPoint *pt3)
{
    r=-1;		// error checking
    c.x=0;
    c.y=0;
    
    if (!this->IsPerpendicular(pt1, pt2, pt3) )				this->CalcCircle(pt1, pt2, pt3);
    else if (!this->IsPerpendicular(pt1, pt3, pt2) )		this->CalcCircle(pt1, pt3, pt2);
    else if (!this->IsPerpendicular(pt2, pt1, pt3) )		this->CalcCircle(pt2, pt1, pt3);
    else if (!this->IsPerpendicular(pt2, pt3, pt1) )		this->CalcCircle(pt2, pt3, pt1);
    else if (!this->IsPerpendicular(pt3, pt2, pt1) )		this->CalcCircle(pt3, pt2, pt1);
    else if (!this->IsPerpendicular(pt3, pt1, pt2) )		this->CalcCircle(pt3, pt1, pt2);
    else
    {
        r=-1;
        return ;
    }
    
    
}


double encircle::CalcCircle(ofPoint *pt1, ofPoint *pt2, ofPoint *pt3)
{
    double yDelta_a= pt2->y - pt1->y;
    double xDelta_a= pt2->x - pt1->x;
    double yDelta_b= pt3->y - pt2->y;
    double xDelta_b= pt3->x - pt2->x;
    
    if (fabs(xDelta_a) <= 0.000000001 && fabs(yDelta_b) <= 0.000000001)
    {
        c.x= 0.5*(pt2->x + pt3->x);
        c.y= 0.5*(pt1->y + pt2->y);
        r= length(c,*pt1);		// calc. radius
        return this->r;
    }
    
    // IsPerpendicular() assure that xDelta(s) are not zero
    double aSlope=yDelta_a/xDelta_a; //
    double bSlope=yDelta_b/xDelta_b;
    if (fabs(aSlope-bSlope) <= 0.000000001)
    {	// checking whether the given points are colinear.
        return -1;
    }
    
    // calc center
    c.x= (aSlope*bSlope*(pt1->y - pt3->y) + bSlope*(pt1->x + pt2 ->x)	- aSlope*(pt2->x+pt3->x) )/(2* (bSlope-aSlope) );
    c.y = -1*(c.x - (pt1->x+pt2->x)/2)/aSlope +  (pt1->y+pt2->y)/2;
    
    
    r= length(c,*pt1);		// calc. radius
    return r;
}


double length(ofPoint a, ofPoint b)
{
    return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));
}

encircle makeCircle (ofPoint a, ofPoint b)
{
    encircle out;
    out.r= length(a,b)/2;
    out.c.x = (a.x + b.x)/2;
    out.c.y	= (a.y + b.y)/2;
    return out;
}

encircle makeCircle(ofPoint a, ofPoint b, ofPoint c)
{
    encircle out(&a,&b,&c);
    return out;
}

bool inCircle(ofPoint pt, encircle cir)
{
    if (length(cir.c,pt)<=(cir.r + 0.000000001))
        return true;
    else
        return false;
    
}

//std::vector<ofPoint> v;
//int main()
//{
//    timestamp_t t0 = get_timestamp();
//    int n;
//    //cout<<"Enter number of points: (at least 2) \n";
//    cin>>n;
//    //cout<<"Enter the coordinates of all the points: \n";
//    ofPoint input;
//    for (int i=0;i<n;i++)
//    {
//        cin>>input.x>>input.y;
//        v.push_back(input);
//    }
//    cout<<endl;
//    DEBUG for (int i=0;i<n;i++)
//    {
////        v[i].show();
////        cout<<endl;
//    }
//    
//    encircle cir=makeCircle(v[0],v[1]);
//    DEBUG
//    {
//        cout <<"Circle with first two made. \n";
//        cir.show();
//        
//    }
//    for(int i=2;i<n;i++)
//    {
//        if (inCircle(v[i],cir))
//        {
//            DEBUG
//            {
//                cout<<"point "<<i<<" has been added.\n";
//                
//            }
//            continue;
//            
//        }
//        else
//        {
//            cir=makeCircle(v[0],v[i]);
//            DEBUG
//            {
//                cout<<"V[0] and v[i] and i being "<<i<<"  \n";
//                cir.show();
//                
//            }
//            for (int j=1;j<i;j++)
//            {
//                if (inCircle(v[j],cir))
//                    continue;
//                else
//                {
//                    double small=INF; ofPoint smallc;
//                    cir=makeCircle(v[j],v[i]);
//                    DEBUG
//                    {
//                        cout<<"v[j] and v[i] where j: "<<j<<"  and i: "<<i<<endl;
//                        cir.show();
//                        
//                    }
//                    for (int k=0;k<j;k++)
//                    {
//                        if (inCircle(v[k],cir))
//                            continue;
//                        else {cir=makeCircle(v[k],v[j],v[i]);}
//                    }
//                    
//                    
//                    DEBUG
//                    {
//                        cout<<"Small set : ";cir.show();
//                    }
//                }
//                
//            }
//            
//            
//        }
//        
//    }
//    
//    
////    cout<<"The circle has been found.\n The center : "; cir.c.show();
//    cout<<"   Radius: "<<cir.r<<endl;
//    
//    timestamp_t t1 = get_timestamp();
//    
//    double secs = (t1 - t0) / 1000000.0L;
//    
//    cout<<"Time taken:  "<<secs<<endl;
//}