#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <sstream>
#include <complex>
#include <cmath>

using ComplexF = std::complex<float>;
using ComplexD = std::complex<double>;

bool halt = false;

std::string cmdLine = "NO COMMAND HAS BEEN RUN YET";

struct qubit 
{
    ComplexF alpha = {1.0,0.0}; 
    ComplexF beta = {0.0,0.0};

    bool measured = false;
    bool selected = false;

    int result = -1;

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
    float s12 = 1.0f/sqrt(2.0f);
    ComplexF newA = s12 * q.alpha + s12 * q.beta;
    ComplexF newB = s12 * q.alpha - s12 * q.beta;
    q.alpha = newA;
    q.beta  = newB;
}

void measure(qubit& q)
{
    if (q.measured) return; // already collapsed, do nothing
    
    float roll = (float)rand() / RAND_MAX;
    
    if (roll < q.probB()) // collapsed to 1
    {
        q.alpha = {0.0f, 0.0f};
        q.beta  = {1.0f, 0.0f};
        q.result = 1;
    }
    else // collapsed to 0
    {
        q.alpha = {1.0f, 0.0f};
        q.beta  = {0.0f, 0.0f};
        q.result = 0;
    }
    
    q.measured = true;
}

int x,y;
void parseCommand(const std::string& input, qubit board[3][3])
{
    std::istringstream ss(input);
    std::string cmd;
    
    ss >> cmd;
    
    bool oneSelected = false;
    for(int i = 0; i <= 2; i++)
    {
        for(int j = 0; j <= 2; j++)
        {
            if(board[j][i].selected)
            {
                oneSelected = true;
            }        
        }
    }
    if(cmd == "select")
    {
        ss >> x >> y;
        y-=1;
        x-=1;

        for(int i = 0; i <= 2; i++)
        {
            for(int j = 0; j <= 2; j++)
            {
                board[j][i].selected = false;
            }
        }
        
        if(x+1 > 3 || y+1 >3)
        {
            cmdLine = "OUT OF BOUNDS, STICK TO 3x3 GRID";
        }
        else
        {
            board[y][x].selected = true;
            cmdLine = "QUBIT X" + std::to_string(x+1) + "Y" + std::to_string(y+1) + " HAS BEEN SELECTED";
        }
    }
    else if(cmd == "hadamard")
    {
        if(oneSelected && !board[y][x].measured)
        {
            Hadamard(board[y][x]);
            cmdLine = "APPLIED HADAMARD ON QUBIT";
        }
        if(board[y][x].measured)
        {
            cmdLine = "QUBIT ALREADY COLLAPSED";
        }
        if(!oneSelected)
            cmdLine = "NO QUBIT HAS BEEN SELECTED";
    }
    else if(cmd == "rotateY")
    {
        float angle;
        ss >> angle;

        if(oneSelected && !board[y][x].measured)
        {
            Ry(angle, board[y][x]);
            cmdLine = "ROTATED QUBIT ON Y BY " + std::to_string(angle);
        }
        if(board[y][x].measured)
        {
            cmdLine = "QUBIT ALREADY COLLAPSED";
        }
        if(!oneSelected)
            cmdLine = "NO QUBIT HAS BEEN SELECTED";
    }
    else if(cmd == "rotateZ")
    {
        float angle;
        ss >> angle;

        if(oneSelected&& !board[y][x].measured)
        {
            Rz(angle, board[y][x]);
            cmdLine = "ROTATED QUBIT ON Z BY " + std::to_string(angle);
        }
        if(board[y][x].measured)
        {
            cmdLine = "QUBIT ALREADY COLLAPSED";
        }
        if(!oneSelected)
            cmdLine = "NO QUBIT HAS BEEN SELECTED";
    }
    else if(cmd == "measure")
    {
        if(oneSelected && !board[y][x].measured)
        {
            measure(board[y][x]);
            cmdLine = "QUBIT COLLAPSED";
        }
        if(!oneSelected)
            cmdLine = "NO QUBIT HAS BEEN SELECTED";
    }
    else if(cmd == "exit" || cmd == "close")
    {
        halt = true;
    }
    else
    {
        cmdLine = "COMMAND DOES NOT EXIST";
    }

}

