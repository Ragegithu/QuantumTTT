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
    float c = cos(angle /2);
    float s = sin(angle /2);

    q.alpha = ComplexF(c,-s) * q.alpha;
    q.beta =  ComplexF(c,+s) * q.beta;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800,800}),"QTTT");

    qubit q;
    std::cout << "Q1 P Alpha: " << q.probA() << std::endl;
    std::cout << "Q1 P Beta: " << q.probB() << std::endl;
    Rz(180,q);
    std::cout << "Q1 P Alpha: " << q.probA() << std::endl;
    std::cout << "Q1 P Beta: " << q.probB() << std::endl;

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