#include <cmath>
#include <fstream>
#include <ctime>
#include <iostream>
#include "CImg.h" 
#define pi 3.14159265359	
#define xsize 256
#define ysize 256
#define proxthresh 10
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
using namespace oscpkt;


struct wave{
	float amplitude;		//The starting amplitude of the emitted wave
	float xpos;				//The x,y positions of the agent
	float ypos;				//...
	float pointvelocity;	//The agent's velocity through space
	float omega;			//The emitted wave's angular velocity
	float beta;				//The emitted wave's spacial attenuation
	float learn;			//The agent's learning agressiveness 
	float contr;			//The agent's estimated contribution...
	float controld;			//...and in the previous timestep, for comparison
	float angle;			//The agent's spacial direction
	bool dominant;			//The agent's state: is it in the most harmonic pair?
};


float xdriver = 0;
float ydriver = 0;
float norm;
float radius;
float value;
float maxfactor;
int maxi;
int maxj;

struct wave wavearray[10];








	
	void wavecalc(wave waveN, float *a, float *p, float x, float y, float t){
		
		
		radius = sqrt(pow((x - waveN.xpos),2) + pow((y - waveN.ypos),2));
		
		*a = waveN.amplitude*exp( waveN.beta *radius)	;
		*p = waveN.omega *t + waveN.beta*radius;		
	}
	
	

