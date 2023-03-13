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
    const Complex test[] = {3.66090068e-02, 2.50302691e-02, 1.25485961e-02, -1.26833657e-03, -1.62422985e-02, -3.17566016e-02, -4.71678124e-02, -6.22306391e-02, -7.73112155e-02, -9.32902985e-02, -1.11208190e-01, -1.31810088e-01, -1.55192508e-01, -1.80705611e-01, -2.07146012e-01, -2.33142310e-01, -2.57551864e-01, -2.79691568e-01, -2.99320205e-01, -3.16420134e-01, -3.30913413e-01, -3.42452342e-01, -3.50367168e-01, -3.53778294e-01, -3.51819941e-01, -3.43897645e-01, -3.29907077e-01, -3.10347470e-01, -2.86274659e-01, -2.59094052e-01, -2.30272569e-01, -2.01079937e-01, -1.72443002e-01, -1.44950178e-01, -1.18979126e-01, -9.48513217e-02, -7.29105053e-02, -5.35043280e-02, -3.69347418e-02, -2.34434625e-02, -1.32302588e-02, -6.44415610e-03, -3.09482748e-03, -2.89790168e-03, -5.13663677e-03, -8.63371636e-03, -1.18745970e-02, -1.32513241e-02, -1.13537353e-02, -5.24119161e-03, 5.35603367e-03, 1.99739342e-02, 3.75656849e-02, 5.68583731e-02, 7.67716693e-02, 9.66956974e-02, 1.16498020e-01, 1.36275882e-01, 1.56000509e-01, 1.75243631e-01, 1.93113038e-01, 2.08401706e-01, 2.19852424e-01, 2.26417637e-01, 2.27441494e-01, 2.22747403e-01, 2.12656892e-01, 1.97981607e-01, 1.79987132e-01, 1.60265566e-01, 1.40467103e-01, 1.21932675e-01, 1.05362036e-01, 9.06831917e-02, 7.72232467e-02, 6.41254165e-02, 5.08101608e-02, 3.72543568e-02, 2.39716909e-02, 1.17307425e-02, 1.14711738e-03, -7.69541351e-03, -1.54611595e-02, -2.35151677e-02, -3.35809911e-02, -4.71937595e-02, -6.51575921e-02, -8.72453371e-02, -1.12275552e-01, -1.38515581e-01, -1.64207903e-01, -1.87991094e-01, -2.09075404e-01, -2.27157250e-01, -2.42157107e-01, -2.53922186e-01, -2.62040413e-01, -2.65857166e-01, -2.64683511e-01, -2.58083801e-01, -2.46090162e-01, -2.29235770e-01, -2.08405029e-01, -1.84601711e-01, -1.58764954e-01, -1.31706947e-01, -1.04164656e-01, -7.69096875e-02, -5.08464277e-02, -2.70257189e-02, -6.52543087e-03, 9.77376120e-03, 2.14036280e-02, 2.83942082e-02, 3.11991673e-02, 3.05306302e-02, 2.71956185e-02, 2.19776548e-02, 1.55719587e-02, 8.59601108e-03, 1.69876711e-03, -4.27173472e-03, -8.16735017e-03, -8.63834531e-03, -4.45239877e-03, 5.10701720e-03, 1.99866848e-02, 3.94024955e-02, 6.21262257e-02, 8.68181499e-02, 1.12222366e-01, 1.37201865e-01, 1.60726983e-01, 1.81927589e-01, 2.00193626e-01, 2.15197362e-01, 2.26751807e-01, 2.34584516e-01, 2.38213615e-01, 2.37049237e-01, 2.30680868e-01, 2.19185958e-01, 2.03282520e-01, 1.84253192e-01, 1.63704297e-01, 1.43268820e-01, 1.24317893e-01, 1.07718906e-01, 9.37064066e-02, 8.19446275e-02, 7.17917730e-02, 6.26481875e-02, 5.41881347e-02, 4.63351154e-02, 3.90181758e-02, 3.18963429e-02, 2.42445373e-02, 1.50744439e-02, 3.42329643e-03, -1.13269080e-02, -2.92271762e-02, -4.97125786e-02, -7.17418504e-02, -9.40849824e-02, -1.15665576e-01, -1.35818967e-01, -1.54333490e-01, -1.71236568e-01, -1.86429523e-01, -1.99374725e-01, -2.09024512e-01, -2.14049534e-01, -2.13254747e-01, -2.05975529e-01, -1.92273718e-01, -1.72868123e-01, -1.48864007e-01, -1.21425570e-01, -9.15391965e-02, -5.99581011e-02, -2.73314591e-02, 5.56079295e-03, 3.75971301e-02, 6.72653677e-02, 9.28626320e-02, 1.12916484e-01, 1.26662317e-01, 1.34340028e-01, 1.37129787e-01, 1.36721041e-01, 1.34707054e-01, 1.32087373e-01, 1.29085717e-01, 1.25325266e-01, 1.20258040e-01, 1.13667933e-01, 1.06053229e-01, 9.87348269e-02, 9.36295874e-02, 9.27550121e-02, 9.76396330e-02, 1.08850392e-01, 1.25803780e-01, 1.46922759e-01, 1.70066579e-01, 1.93058031e-01, 2.14122172e-01, 2.32116190e-01, 2.46523617e-01, 2.57274081e-01, 2.64493246e-01, 2.68281224e-01, 2.68593827e-01, 2.65259424e-01, 2.58098840e-01, 2.47070686e-01, 2.32378382e-01, 2.14520902e-01, 1.94292394e-01, 1.72726277e-01, 1.50970802e-01, 1.30100751e-01, 1.10909634e-01, 9.37587229e-02, 7.85487009e-02, 6.48264753e-02, 5.19790047e-02, 3.94354182e-02, 2.68051939e-02, 1.38994897e-02, 6.16051864e-04, -1.32592434e-02, -2.82085213e-02, -4.49740752e-02, -6.43506474e-02, -8.68560613e-02, -1.12441716e-01, -1.40400299e-01, -1.69526985e-01, -1.98452353e-01, -2.25973275e-01, -2.51217811e-01, -2.73586781e-01, -2.92547954e-01, -3.07434479e-01, -3.17384548e-01, -3.21479675e-01, -3.19036422e-01, -3.09923939e-01, -2.94752554e-01, -2.74818801e-01, -2.51791023e-01, -2.27242224e-01, -2.02226166e-01, -1.77107050e-01, -1.51762367e-01};
    
    CArray data(test, 256);

    // forward fft
    fft(data);

    cout << "fft" << endl;
    for (int i = 0; i < 256; ++i)
    {
        cout << real(data[i]) << endl;        
    }

    // inverse fft
    ifft(data);

    cout << endl << "ifft" << endl;
    for (int i = 0; i < 256; ++i)
    {
        cout << real(data[i]) << endl;
    }
   
}
void loop()
{
//nil
}
