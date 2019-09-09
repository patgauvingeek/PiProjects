#include <iostream>
#include <vector>
#include <thread>

#include "RealTimeApplication.h"
#include "PwmElement.h"

const int cDurationPrecision = 8;
    
const int cSemibreve = cDurationPrecision * 4;
const int cMinim = cDurationPrecision * 2;
const int cCrotchet = cDurationPrecision;
const int cQuaver = cDurationPrecision / 2;
const int cSemiQuaver = cDurationPrecision / 4;

const int cSilence=0.0;
const int cC4=261.63;
const int cCS4=277.18;
const int cD4=293.66;
const int cDS4=311.13;
const int cE4=329.63;
const int cF4=349.23;
const int cFS4=369.99;
const int cG4=392;
const int cGS4=415.3;
const int cA4=440;
const int cAS4=466.16;
const int cB4=493.88;
const int cC5=523.25;
const int cCS5=554.37;
const int cD5=587.33;
const int cDS5=622.25;
const int cE5=659.25;
const int cF5=698.46;
const int cFS5=739.99;
const int cG5=783.99;
const int cGS5=830.61;
const int cA5=880;
const int cAS5=932.33;
const int cB5=987.77;
const int cC6=1046.5;

class Note
{
  public:
    Note(double frequency, int iterationCount)
      : mFrequency(frequency)
      , mDuration(iterationCount)
    { }
    double frequency() const { return mFrequency; }
    double duration() const { return mDuration; }
    
  private:
    double mFrequency;
    int mDuration;
};

class Partition
{
  public:
    Partition(std::string const & pwm)
      : mPwm(pwm)
      , mCurrentNoteIndex(0)
      , mCurrentNoteIteration(0)
      , mNotes()
    { }
    
    void addNote(double frequency, int iterationCount)
    {
      mNotes.emplace_back(frequency, iterationCount);
    }

    void update()
    {
      if (mCurrentNoteIteration == 0)
      {
        if (mNotes[mCurrentNoteIndex].frequency() > 5.0)
        {
          mPwm.setFrequency(mNotes[mCurrentNoteIndex].frequency());
          mPwm.start();
        }
        mCurrentNoteIteration++;
      }
      else if (mNotes[mCurrentNoteIndex].duration() == mCurrentNoteIteration)
      {
        mPwm.stop();
        mCurrentNoteIndex++;
        if (mCurrentNoteIndex == mNotes.size())
        {
          mCurrentNoteIndex = 0;
        }
        mCurrentNoteIteration = 0;
        return; // makes "mCurrentNoteIteration == 0" on the next update
      }
      mCurrentNoteIteration++;      
    }

  private:
    SimOn::PwmElement mPwm;
    size_t mCurrentNoteIndex;
    int mCurrentNoteIteration;
    std::vector<Note> mNotes;

};

