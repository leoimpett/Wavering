#include <cmath>
#include <fstream>
#include <ctime>
#include <iostream>
//#include "./CImg-1.5.6/CImg.h" 
//#include "CImg.h"
#include "CImg-1.6.2/CImg.h"
#define pi 3.14159265359	
#define xsize 256
#define ysize 256
#define proxthresh 30
#define OSCPKT_OSTREAM_OUTPUT
#include "oscpkt/oscpkt.hh"
#include "oscpkt/udp.hh"
using std::cout;
using std::cerr;
using namespace oscpkt;
const int PORT_NUM = 9109;

			//2D WAVES 

using namespace std;
using namespace cimg_library; 	
float xdriver = 0;
float ydriver = 0;
float xdriver2 = 0;
float ydriver2 = 0;
float xdriver3 = 0;
float ydriver3 = 0;
float norm;
float norm2;
float norm3;
float xold = 100;
float yold = 105;
float xold2 = 75;
float yold2 = 135;
float xold3 = 175;
float yold3 = 108;
float xstart3, ystart3;
float xstart2, ystart2;
float xstart, ystart;
float omega1 = 0.15111; //0.44; // 440.0;
float omega2 = 0.2111; //0.49388; // 445.0;
float omega3 = 0.25111; //0.2616; // 704.0;
float value;
float contr1;
float contr1old;
float contr2;
float contr2old;
float contr3;
float contr3old;
float learn1 = 1.01;
float learn2 = 1.02;
float learn3 = 1.03;
	
	void wave1(float *a1, float *p1, float x, float y, float t){
		

		float tstart = 0;
		
		float beta = -0.01;
		
		float pointvelocity = 0.614563;
		norm = sqrt( (pow(xdriver,2) + pow(ydriver,2)) );
		if(norm == 0){
			norm = 1;
		}
		float xstep = xdriver/norm;
		float ystep = ydriver/norm;	
		xstart = xold + pointvelocity*(1-((xstep+2)/(pow(xstep,2)+1)));
		ystart = yold + pointvelocity*(1-((ystep+2)/(pow(ystep,2)+1)));		

		
		float radius = sqrt(pow((x-xstart),2) + pow((y-ystart),2));
		
		*a1 = exp(beta*radius)*500;
		*p1 = omega1*t + beta*radius;		
	}
	
	
	
	
		void wave2(float *a1, float *p1, float x, float y, float t){
		
		float pointvelocity = 0.14865;
		
		norm2 = sqrt(pow(xdriver2,2)+pow(ydriver2,2));
		if(norm2 == 0){
			norm2 = 1;
		}
		float xstep = xdriver2/norm2;
		float ystep = ydriver2/norm2;	
		xstart2 = xold2 + pointvelocity*(1-((xstep+2)/(pow(xstep,2)+1)));
		ystart2 = yold2 + pointvelocity*(1-((ystep+2)/(pow(ystep,2)+1)));			


		float tstart = 0;
		 
		float beta = -0.01;
		
	
		
		float radius = sqrt(pow((x-xstart2),2) + pow((y-ystart2),2));
		
		*a1 = exp(beta*radius)*500;
		*p1 = omega2*t + beta*radius;		
	}
	
	
	
		void wave3(float *a1, float *p1, float x, float y, float t){
		
		float pointvelocity = 1.31546;
		
		norm3 = sqrt(pow(xdriver3,2)+pow(ydriver3,2));
		if(norm3 == 0){
			norm3 = 1;
		}
		float xstep = xdriver3/norm3;
		float ystep = ydriver3/norm3;	
		xstart3 = xold3 + pointvelocity*(1-((xstep+2)/(pow(xstep,2)+1)));
		ystart3 = yold3 + pointvelocity*(1-((ystep+2)/(pow(ystep,2)+1)));			


		float beta = -0.01;
		
	
		
		float radius = sqrt(pow((x-xstart3),2) + pow((y-ystart3),2));
		
		*a1 = exp(beta*radius)*500;
		*p1 = omega3*t + beta*radius;		
	}	
	
	
	


