#include <stdio.h>
#include <math.h>

// 函数声明
void transpose(double A[][2], double At[][10], int rows, int cols);
void multiply(double A[][2], double B[][2], double result[][2], int rowsA, int colsA, int colsB);
void multiplyVector(double A[][2], double y[], double result[], int rowsA, int colsA);
void invert2x2(double A[][2], double Ainv[][2]);
void solveLinearSystem(double A[][2], double b[], double x[]);

int main() {
    // 数据表
    double x[] = {0.25, 0.50, 0.75, 1.00, 1.25, 1.50, 1.75, 2.00, 2.25, 2.50};
    double y[] = {1.284, 1.648, 2.117, 2.718, 3.427, 2.798, 3.534, 4.456, 5.465, 5.894};
    int n = 10;

    // 构造设计矩阵 A (10x2)
    double A[10][2];
    for (int i = 0; i < n; i++) {
        A[i][0] = sin(x[i]);
        A[i][1] = cos(x[i]);
    }

    // 计算 A^T
    double At[2][10];
    transpose(A, At, n, 2);

    // 计算 A^T * A
    double ATA[2][2];
    multiply(At, A, ATA, 2, n, 2);

    // 计算 A^T * y
    double ATy[2];
    multiplyVector(At, y, ATy, 2, n);

    // 解线性方程组 ATA * [a; b] = ATy
    double coefficients[2];
    solveLinearSystem(ATA, ATy, coefficients);

    double a = coefficients[0];
    double b = coefficients[1];

    // 计算均方误差 (MSE)
    double mse = 0.0;
    for (int i = 0; i < n; i++) {
        double predicted_y = a * sin(x[i]) + b * cos(x[i]);
        mse += pow(y[i] - predicted_y, 2);
    }
    mse /= n;

    // 输出结果
    printf("a = %.15f, b = %.15f, 均方误差 = %.15f\n", a, b, mse);

    return 0;
}

// 矩阵转置
void transpose(double A[][2], double At[][10], int rows, int cols) {
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            At[i][j] = A[j][i];
        }
    }
}

// 矩阵乘法
void multiply(double A[][2], double B[][2], double result[][2], int rowsA, int colsA, int colsB) {
    for (int i = 0; i < rowsA; i++) {
        for (int j = 0; j < colsB; j++) {
            result[i][j] = 0.0;
            for (int k = 0; k < colsA; k++) {
                result[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// 矩阵与向量相乘
void multiplyVector(double A[][2], double y[], double result[], int rowsA, int colsA) {
    for (int i = 0; i < rowsA; i++) {
        result[i] = 0.0;
        for (int j = 0; j < colsA; j++) {
            result[i] += A[i][j] * y[j];
        }
    }
}

// 2x2 矩阵求逆
void invert2x2(double A[][2], double Ainv[][2]) {
    double det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
    if (det == 0) {
        printf("矩阵不可逆\n");
        return;
    }
    Ainv[0][0] = A[1][1] / det;
    Ainv[0][1] = -A[0][1] / det;
    Ainv[1][0] = -A[1][0] / det;
    Ainv[1][1] = A[0][0] / det;
}

// 解线性方程组 Ax = b
void solveLinearSystem(double A[][2], double b[], double x[]) {
    double Ainv[2][2];
    invert2x2(A, Ainv);
    x[0] = Ainv[0][0] * b[0] + Ainv[0][1] * b[1];
    x[1] = Ainv[1][0] * b[0] + Ainv[1][1] * b[1];
}
