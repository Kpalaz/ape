/****************************************************************************/
/*!
\file   Effect.cpp
\author Kaan Palaz
\par    email: kaan.palaz\@digipen.edu
\par    DigiPen login: kaan.palaz
\par    Course: CS246
\par    FINAL PROJECT
\date   12/07/2016
\brief cl /EHsc Effect.cpp EffectDriver.cpp Control.cpp user32.lib portaudio_x86.lib


*/
/*************************************************************************/
#include "Effect.h"
#define R 0.99985
#define NORMAL 27570.836
float frand(float a = 0, float b = 0)
{
    return a + (b - a) * float(rand() / float(RAND_MAX));
}

Effect::Effect(float rate) :Control(3, "Plucked Filter Demo")
{
    std::cout << "Test" << std::endl;
    myrate = rate;
    datasize = rate*numberofsemitones;
    SetLabel(0, "Frequency");
    SetRange(0, 0, 440);
    SetValue(0, f0);
    SetLabel(1, "Play");
    SetRange(1, 0, 1);
    SetValue(1, 0);
    SetLabel(2, "Number of Semitones");
    SetRange(2, 1, 12);
    SetValue(2, 8);
    //SetLabel(3, "Gain");
    //SetRange(3, 0, 12);
    //SetValue(3, -6);
    
    
    Show();
    Reset();
}

Effect::~Effect(void)
{

}

float Effect::operator()(float x)
{
    if (playing == false)
        return 0.0f;


    //std::cout << myindex << std::endl;
    if (myindex == datasize)
    {
        SetValue(1, 0);
        Reset();
        return 0.0f;
    }
    float out = filtered[myindex++];
    //std::cout << out << std::endl;
    return out;
}

void Effect::ValueChanged(unsigned id, int value)
{
    std::stringstream ss;
    //freq
    if (id == 0)
    {
        freq = value;
        ss << "Note Frequency: " << freq << "Hz";
        SetValue(1, false);
        Reset();
    }

    //playing bool
    else if (id == 1)
    {
        playing = (bool)value;
        ss << "Playing: " << playing;
    }

    //semitones
    else if (id == 2)
    {
        semitones = value;
        ss << "Number of Semitones: " << semitones;

        SetValue(1, false);
        Reset();

    }

    else if (id == 3)
    {
        gain = value - 6;
        ss << "Gain: " << gain << "dB";

        SetValue(1, false);
        Reset();

    }


    //std::cout << ss.str() << std::endl;
    SetLabel(id, ss.str().c_str());
}

void Effect::Reset()
{
    if (filtered)
    {
        delete[] filtered;
        filtered = 0;
    }
    myindex = 0;
    f0 = freq;
    numberofsemitones = semitones;
    datasize = myrate*numberofsemitones;
    filtered = new float[datasize];
    calculatepluck();
}


void Effect::calculatepluck()
{
    if (playing == true || f0 == 0)
        return;

    int start = 0;
    int end = myrate;
    float m1 = -1.0f;
    for (int j = 0; j < numberofsemitones; ++j)
    {
        w0 = f0 / ((0.5f)*(float)myrate) * PI;
        D = (float)myrate / (float)f0;
        L = std::floor(D - (0.5f));
        delta = (float)(D - 0.5) - L;
        a = (sinf((1 - delta)*(w0 / 2.0f))) / (sinf((1 + delta)*(w0 / 2.0f)));
        std::queue<float> queue;

        //std::cout << "Current Semitone: " << jsa << std::endl;
        //std::cout << "rate is:  " << myrate << std::endl;
        //std::cout << "data size is:  " << datasize * 2 << std::endl;
        //std::cout << "f0 is:  " << f0 << std::endl;
        //std::cout << "w0 is:  " << w0 << std::endl;
        //std::cout << "D is:  " << D << std::endl;
        //std::cout << "L is:  " << L << std::endl;
        //std::cout << "a is:  " << a << std::endl;
        //std::cout << "delta is:  " << delta << std::endl;
        int offset = myrate/25;
        int temp = start + offset;
        for (int i = start; i < temp; ++i)
        {
            filtered[i] = frand(-15000.0f, 15000.0f);
        }

        for (int i = 0; i <  L ; ++i)
        {
            queue.push(0.0f);
        }

        float lowprev = 0.0;
        float allprevout = 0.0;
        float allprevin = 0.0;

        for (int i = start; i < end; ++i)
        {
            float delayval = (filtered[i] + (queue.front() * powf(R, L)));
            queue.pop();
            float lowpassval = (delayval + lowprev) / 2;
            lowprev = delayval;

            //ut = axt + xt-1 - ayt-1
            float allpassval = (a * lowpassval) + allprevin - (a * allprevout);
            allprevin = lowpassval;
            allprevout = allpassval;

            //add to delay
            filtered[i] = allpassval;
            queue.push(allpassval);

            if (m1 < std::fabs(allpassval))
                m1 = std::fabs(allpassval);
        }

        if (j == 2 || j == 6)
        {
            f0 *= pow(2.0, 1.0 / 12.0);
        }

        else
        {
            f0 *= pow(2.0, 2.0 / 12.0);
        }
        start += myrate;
        end += myrate;
    }


    //normalize
    float normalize = ((float)(gain)/ m1 ) ;
    for (int i = 0; i < datasize; ++i)
    {
        filtered[i] *= normalize;
    }
}