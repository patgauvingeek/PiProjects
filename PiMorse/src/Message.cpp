#include <map>
#include <string>
#include <vector>
#include <thread>

#include "GpIoElement.h"
#include "Message.h"

namespace PiMorse
{
  class MessageImpl
  {
    public:
      const int DotDurationInMilliseconds = 500;
      const int DashDurationInMilliseconds = 3 * DotDurationInMilliseconds;
      const int SilenceDurationBetweenUnitInMilliseconds = DotDurationInMilliseconds;
      // Total letter pause
      // SilenceDurationBetweenUnitInMilliseconds + LetterPauseDurationDifferenceInMilliseconds
      //   = 3 * DotDurationInMilliseconds
      const int LetterPauseDurationDifferenceInMilliseconds = 2 * DotDurationInMilliseconds;
      // Total word pause
      // SilenceDurationBetweenUnitInMilliseconds + LetterPauseDurationDifferenceInMilliseconds
      //   + WorldPauseDurationDifferenceInMilliseconds = 7 * DotDurationInMilliseconds
      const int WorldPauseDurationDifferenceInMilliseconds = 4 * DotDurationInMilliseconds;

      MessageImpl(std::string message)
        : mMessage(message)
        , mInternational({
                           {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."}, {'F', "..-."},
                           {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"}, {'K', "-.-"},
                           {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"}, {'P', ".--."},
                           {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"}, {'U', "..-"},
                           {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"}, {'Z', "--.."},
                           {'1', ".----"}, {'2', "..---"}, {'3', "...--"}, {'4', "....-"}, {'5', "....."},
                           {'6', "-...."}, {'7', "--..."}, {'8', "---.."}, {'9', "----."}, {'0', "-----"},
                         }) { }

      void send(std::string const & gpio)
      {
        //word preprocessing
        std::string word;
        std::vector<std::string> words;
        for(auto letter : mMessage)
        {
          if (letter == ' ')
          {
            words.push_back(word);
            word.clear();
          }
          else
          {
            word += letter;
          }
        }

        if (word != "")
        {
          words.push_back(word);
        }

        SimOn::GpIoElement gpioElement(gpio);
        gpioElement.setDirection(SimOn::Direction::Output);

        for(auto word : words)
        {
          mProcessingWord(word);
          for(auto letter : word)
          {
            mProcessingLetter(letter);
            //
            std::string const & units = mInternational[letter];
            for(auto unit : units)
            {
              mProcessingUnit(unit);
              gpioElement.setValue(SimOn::Value::High);
              if (unit == '.')
              {
                std::this_thread::sleep_for(std::chrono::milliseconds(DotDurationInMilliseconds));
              }
              else if (unit == '-')
              {
                std::this_thread::sleep_for(std::chrono::milliseconds(DashDurationInMilliseconds));
              }
              mUnitProcessed(unit);
              gpioElement.setValue(SimOn::Value::Low);
              std::this_thread::sleep_for(std::chrono::milliseconds(SilenceDurationBetweenUnitInMilliseconds));
            }
            mLetterProcessed(letter);
            std::this_thread::sleep_for(std::chrono::milliseconds(LetterPauseDurationDifferenceInMilliseconds));
          }
          mWordProcessed(word);
          std::this_thread::sleep_for(std::chrono::milliseconds(WorldPauseDurationDifferenceInMilliseconds));
        }
      }

      void setProcessingWord(std::function<void(std::string const &)> processingWord)
      {
        mProcessingWord = processingWord;
      }

      void setWordProcessed(std::function<void(std::string const &)> wordProcessed)
      {
        mWordProcessed = wordProcessed;
      }

      void setProcessingLetter(std::function<void(char)> processingLetter)
      {
        mProcessingLetter = processingLetter;
      }

      void setLetterProcessed(std::function<void(char)> letterProcessed)
      {
        mLetterProcessed = letterProcessed;
      }

      void setProcessingUnit(std::function<void(char)> processingUnit)
      {
        mProcessingUnit = processingUnit;
      }

      void setUnitProcessed(std::function<void(char)> unitProcessed)
      {
        mUnitProcessed = unitProcessed;
      }

    private:
      std::function<void(std::string const &)> mProcessingWord;
      std::function<void(std::string const &)> mWordProcessed;
      std::function<void(char)> mProcessingLetter;
      std::function<void(char)> mLetterProcessed;
      std::function<void(char)> mProcessingUnit;
      std::function<void(char)> mUnitProcessed;

      std::string mMessage;
      std::map<char, std::string> mInternational;

  };


  Message::Message(std::string message)
    : mImplementation(new MessageImpl(message))
  {
  }

  Message::~Message()
  {
    if (mImplementation != nullptr)
    {
      delete mImplementation;
      mImplementation = nullptr;
    }
  }

  void Message::send(std::string const & gpio)
  {
    mImplementation->send(gpio);
  }

  void Message::setProcessingWord(std::function<void(std::string const &)> processingWord)
  {
    mImplementation->setProcessingWord(processingWord);
  }

  void Message::setWordProcessed(std::function<void(std::string const &)> wordProcessed)
  {
    mImplementation->setWordProcessed(wordProcessed);
  }

  void Message::setProcessingLetter(std::function<void(char)> processingLetter)
  {
    mImplementation->setProcessingLetter(processingLetter);
  }

  void Message::setLetterProcessed(std::function<void(char)> letterProcessed)
  {
    mImplementation->setLetterProcessed(letterProcessed);
  }

  void Message::setProcessingUnit(std::function<void(char)> processingUnit)
  {
    mImplementation->setProcessingUnit(processingUnit);
  }

  void Message::setUnitProcessed(std::function<void(char)> unitProcessed)
  {
    mImplementation->setUnitProcessed(unitProcessed);
  }
}