int main()
{
    sf::RenderWindow window(sf::VideoMode({1200,800}),"QTTT");

    qubit qbitarr[3][3] = {};
    float qubitSize = 200;
    
    sf::Font font;
    if (!font.openFromFile("../assets/dosfont.ttf"))
    {
        std::cout << "font not loaded; WRONG FILE LOCATION" << std::endl;
    }

    sf::Texture Otex;
    if(!Otex.loadFromFile("../assets/O.png"))
        std::cout << "O texture not loaded; WRONG FILE LOCATION" << std::endl;
    
    sf::Texture Xtex;
    if(!Xtex.loadFromFile("../assets/X.png"))
        std::cout << "X texture not loaded; WRONG FILE LOCATION" << std::endl;
    
    sf::Texture SPtex;
    if(!SPtex.loadFromFile("../assets/SuperPosition.png"))
        std::cout << "SuperPosition texture not loaded; WRONG FILE LOCATION" << std::endl;

    std::string inputBuffer;
    sf::Text inputText(font, inputBuffer,32);

    float SQAlpha = 0, SQBeta = 0;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>() || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) || halt == true)
                window.close();
            
            
            if (const auto* text = event->getIf<sf::Event::TextEntered>())
            {
                if (text->unicode == '\b' && !inputBuffer.empty())
                    inputBuffer.pop_back();
                else if (text->unicode == '\r')
                    {
                        parseCommand(inputBuffer,qbitarr);
                        inputBuffer.clear();
                    }
                else if (text->unicode < 128)
                    inputBuffer += static_cast<char>(text->unicode);
            }
        }
        
        window.clear(sf::Color({126,126,126,255}));

        //XO qubit part below.
        for(int i=0; i <= 2; i++)
        {
            for(int j=0; j <= 2; j++)
            {
                sf::RectangleShape qbGrid; sf::RectangleShape qbShape;
                qbGrid.setSize({qubitSize,qubitSize}); qbShape.setSize({qubitSize,qubitSize});
                qbGrid.setOutlineThickness(3);
                if(qbitarr[j][i].selected == true)
                {
                    qbGrid.setFillColor(sf::Color({190,190,190,255})); qbGrid.setOutlineColor(sf::Color::Black);
                    SQAlpha = std::norm(qbitarr[j][i].alpha);
                    SQBeta = std::norm(qbitarr[j][i].beta);
                }
                else
                {
                    qbGrid.setFillColor(sf::Color::Transparent);
                }

                if(!qbitarr[j][i].measured)
                {
                    qbShape.setTexture(&SPtex);
                }
                else if(qbitarr[j][i].result == 1)
                {
                    qbShape.setTexture(&Xtex);
                }
                else if(qbitarr[j][i].result == 0)
                {
                    qbShape.setTexture(&Otex);
                }

                qbShape.setPosition({j*(qubitSize + 20) + 20,i*(qubitSize+ 20) + 80});
                qbGrid.setPosition({j*(qubitSize + 20) + 20,i*(qubitSize+ 20) + 80});
                window.draw(qbGrid);
                window.draw(qbShape);
            }
        }

        
        //sidePanel
        sf::RectangleShape infoWindow;
        infoWindow.setFillColor(sf::Color::Black);
        infoWindow.setOutlineColor(sf::Color::White);
        infoWindow.setOutlineThickness(3);
        infoWindow.setSize({800,1000});
        infoWindow.setPosition({700,0});

        sf::Text cmLine(font, cmdLine);
        cmLine.setPosition({720,300});

        sf::Text qbitInfoText(font, "Alpha: " + std::to_string(SQAlpha) + " " + "Beta: " + std::to_string(SQBeta));
        qbitInfoText.setPosition({720,100});

        //input box
        inputText.setPosition({710,400});
        inputText.setString("> " + inputBuffer);


        window.draw(infoWindow);
        window.draw(qbitInfoText);
        window.draw(cmLine);
        window.draw(inputText);

        window.display();
    }
}