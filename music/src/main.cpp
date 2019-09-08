#include <iostream>
#include <vector>
#include <thread>

#include "RealTimeApplication.h"
#include "PwmElement.h"

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
        mPwm.setFrequency(mNotes[mCurrentNoteIndex].frequency());
        mPwm.start();
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
  const int cDurationPrecision = 8;
  auto wSleep = wTempo / cDurationPrecision; // to be able to control fast note.

  //
  // Start playing music !
  //
  SimOn::RealTimeApplication::initialize();

  Partition wPartition("0");
  
  const int cSemibreve = cDurationPrecision * 4;
  const int cMinim = cDurationPrecision * 2;
  const int cCrotchet = cDurationPrecision;
  const int cQuaver = cDurationPrecision / 2;
  const int cSemiQuaver = cDurationPrecision / 4;

  const double cC4 = 261.63;
  const double cD4 = 293.66;
  const double cE4 = 329.63;
  
  wPartition.addNote(cC4, cCrotchet);
  wPartition.addNote(cC4, cCrotchet);
  wPartition.addNote(cD4, cCrotchet);
  wPartition.addNote(cD4, cCrotchet);
  wPartition.addNote(cE4, cCrotchet);
  wPartition.addNote(cE4, cCrotchet);

  while(!SimOn::RealTimeApplication::isTerminated())
  {   
    wPartition.update();
    std::this_thread::sleep_for(std::chrono::milliseconds(wSleep));
  }
}