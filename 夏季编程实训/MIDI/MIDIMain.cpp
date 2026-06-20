#include <iostream>
#include <windows.h>
#include <stdlib.h>

using namespace std;


DWORD MidiOutMessage(HMIDIOUT hMidi, int iStatus, int iChannel, int iFlip, int iVolume) {
	DWORD dwMessage = (iVolume << 16) | (iFlip << 8) | iStatus         | iChannel;
	//      音量      |     音符     | 状态字节(高4位) | 通道(低4位)
	return midiOutShortMsg(hMidi, dwMessage);
}//更加方便调用midiOutShortMsg函数

enum Scale
{
	Rest = 0, C8 = 108, B7 = 107, A7s = 106, A7 = 105, G7s = 104, G7 = 103, F7s = 102, F7 = 101, E7 = 100,
	D7s = 99, D7 = 98, C7s = 97, C7 = 96, B6 = 95, A6s = 94, A6 = 93, G6s = 92, G6 = 91, F6s = 90, F6 = 89,
	E6 = 88, D6s = 87, D6 = 86, C6s = 85, C6 = 84, B5 = 83, A5s = 82, A5 = 81, G5s = 80, G5 = 79, F5s = 78,
	F5 = 77, E5 = 76, D5s = 75, D5 = 74, C5s = 73, C5 = 72, B4 = 71, A4s = 70, A4 = 69, G4s = 68, G4 = 67,
	F4s = 66, F4 = 65, E4 = 64, D4s = 63, D4 = 62, C4s = 61, C4 = 60, B3 = 59, A3s = 58, A3 = 57, G3s = 56,
	G3 = 55, F3s = 54, F3 = 53, E3 = 52, D3s = 51, D3 = 50, C3s = 49, C3 = 48, B2 = 47, A2s = 46, A2 = 45,
	G2s = 44, G2 = 43, F2s = 42, F2 = 41, E2 = 40, D2s = 39, D2 = 38, C2s = 37, C2 = 36, B1 = 35, A1s = 34,
	A1 = 33, G1s = 32, G1 = 31, F1s = 30, F1 = 29, E1 = 28, D1s = 27, D1 = 26, C1s = 25, C1 = 24, B0 = 23,
	A0s = 22, A0 = 21
};//把数字转化为对应的音符,其中60代表C4，数字加一音符升半音

enum Voice
{
	l1 = C3, l2 = D3, l3 = E3, l4 = F3, l5 = G3, l6 = A3, l7 = B3,
	m1 = C4, m2 = D4, m3 = E4, m4 = F4, m5 = G4, m6 = A4, m7 = B4,
	h1 = C5, h2 = D5, h3 = E5, h4 = F5, h5 = G5, h6 = A5, h7 = B5,
    j=770,k=385,l=192
};//低八度，中八度与高八度的音符，以及休止符

void SeeYouAgain(HMIDIOUT handle){

	
	int see[]={m5,m2,k,h2,k,h1,k,m5,m3,m1,j,h1,l,h2,l,h3,l,h2,l,h1,l,h2,l,m5,m4,k,h2,k,h1,k,m5,m1,j,h1,l,h2,l,h3,m2,l7,l,h2,l,h1,l,h2,l,m5,m3,l6,k,h2,k,h1,k,m5,m1,j,h1,l,h2,l,h3,l,h2,l,h1,l,h2,l,
	           m5,m4,m1,k,h2,k,h1,k,m5,m3,l5,j,m1,k,m3,k,m5,k,m1,m6,l6,k,j,m3,m5,m1,k,j,k,m1,k,m2,m4,k,m2,k,m1,k,m3,m1,k,j,k,m3,l,m5,l,m1,m6,l6,k,m7,k,m6,k,m5,m3,m1,k,m3,k,m2,k,m2,k,m1,k,
			   m2,l4,k,m2,m3,k,m1,m1,k,j,l5,l,m1,l,m3,l,m5,l,m3,m6,l6,j,k,m5,m3,m1,k,k,m6,j,m1,k,m2,l4,k,m2,k,m1,k,m3,m1,k,j,m3,l5,k,m5,k,m6,l6,k,h1,k,h2,k,m5,h3,m1,k,h2,k,h1,k,m3,m6,k,h1,k,
			   m4,h2,l4,k,h2,k,h1,k,h1,m3,m1,k,j,m6,k,h1,k,m4,h2,l4,k,h2,k,h1,k,m3,h1,m1,k,j,j,h3,k,l,h2,l,h3,k,l,h2,l,h3,k,l,h2,l,h3,l,h5,l,h3,l,h2,l,h1,k,l,m6,l,h1,k,h2,l,h3,l,j,k,h1,l,m6,l,
			   h1,k,l,m6,l,h1,k,h2,l,h3,l,j,h1,k,h1,l,m6,l,h1,k,l,m6,l,h1,k,h2,l,h1,l,j,m5,l,m6,l,h1,l,h2,l,h3,k,l,h2,l,h3,k,l,h2,l,h3,k,l,h2,l,h3,k,l,h2,l,h3,l,h5,l,h3,l,h2,l,
			   h1,k,l,m6,l,h1,k,h2,l,h3,l,j,k,h1,l,m6,l,h1,k,l,m6,l,h1,k,h2,l,h3,l,j,h1,k,h1,l,m6,l,h1,k,l,m6,l,h1,k,h2,l,h1,l,j,j};
	
     //see you again的部分简谱
	
	
	
	
	for(int i=0;i<sizeof(see) / sizeof(see[0]);i++){
		if(see[i]!=j&&see[i]!=k&&see[i]!=l)
		MidiOutMessage(handle, 0x90, 0x00, see[i], 127);//模拟钢琴的声音
        else 
		Sleep(see[i]);//模拟音符的延长
	}
	
}


int main() {
	HMIDIOUT handle;
	bool result=midiOutOpen(&handle, 0, 0, 0, CALLBACK_NULL);
	if(result) cout<<"Error"<<endl;//检测midi是否正常开启
	
    SeeYouAgain(handle);
    
    midiOutClose(handle);//关闭midi程序
    
	system("pause");
	return 0;
}