int main() {
	
	
	
	cout << "HELLO WORLD" << endl;


	  UdpSocket sock;
  sock.connectTo("Jonathans-MacBook-Pro.local", PORT_NUM);
	
	float grid[xsize][ysize][3];
	
	float timet = 3;

	
	CImg<unsigned char> img(xsize,ysize,1,3);      

	  CImgDisplay main_disp(img,"Waves, V12!");

	cimg_forXYC(img,x,y,v) {
	img(x,y,0) = 255;
	img(x,y,1) = 255;
	img(x,y,2) = 255;
		
	}
	
	
	float amp1,amp2, amp3;
	float pha1,pha2, pha3;
	
		int interx = 0;
	int intery = 0;
	float biggestyet = 0;
	
	for(timet = 1; timet < 10000; timet= timet + 0.01){


//cout << "time " << timet << endl;


	 interx = 100;
	 intery = 100;
	 biggestyet = 0;

	for(int i = 0; i < xsize; i++){
		for (int j = 0; j < ysize; j++){
	
	//grid[i][j][1] = real(wave1(i,j,time)) + real(wave2(i,j,time)) + real(wave3(i,j,time));
	
	wave1(&amp1,&pha1,i,j,timet);
	wave2(&amp2,&pha2,i,j,timet);
	wave3(&amp3,&pha3,i,j,timet);
	

	
	value = (amp1 * cos(pha1)) + (amp2 * cos(pha2)) + (amp3 * cos(pha3)) - 0.1*((amp1) + (amp2) + (amp3));
	grid[i][j][1] = value;
	
	if (grid[i][j][1] >= biggestyet){
		biggestyet = value;
		interx = i;
		intery = j;
		
	contr1old = contr1;
	contr2old = contr2;
	contr3old = contr3;
	contr1 = amp1*cos(pha1) / ((amp2 * cos(pha2)) + (amp3 * cos(pha3)));
	contr2 = amp1*cos(pha2) / ((amp1 * cos(pha1)) + (amp3 * cos(pha3)));
	contr3 = amp1*cos(pha3) / ((amp2 * cos(pha2)) + (amp1 * cos(pha1)));
	
	}
	
	}
}
	
	xdriver = interx - xold;
	ydriver = intery - yold;
	
	xdriver2 = interx - xold2;
	ydriver2 = intery - yold2;	

	xdriver3 = interx - xold3;
	ydriver3 = intery - yold3;		
	
	
	
cimg_forXYC(img,x,y,v) {
	
img(x,y,0) = 10*grid[x][y][1]; //r
img(x,y,1) = grid[x][y][1];	//g
img(x,y,2) = 0.1*grid[x][y][1];//b


img(interx,intery,0) = 255;
img(interx,intery,1) = 0;
img(interx,intery,2) = 0;


img(xstart,ystart,0) = 150;
img(xstart,ystart,1) = 150;
img(xstart,ystart,2) = 150;

img(xstart2,ystart2,0) = 150;
img(xstart2,ystart2,1) = 150;
img(xstart2,ystart2,2) = 150;

img(xstart3,ystart3,0) = 150;
img(xstart3,ystart3,1) = 150;
img(xstart3,ystart3,2) = 150;

};

img.display(main_disp);


xold = xstart;
yold = ystart;
if(xold < 10){xold = 10;};
if(xold > (xsize-10)){xold = xsize-10;};
if(yold < 10){yold = 10;};
if(yold > (ysize-10)){yold = ysize-10;};

xold2 = xstart2;
yold2 = ystart2;
if(xold2 < 10){xold2 = 10;};
if(xold2 > (xsize-10)){xold2 = xsize-10;};
if(yold2 < 10){yold2 = 10;};
if(yold2 > (ysize-10)){yold2 = ysize-10;};

xold3 = xstart3;
yold3 = ystart3;
if(xold3 < 10){xold3 = 10;};
if(xold3 > (xsize-10)){xold3 = xsize-10;};
if(yold3 < 10){yold3 = 10;};
if(yold3 > (ysize-10)){yold3 = ysize-10;};



	wave1(&amp1,&pha1,interx,intery,timet);
	wave2(&amp2,&pha2,interx,intery,timet);
	wave2(&amp3,&pha3,interx,intery,timet);
	//float scalaramplitude =  (amp1 * 0.707) + (amp2 * 0.707);
	//float vectoramplitude = (amp1 * cos(pha1)) + (amp2 * cos(pha2)) ;


      Message msg("/msg"); 
	   msg.pushFloat((amp1 * cos(pha1))); // amplitude of agent 1
	   msg.pushFloat(omega1);               //phase of agent 1
       msg.pushFloat((amp2 * cos(pha2))); //amplitude of agent 2
       msg.pushFloat(omega2);                   //phase of agent 2
       msg.pushFloat((amp3 * cos(pha3)));       //amplitude of agent 3
       msg.pushFloat(omega3);               //phase of agent 3
       msg.pushFloat(xstart);           //x position of agent 1
       msg.pushFloat(ystart);           //y position of agent 1
       msg.pushFloat(xstart2);          //""" agent 2
       msg.pushFloat(ystart2);
       msg.pushFloat(xstart3);          //""" agent 3
       msg.pushFloat(ystart3);
       msg.pushFloat(interx);           //x position of maximum interf.
       msg.pushFloat(intery);           //y position of maximum interf. (nb size 400px)
       msg.pushFloat(contr1);           //contribution of agent 1
       msg.pushFloat(contr2);           //"" of agent 2
       msg.pushFloat(contr3);           //"" of agent 3
      PacketWriter pw;
      pw.startBundle().startBundle().addMessage(msg).endBundle().endBundle();
      bool ok = sock.sendPacket(pw.packetData(), pw.packetSize());
      
      
          PacketReader pr;
       if (sock.isOk()) {   
		    cout << "Socket is OK! \n";   
      if (sock.receiveNextPacket(30)) {  //number of milliseconds allowable delay
        pr.init(sock.packetData(), sock.packetSize());
        oscpkt::Message *msg;
        
        
        while (pr.isOk() && (msg = pr.popMessage()) != 0) {
          int iarg;
          if (msg->match("/").popInt32(iarg).isOkNoMoreArgs()) {
            cout << "Server: received /ping " << iarg << " from " << sock.packetOrigin() << "\n";
          } 
          else {
            cout << "Server: unhandled message: " << *msg << "\n";
          }
        }
        
        
        
        
      }
    }

	



		//cout << xdriver2 << ", " << ydriver2  << endl;
		

//	    while(main_disp.is_key() != 1){
 //    main_disp.wait();
  //  }
	



if( (contr1 - contr1old) < 0){
	learn1 = 1/learn1;
}

if( (contr2 - contr2old) < 0){
	learn2 = 1/learn2;
}

if( (contr3 - contr3old) < 0){
	learn3 = 1/learn3;
}

omega1 = omega1*learn1;
omega2 = omega2*learn2;
omega3 = omega3*learn3;

//cout << "w1 " << omega1 << "  w2 " << omega2 << " w3 " << omega3 << endl;

}


}
	
	
	
	
	
	
	
	
	
	
	
	


	

	
	
	
	
	
	
	
	
	
	
	
	


	
