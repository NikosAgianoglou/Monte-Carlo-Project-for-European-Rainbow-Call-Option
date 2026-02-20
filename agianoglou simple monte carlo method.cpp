#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <random>
#include <algorithm>
using namespace std;

// Generate a random number between 0 and 1
// Return a uniform number in [0,1].
double UniRand()
{
    return rand() / double(RAND_MAX);
}

// The inverse of the cummulative distribution function of a Normal
// distribution with mean mu and standard deviation sigma, used to generate
// gaussian random numbers - necessary for the Monte Carlo method below
double inverse_of_normal_cdf(double p, double mu, double sigma)
{
    if (p <= 0.0 || p >= 1.0)
    {
        std::stringstream os;
        os << "Invalid input argument (" << p
           << "); must be larger than 0 but less than 1.";
        throw std::invalid_argument(os.str());
    }

    double r, val;

    const double q = p - 0.5;

    if (std::abs(q) <= .425) {
        r = .180625 - q * q;
        val =
            q * (((((((r * 2509.0809287301226727 +
                33430.575583588128105) * r + 67265.770927008700853) * r +
                45921.953931549871457) * r + 13731.693765509461125) * r +
                1971.5909503065514427) * r + 133.14166789178437745) * r +
                3.387132872796366608)
            / (((((((r * 5226.495278852854561 +
                     28729.085735721942674) * r + 39307.89580009271061) * r +
                   21213.794301586595867) * r + 5394.1960214247511077) * r +
                 687.1870074920579083) * r +
                42.313330701600911252) * r + 1);
    }
    else {
        if (q > 0) {
            r = 1 - p;
        }
        else {
            r = p;
        }

        r = std::sqrt(-std::log(r));

        if (r <= 5) {
            r += -1.6;
            val =
                ((((((r * 7.7454501427834140764e-4 +
                      .0227238449892691845833) * r + .24178072517745061177) * r +
                    1.27045825245236838258) * r +
                   3.64784832476320460504) * r +
                  5.7694972214606914055) * r +
                 4.6303378461565452959)
                / (((((((r *
                        1.05075007164441684324e-9 + 5.475938084995344946e-4) *
                       r + .0151986665636164571966) * r +
                      .14810397642748007459) * r +
                     .68976733498510000455) * r +
                    1.6763848301838038494) * r +
                   2.05319162663775882187) * r + 1);
        }
        else { /* very close to 0 or 1 */
            r += -5;
            val =
                ((((((r * 2.01033439929228813265e-7 +
                      2.71155556874348757815e-5) * r +
                     .0012426609473880784386) * r +
                    .026532189526576123093) * r +
                   .29656057182850489123) * r +
                  1.7848265399172913358) * r +
                 5.463784911641413699)
                / (((((((r *
                        2.04426310338993978564e-15 + 1.421511758316445887e-7) *
                       r + 1.8463183175100546818e-5) * r +
                      7.868691311456132591e-4) * r +
                     .0148753612908506148525) * r +
                    .13692988092273580531) * r +
                   .59983220655588793769) * r + 1);
        }

        if (q < 0.0) {
            val = -val;
        }
    }

    return mu + sigma * val;
}

// Generate a gaussian random number from N(0,1)
double Standard_Normal_Rand() {
    static std::mt19937_64 rng(123456);                 // fixed seed for reproducibility
    static std::normal_distribution<double> nd(0.0, 1.0);
    return nd(rng);
}
double sample_var =0.0;
 
// Pricing a European Call Option with the Monte Carlo method
double MC_Call_Price_Rainbow(double S1,double S2,double K,double r,double v1,double v2,double T,double p,int num_sims)  
 {
	double nu_T1=(r-0.5*v1*v1)*T;
	double nu_T2=(r-0.5*v2*v2)*T;
	double v_T1=v1*sqrt(T);
	double v_T2=v2*sqrt(T);
	
	double disc_payoff_sum=0.0;
	double disc_payoff_squared_sum=0.0;  
    
	for (int i=0; i<num_sims; i++){
		double epsilon1=Standard_Normal_Rand();
		double epsilon2=Standard_Normal_Rand();
	
		//Simple Monte Carlo
		double S_T1=S1*exp(nu_T1+v_T1*epsilon1);
		double S_T2=S2*exp(nu_T2+v_T2*epsilon2);
		double disc_payoff=std::max(std::min(S_T1,S_T2)-K,0.0);
		double disc_payoff_squared=disc_payoff*disc_payoff;
		disc_payoff_sum+=disc_payoff;
		disc_payoff_squared_sum+=disc_payoff_squared;
}
    
double disc_payoff_average=disc_payoff_sum/num_sims;
double price=disc_payoff_average*std::exp(-r*T);
sample_var=((disc_payoff_squared_sum-num_sims*disc_payoff_average*disc_payoff_average)/(num_sims-1))*std::exp(-2.0*r*T);

return price;
}
int main() {

    // First we create the parameter list
    int num_sims = 4000000;   // Number of simulated asset paths
    double S1 = 100.0;      // Stock1 price
    double S2 = 100.0;      // Stock2 price
    double K = 100.0;      // Strike price
    double r = 0.05;       // Risk-free rate (5%)
    double v1 = 0.2;        // Volatility of the underlying (20%)
    double v2 = 0.2;        // Volatility of the underlying (20%)
    double T = 1.0;        // One year until expiration
    double p = 0.0;       //correlation p between S1 AND S2

    // Then we calculate the call value via Monte Carlo
    double call = MC_Call_Price_Rainbow(S1, S2, K, r, v1, v2, T, p, num_sims);

    // Finally we output the parameters and prices
    
    // Relative error for alpha = 5% (95% confidence)
    double z = inverse_of_normal_cdf(0.975, 0.0, 1.0);
    double rel_error = z * std::sqrt(sample_var / num_sims) / call;
    
    cout << "--- SIMPLE MONTE CARLO METHOD ---" << endl;
    cout << "Number of Paths: " << num_sims << std::endl;
    cout << "Underlying 1: " << S1 << "\n";
    cout << "Underlying 2: " << S2 << "\n";
    cout << "Strike: " << K << "\n";
    cout << "Risk-Free Rate: " << r << "\n";
    cout << "Volatility1: " << v1 << "\n";
    cout << "Volatility2: " << v2 << "\n";
    cout << "Maturity: " << T << "\n";
    cout << "Correlation p between S1 AND S2: " << p << "\n";

    cout << "Call Price: " << call << "\n";
    cout << "Relative Error: " << rel_error << "\n";

    return 0;
}