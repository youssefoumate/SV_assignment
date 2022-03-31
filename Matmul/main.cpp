#include <cv.h>
#include <highgui.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<iostream>
#include <omp.h>

#include <vector>

class Timer {
    struct timespec s_;
public:
    Timer() { tic(); }
    void tic() {
        clock_gettime(CLOCK_REALTIME, &s_);
    }

    double toc() {
        struct timespec e;
        clock_gettime(CLOCK_REALTIME, &e);
        return (double)(e.tv_sec - s_.tv_sec) + 1e-9 * (double)(e.tv_nsec - s_.tv_nsec);
    }
};

// Please optimize this function
void matmult(int m, int n, int k, const float* mat_a, const float* mat_b, float* mat_c)
{
    /*
        == input ==
        mat_a: m x k matrix
        mat_b: k x n matrix

        == output ==
        mat_c: m x n matrix (output)
    */

    for (int i1=0; i1<m; i1++) {
        for (int i2=0; i2<n; i2++) {
            mat_c [n*i1 + i2] = 0;
            for (int i3=0; i3<k; i3++) {
                mat_c[n*i1 + i2] += mat_a[i1 * k + i3] * mat_b[i3 * n + i2];
            }
        }
    }
}

// Increasing cache hit
void cache_matmult(int m, int n, int k, const float* mat_a, const float* mat_b, float* mat_c)
{
    /*
        == input ==
        mat_a: m x k matrix
        mat_b: k x n matrix

        == output ==
        mat_c: m x n matrix (output)
    */

    for (int i1=0; i1<m; i1++) {
        for (int i3=0; i3<n; i3++) {
            mat_c [n*i1 + i3] = 0;
            for (int i2=0; i2<k; i2++) {
                mat_c[n*i1 + i2] += mat_a[i1 * k + i3] * mat_b[i3 * n + i2];
            }
        }
    }
}

void genmat(int n, int m, std::vector<float>& mat, cv::Mat &cv_mat)
{
    srand(time(0));
    mat.resize(n * m);
    for (int i=0; i < mat.size(); i++) mat[i] = rand() % 100;
    // fill cv_mat
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++)
            cv_mat.at<float>(i,j) = rand() % 100;
}

void dumpmat(int n, int m, std::vector<float>& mat)
{
    for (int i=0; i<n; i++)
    {
        for (int j=0; j<m; j++)
            printf("%f ", mat[i * m + j]);
        printf("\n");
    }
}

int main(int argc, char** argv)
{
    std::vector<float> mat_a;
    std::vector<float> mat_b;
    std::vector<float> mat_c;
    int N = 2048;
    // cv Mats
    cv::Mat cv_mat_a = cv::Mat::zeros(N,N,CV_32F);
    cv::Mat cv_mat_b = cv::Mat::zeros(N,N,CV_32F);
    cv::Mat cv_mat_c = cv::Mat::zeros(N,N,CV_32F);

    genmat(N, N, mat_a, cv_mat_a);
    genmat(N, N, mat_b, cv_mat_b);
    genmat(N, N, mat_c, cv_mat_c);

    Timer t;
    double elapsed=0;
    double cv_elapsed=0;
    double ch_elapsed=0;
    double p_elapsed=0;
    const int iteration = 1;
    //naive
    for (int i=0; i<iteration; i++)
    {
        t.tic();
        matmult(N, N, N, &mat_a[0], &mat_b[0], &mat_c[0]);
        elapsed += t.toc();
    }
    printf("matmul %lf ms\n", 1000.0 * elapsed / iteration);
    //std::cout << "******naive*******" << '\n';
    //dumpmat(N, N, mat_c);
    for (int i=0; i<iteration; i++) {
        t.tic();
        cv_mat_c = cv_mat_a * cv_mat_b;
        cv_elapsed += t.toc();
    }
    printf("cv mat mul %lf ms\n", 1000.0 * cv_elapsed / iteration);
    //std::cout << "********opencv*******" << '\n';
    //std::cout << "cv_M = " << std::endl << " "  << cv_mat_c << std::endl << std::endl;
    //printf("******\n");

    //dumpmat(100, 100, mat_c);
    return 0;
}