void fillImperialMarch(Partition &partition)
{
  partition.addNote(cA4, cCrotchet);
  partition.addNote(cA4, cCrotchet);
  partition.addNote(cA4, cCrotchet);
  partition.addNote(cF4, cQuaver + cSemiQuaver);
  partition.addNote(cC5, cSemiQuaver);
  partition.addNote(cA4, cCrotchet);
  partition.addNote(cF4, cQuaver + cSemiQuaver);
  partition.addNote(cC5, cSemiQuaver);
  partition.addNote(cA4, cCrotchet + cSemiQuaver);

  partition.addNote(cSilence, cSemiQuaver);

  partition.addNote(cE5, cCrotchet);
  partition.addNote(cE5, cCrotchet);
  partition.addNote(cE5, cCrotchet);
  partition.addNote(cF5, cQuaver + cSemiQuaver);
  partition.addNote(cC5, cSemiQuaver);
  partition.addNote(cGS4, cCrotchet);
  partition.addNote(cF4, cQuaver + cSemiQuaver);
  partition.addNote(cC5, cSemiQuaver);
  partition.addNote(cA4, cCrotchet + cSemiQuaver);

  partition.addNote(cSilence, cSemiQuaver);

  partition.addNote(cA5, cCrotchet);
  partition.addNote(cA4, cQuaver);
  partition.addNote(cA4, cSemiQuaver);
  partition.addNote(cA5, cQuaver + cSemiQuaver);
  partition.addNote(cGS5, cQuaver);
  partition.addNote(cG5, cQuaver);
  partition.addNote(cFS5, cSemiQuaver);
  partition.addNote(cF5, cSemiQuaver);
  partition.addNote(cFS5, cQuaver);

  partition.addNote(cSilence, cQuaver);

  partition.addNote(cAS4, cQuaver);
  partition.addNote(cDS5, cQuaver + cSemiQuaver);
  partition.addNote(cD5, cQuaver);
  partition.addNote(cCS5, cQuaver);
  partition.addNote(cC5, cSemiQuaver);
  partition.addNote(cB4, cSemiQuaver);
  partition.addNote(cC5, cQuaver);

  partition.addNote(cSilence, cQuaver);

  partition.addNote(cF4, cSemiQuaver);
  partition.addNote(cGS5, cCrotchet);
  partition.addNote(cF4, cQuaver + cSemiQuaver);
  partition.addNote(cA4, cSemiQuaver);
  partition.addNote(cC5, cCrotchet);
  partition.addNote(cA4, cQuaver + cSemiQuaver);
  partition.addNote(cC5, cSemiQuaver);
  partition.addNote(cE5, cCrotchet + cSemiQuaver);

  partition.addNote(cA5, cCrotchet);
  partition.addNote(cA4, cQuaver);
  partition.addNote(cA4, cSemiQuaver);
  partition.addNote(cA5, cQuaver + cSemiQuaver);
  partition.addNote(cGS5, cQuaver);
  partition.addNote(cG5, cQuaver);
  partition.addNote(cFS5, cSemiQuaver);
  partition.addNote(cF5, cSemiQuaver);
  partition.addNote(cFS5, cQuaver);

  partition.addNote(cSilence, cQuaver);

  partition.addNote(cAS4, cQuaver);
  partition.addNote(cDS5, cQuaver + cSemiQuaver);
  partition.addNote(cD5, cQuaver);
  partition.addNote(cCS5, cQuaver);
  partition.addNote(cC5, cSemiQuaver);
  partition.addNote(cB4, cSemiQuaver);
  partition.addNote(cC5, cQuaver);

  partition.addNote(cSilence, cQuaver);

  partition.addNote(cF4, cQuaver);
  partition.addNote(cGS5, cCrotchet);
  partition.addNote(cF4, cQuaver + cSemiQuaver);
  partition.addNote(cC5, cSemiQuaver);
  partition.addNote(cA4, cCrotchet);
  partition.addNote(cF4, cQuaver + cSemiQuaver);
  partition.addNote(cC5, cSemiQuaver);
  partition.addNote(cA4, cCrotchet + cSemiQuaver); 

  partition.addNote(cSilence, cQuaver);
}

int main(int argc, char *argv[])
{
  //
  // create strings in a vector for every arguments
  //
  std::vector<std::string> wArgs;
  for(auto i = 0; i < argc; i++)
  {
    wArgs.emplace_back(argv[i]);
  }
  
  //
  // Parse command line arguments
  //
  auto wTempo = 800;
  for (auto i = wArgs.begin(); i < wArgs.end(); i++)
  {
    if (*i == "--tempo")
    {
      i++;
      wTempo = atoi((*i).c_str());
    }
  }
  auto wSleep = wTempo / cDurationPrecision; // to be able to control fast note.

  //
  // Start playing music !
  //
  SimOn::RealTimeApplication::initialize();

  Partition wPartition("0");
  fillImperialMarch(wPartition);

  while(!SimOn::RealTimeApplication::isTerminated())
  {   
    wPartition.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(wSleep));
  }
}