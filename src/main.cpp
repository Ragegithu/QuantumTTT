#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cassert>
#include <complex>

using ComplexF = std::complex<float>;
using ComplexD = std::complex<double>;

struct qubit 
{
    ComplexF alpha = {1.0,0.0}; 
    ComplexF beta = {0.0,0.0};

    bool measured = false;

    float probA()
    {
        return alpha.real() * alpha.real() + alpha.imag() * alpha.imag();
    }
    
    float probB()
    {
        return beta.real() * beta.real() + beta.imag() * beta.imag();
    }
};

void Rz(float angle, qubit& q)
{
    float rad = angle * (M_PI / 180);
    float c = cos(rad /2);
    float s = sin(rad /2);

    q.alpha = ComplexF(c,-s) * q.alpha;
    q.beta =  ComplexF(c,+s) * q.beta;
}

void Ry(float angle, qubit& q)
{
    float rad = angle * (M_PI / 180);
    float c = cos(rad/2);
    float s = sin(rad/2);

    ComplexF newA = c * q.alpha - s * q.beta;
    ComplexF newB = c * q.beta + s * q.alpha;

    q.alpha = newA;
    q.beta = newB;
}

void Hadamard(qubit& q)
{
    float s12 = 1/sqrt(2);
    ComplexF newA = s12 * q.alpha + s12 * q.alpha;
    ComplexF newB = s12 * q.beta - s12 * q.beta;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800,800}),"QTTT");

    //just testing
    qubit q;
    std::cout << "Q1 P Alpha: " << q.probA() << std::endl;
    std::cout << "Q1 P Beta: " << q.probB() << std::endl;
    Hadamard(q);
    Rz(180,q);
    Ry(45,q);
    Rz(180,q);
    Ry(170,q);
    Hadamard(q);
    std::cout << "Q1 P Alpha: " << q.probA() << std::endl;
    std::cout << "Q1 P Beta: " << q.probB() << std::endl;

    //be sure to remove the above block pls :)

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.display();
    }
}