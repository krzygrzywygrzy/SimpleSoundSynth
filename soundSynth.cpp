#include <iostream>
#include "olcNoiseMaker.h"

using namespace std;

atomic<double> dFrequencyOutput = 0.0;

double toAngularVelocity(double dHertz) {
	return dHertz * 2.0 * PI;
}

double osc(double dHertz, double dTime, int nType) {
	switch (nType)
	{
	case 0:
		return sin(toAngularVelocity(dHertz) * dTime);
	case 1: 
		return sin(toAngularVelocity(dHertz) * dTime) > 0.0 ? 1.0 : -1.0;

	case 2: //Triangle wave
		return asin(sin(toAngularVelocity(dHertz) * dTime) * 2.0 / PI);
	case 3: //SAW wave (analogue) 
	{
		double dOutput = 0.0;

		for (double n = 1.0; n < 50; n++) {
			dOutput += (sin(n * toAngularVelocity(dHertz) * dTime));
		}

		return dOutput * (2.0 / PI);
	}
	case 4: //SAW Wave (fast)
		return (2.0 / PI) * (dHertz * PI * fmod(dTime, 1.0 /dHertz) - (PI / 2.0));

	case 5: //Pseudo Random Noise
		return 2.0 * ((double)rand() / (double)RAND_MAX) - 1.0;
	default:
		return 0;
	}
}

double MakeNoise(double dTime) {
	double dOutput = osc(dFrequencyOutput, dTime, 3);
	return dOutput * 0.01;
}



int main()
{
	wcout << "Synth" << endl;

	//Getting all sound hardware
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	//Display findings
	for (auto d : devices) wcout << "Found Output Device:" << d << endl;

	//Create sound machine
	olcNoiseMaker<short> sound(devices[0], 44100);

	//Link noise function with sound machine 
	sound.SetUserFunction(MakeNoise);
	
	double dOctaveBaseFrequency = 110.0; //A2
	double d12thRootOf2 = pow(2.0, 1.0 / 12.0);

	while (1) {
		// Add a keyboard 
		//bool bKeyPressed = false;
		//for (int k = 0; k < 15; k++) {
		//	if (GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe"[k])) & 0x8000) {
		//		//dFrequencyOutput = dOctaveBaseFrequency * pow(d12thRootOf2, k);
		//		dFrequencyOutput = 440.0; //A3s
		//}
		//}
		
		//if (!bKeyPressed)
			dFrequencyOutput = 440.0;
	}

	return 0;
}
