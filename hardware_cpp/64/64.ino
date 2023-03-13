#include <complex>
#include <iostream>
#include <valarray>
using namespace std;

//#define float PI = 3.141592653589793238460;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;




// Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
// Better optimized but less intuitive
// !!! Warning : in some cases this code make result different from not optimased version above (need to fix bug)
// The bug is now fixed @2017/05/30 
void fft(CArray& x)
{
	// DFT
	unsigned int N = x.size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				Complex t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//Complex f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
}

// inverse fft (in-place)
void ifft(CArray& x)
{
    // conjugate the complex numbers
    x = x.apply(std::conj);

    // forward fft
    fft( x );

    // conjugate the complex numbers again
    x = x.apply(std::conj);

    // scale the numbers
    x /= x.size();
}



void setup()
{
    Serial.begin(115200);
    const Complex test[] = { 0.03660901,  0.02503027,  0.0125486,  -0.00126834, -0.0162423,  -0.0317566,
 -0.04716781, -0.06223064, -0.07731122, -0.0932903,  -0.1112081, -0.13181009,
 -0.15519251, -0.18070561, -0.20714601, -0.23314231, -0.25755186, -0.27969157,
 -0.2993202,  -0.31642013, -0.33091341, -0.34245234, -0.35036717, -0.35377829,
 -0.35181994, -0.34389764, -0.32990708, -0.31034747, -0.28627466, -0.25909405,
 -0.23027257, -0.20107994, -0.172443,   -0.14495018, -0.11897913, -0.09485132,
 -0.07291051, -0.05350433, -0.03693474, -0.02344346, -0.01323026, -0.00644416,
 -0.00309483, -0.0028979,  -0.00513664, -0.00863372, -0.0118746,  -0.01325132,
 -0.01135374, -0.00524119,  0.00535603,  0.01997393,  0.03756568,  0.05685837,
  0.07677167,  0.0966957,   0.11649802,  0.13627588,  0.15600051,  0.17524363,
  0.19311304,  0.20840171,  0.21985242,  0.22641764};
    
    CArray data(test, 64);

    // forward fft
    fft(data);

    cout << "fft" << endl;
    for (int i = 0; i < 64; ++i)
    {
        cout << real(data[i]) << endl;        
    }

    // inverse fft
    ifft(data);

    cout << endl << "ifft" << endl;
    for (int i = 0; i < 64; ++i)
    {
        cout << real(data[i]) << endl;
    }
   
}
void loop()
{
//nil
}

