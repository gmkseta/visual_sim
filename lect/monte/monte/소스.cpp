#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double getRandomNumber(const double min, const double max);
bool isInsideCircle(const double x, const double y);
double estimatePI(const int num_pts_square);


void main()
{
	FILE *of = fopen("square.txt", "w");

	srand((unsigned int)time(NULL));

	for (int num_pts_square = 10; num_pts_square < 100000000; num_pts_square *= 2)
	{
		const double pi_mc = estimatePI(num_pts_square);
		printf("Estimate PI with %d radom pts is %f\n", num_pts_square, pi_mc);
		fprintf(of, "%f\n", pi_mc);
	}
	fclose(of);
}

double getRandomNumber(const double min, const double max) {
	//temp >= 0.0&&temp<=1.0

	double temp = (double)rand() / (double)RAND_MAX;//


	temp = min + (max - min)*temp;

	return temp;

}
bool isInsideCircle(const double x, const double y) {
	const double x_c = 0.5;
	const double y_c = 0.5;
	const double r = 0.5;

	const double f = (x - x_c)*(x - x_c) + (y - y_c)*(y - y_c) - r*r;
	if (f > 0.0)return false;
	else return true;

}
double estimatePI(const int num_pts_square) {
	int num_pts_circle = 0;
	for (int i = 0; i < num_pts_square; i++) {

		double x = getRandomNumber(0.0, 1.0);
		double y = getRandomNumber(0.0, 1.0);

		if (isInsideCircle(x , y) == true) {
			num_pts_circle++;
		}

	}
	const double pi_mc = 4.0 * num_pts_circle / num_pts_square;
	return pi_mc;
}
