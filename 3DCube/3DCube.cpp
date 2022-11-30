#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

float A, B, C;

float cubeWidth = 20;
const int width = 160, height = 44; // 160 * 44
float zBuffer[width * height];
char buffer[width * height];
int backgroundASCIICode = ' ';
int distanceFromCam = 100;
float horizontalOffset = -2 * cubeWidth;
float K1 = 40;

float incrementSpeed = 0.6;

float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k) {
    return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
        j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
    return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
        j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
        i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
    return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, char ch) {

    x = calculateX(cubeX, cubeY, cubeZ);
    y = calculateY(cubeX, cubeY, cubeZ);
    z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

    ooz = 1 / z;

    xp = (int)(width / 2 + horizontalOffset + K1 * ooz * x * 2);
    yp = (int)(height / 2 + K1 * ooz * y);

    idx = xp + yp * width;

    if (idx >= 0 && idx < width * height) {
        if (ooz > zBuffer[idx]) {
            zBuffer[idx] = ooz;
            buffer[idx] = ch;
        }
    }
}

int main() {

    system("CLS");

    while (true) {

        memset(buffer, backgroundASCIICode, width * height); // fills all buffer with ' '

        memset(zBuffer, 0, width * height * 4);

       

        // first cube
        for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
            for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += incrementSpeed) {
                calculateForSurface(cubeX, cubeY, -cubeWidth, '.');
                calculateForSurface(cubeWidth, cubeY, cubeX, '-');
                calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
                calculateForSurface(-cubeX, cubeY, cubeWidth, ':');
                calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
                calculateForSurface(cubeX, cubeWidth, cubeY, '+');
            }
        }
        
        
        system("CLS");

        for (int k = 0; k < width * height; k++) {
            if (k % width) {
                std::cout << buffer[k];
            }
            else
            {
                std::cout << char(10);
            }

           //  std::cout << buffer[k];
           // putchar(k % width ? buffer[k] : 10);

        }

        A += 0.05;
        B += 0.05;
        C += 0.01;

       // usleep(8000 * 2);
       std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}