int main() {
	
	
	srand (time(NULL));
	
	
	int wsize = 6; //number of waves
float cofactors[wsize][wsize];


					//amp,xstart,ystart,pointvel,omega,beta,learn,contrib
	wavearray[0] = {500,(rand() % xsize),(rand() % ysize),2.5,1.4,-0.001,0.15,0, 0, (rand() % 10)/ 5, 0};
wavearray[1] = {500,(rand() % xsize),(rand() % ysize),1.3,2.6,-0.001,0.13,0, 0, (rand() % 10)/ 5, 0};
wavearray[2] = {500,(rand() % xsize),(rand() % ysize),3.5,3.55,-0.001,0.24,0, 0, (rand() % 10)/ 5, 0};
wavearray[3] = {500,(rand() % xsize),(rand() % ysize),1.1,0.54,-0.001,0.53,0, 0, (rand() % 10)/ 5, 0};
wavearray[4] = {500,(rand() % xsize),(rand() % ysize),1.2,0.46,-0.001,0.05,0, 0, (rand() % 10)/ 5, 0};
wavearray[5] = {500,50,270,1.3,1.24,-0.001,0.46,0, 0, (rand() % 10)/ 5, 0};
wavearray[6] = {500,250,270,1.25,3.4,-0.001,0.13,0, 0, (rand() % 10)/ 5, 0};
wavearray[7] = {500,50,70,1.415,2.4,-0.001,0.42,0, 0, (rand() % 10)/ 5, 0};
wavearray[8] = {500,150,70,1.245,1.234,-0.001,0.4315,0, 0, (rand() % 10)/ 5, 0};
wavearray[9] = {500,50,170,1.35,1.414,-0.001,0.25,0, 0, (rand() % 10)/ 5, 0};
	
	cout << "HELLO WORLD" << endl;


	  UdpSocket sock;
  sock.connectTo("Jonathans-MacBook-Pro.local", PORT_NUM);
	
	float grid[xsize][ysize];
	
	float timet = 0;

	
	CImg<unsigned char> img(xsize,ysize,1,3);      
	  CImgDisplay main_disp(img,"Press any key to advance time");




	cimg_forXYC(img,x,y,v) {
	img(x,y,0) = 255;
	img(x,y,1) = 255;
	img(x,y,2) = 255;
		
	}
	
	
	float amp;
	float pha;
	
		int interx = 0;
	int intery = 0;
	float biggestyet = 0;
	
	for(timet = 0; timet < 100; timet= timet + 0.01){

	 interx = 100;
	 intery = 100;
	 biggestyet = 0.0;

	for(int i = 0; i < xsize; i++){
		for (int j = 0; j < ysize; j++){
	
	value = 0;
	for( int w = 0; w < wsize; w++){
		wavecalc(wavearray[w],&amp,&pha,i,j,timet);
		value += (amp * cos(pha));
		//value -= 0.1*(amp * 0.1);
	}
	
	grid[i][j] = abs(value);
	
	if (abs(grid[i][j]) >= biggestyet){
		biggestyet = value;
		interx = i;
		intery = j;
	}
	}
}
	


	for( int w = 0; w < wsize; w++){ 
		
		
wavecalc(wavearray[w],&amp,&pha,interx,intery,timet);
wavearray[w].controld = wavearray[w].contr;	





wavearray[w].xpos += wavearray[w].pointvelocity * cos(wavearray[w].angle);
wavearray[w].ypos += wavearray[w].pointvelocity * sin(wavearray[w].angle);




if(wavearray[w].xpos < 10){
	wavearray[w].xpos = 30;
	wavearray[w].angle -= pi/2;};
if(wavearray[w].xpos > (xsize-10)){
	wavearray[w].xpos = xsize-30;
	wavearray[w].angle -= pi/2;};
if(wavearray[w].ypos < 10){
	wavearray[w].ypos = 30;
	wavearray[w].angle -= pi/2;};
if(wavearray[w].ypos > (ysize-10)){
	wavearray[w].ypos = ysize-30;
	wavearray[w].angle -= pi/2;};




/*
if(wavearray[w].xpos < 10){
	wavearray[w].xpos = xsize-10;};
if(wavearray[w].xpos > (xsize-10)){
	wavearray[w].xpos = 10;};
if(wavearray[w].ypos < 10){
	wavearray[w].ypos = ysize-10;};
if(wavearray[w].ypos > (ysize-10)){
	wavearray[w].ypos = 10;};
	*/
	
	
//cout << "Array Number" << w << endl << wavearray[w].amplitude << "|||" << wavearray[w].xpos << endl;






maxfactor = 10;
for( int i = 0; i < wsize; i++){
	for( int j = 0; j < wsize; j++){
		if(i != j){
		cofactors[i][j] = abs(remainder(wavearray[i].omega,wavearray[j].omega));
		if(cofactors[i][j] < maxfactor){
			maxfactor = cofactors[i][j];
			maxi = i;
			maxj = j;
}
	}
}
}



	
//CONTRIBUTION TO INTERFERENCE  
wavearray[w].contr = ((amp * cos(pha)) - (amp * 0.1)) / (biggestyet - (amp * cos(pha)) + (amp * 0.1));
//CONTRIBUTION TO COFACTORS:
//wavearray[w].contr = 0;
//for( int j = 0; j < wsize; j++){wavearray[w].contr += cofactors[w][j];};



if( (wavearray[w].contr - wavearray[w].controld) > 0){ //CHANGE BACK TO < FOR CONTRIB TO INTERFERENCE
	wavearray[w].learn = -wavearray[w].learn;
	wavearray[w].learn = 0.9*wavearray[w].learn;
}
else{
wavearray[w].learn = wavearray[w].learn/0.9;
}

 wavearray[w].angle += (((rand() % 100) - 50)/10.0)*wavearray[w].learn;
 wavearray[w].omega += 0.01*wavearray[w].learn;
 wavearray[w].amplitude += ((rand() % 1) - 0.5)*wavearray[w].learn;
}





//*****EVOLUTIONARY ALGORYTHM
if( fmod(timet/0.01, 10) > (rand() % 50)){
//cout << "GOT TIME = " << fmod(timet/0.01, 10) << ", " << timet << endl;

for( int i = 0; i < wsize; i++){
	if( (i != maxi) && (i != maxj) ){
	radius = sqrt(pow(interx - wavearray[i].xpos,2) + (intery - wavearray[i].ypos,2)) / xsize;
	wavearray[i].amplitude;
	wavearray[i].xpos;
	wavearray[i].ypos;
	wavearray[i].pointvelocity = radius*((rand() %101 - 50)/100);
	wavearray[i].omega = radius*((rand() %101 - 50)/10);
	wavearray[i].beta;
	wavearray[i].learn = ((rand() %101 - 50)/10000);
	wavearray[i].contr;
	wavearray[i].controld; 
	wavearray[i].angle = 2*pi*xsize*((rand() %101 - 50)/100);
	wavearray[i].dominant = 0;
	}
	else{
	wavearray[i].dominant = 1;	
	}
}

}

/*	    while(main_disp.is_key() != 1){
     main_disp.wait();
    }
  */


			
				
cimg_forXYC(img,x,y,v) {

//img(x,y,0) = 10*grid[x][y]; //r
//img(x,y,1) = 1*grid[x][y];	//g
//img(x,y,2) = 0.1*grid[x][y];//b


img(interx,intery,0) = 0;
img(interx,intery,1) = 0;
img(interx,intery,2) = 0;



	for( int w = 0; w < wsize; w++){ 
		if( (w != maxi) && (w != maxj) ){
img(wavearray[w].xpos,wavearray[w].ypos,0) = 255;
img(wavearray[w].xpos,wavearray[w].ypos,1) = 0;
img(wavearray[w].xpos,wavearray[w].ypos,2) = 0;
}
else{
img(wavearray[w].xpos,wavearray[w].ypos,0) = 0;
img(wavearray[w].xpos,wavearray[w].ypos,1) = 255;
img(wavearray[w].xpos,wavearray[w].ypos,2) = 0;
	}
}
};




img.display(main_disp);

      Message wv0;
      Message wv1;
      Message wv2;
      Message wv3;
      Message wv4;
      Message wv5;
       
      
      
	  wv0.init("/wave0").pushFloat(wavearray[0].amplitude).pushFloat(wavearray[0].omega).pushBool(wavearray[0].dominant);
	  wv1.init("/wave1").pushFloat(wavearray[1].amplitude).pushFloat(wavearray[1].omega).pushBool(wavearray[1].dominant);
	  wv2.init("/wave2").pushFloat(wavearray[2].amplitude).pushFloat(wavearray[2].omega).pushBool(wavearray[2].dominant);
	  wv3.init("/wave3").pushFloat(wavearray[3].amplitude).pushFloat(wavearray[3].omega).pushBool(wavearray[3].dominant);
	  wv4.init("/wave4").pushFloat(wavearray[4].amplitude).pushFloat(wavearray[4].omega).pushBool(wavearray[4].dominant);
	  wv5.init("/wave5").pushFloat(wavearray[5].amplitude).pushFloat(wavearray[5].omega).pushBool(wavearray[5].dominant);	
	  
	 // cout << "maxfactor " << maxfactor << ", i = " << maxi << ", j = " << maxj << endl;
  	  	
	// cout << "omega " << wavearray[5].omega << endl;
 /*      msg.pushFloat((amp2 * cos(pha2)));
       msg.pushFloat(omega2);
       msg.pushFloat((amp3 * cos(pha3)));
       msg.pushFloat(omega3);
       msg.pushFloat(xstart);
       msg.pushFloat(ystart);
       msg.pushFloat(xstart2);
       msg.pushFloat(ystart2);
       msg.pushFloat(xstart3);
       msg.pushFloat(ystart3);
       msg.pushFloat(interx);
       msg.pushFloat(intery);
       msg.pushFloat(contr1);
       msg.pushFloat(contr2);
       msg.pushFloat(contr3);
*/
       
      PacketWriter pw;
      pw.startBundle().startBundle().addMessage(wv0).addMessage(wv1).addMessage(wv2).addMessage(wv3).addMessage(wv4).addMessage(wv5).endBundle().endBundle();
      bool ok = sock.sendPacket(pw.packetData(), pw.packetSize());







	

}




}
	
	
	
	
	
	
	
	
	
	
	
	


	
