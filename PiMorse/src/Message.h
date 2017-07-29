#include <string>
#include <functional>
namespace PiMorse
{
  class MessageImpl;
  class Message
  {
    public:
      Message(std::string message);
      ~Message();

      void send(std::string const & gpio);

      void setProcessingWord(std::function<void(std::string const &)> processingLetter);
      void setWordProcessed(std::function<void(std::string const &)> letterProcessed);
      void setProcessingLetter(std::function<void(char)> processingLetter);
      void setLetterProcessed(std::function<void(char)> letterProcessed);
      void setProcessingUnit(std::function<void(char)> processingUnit);
      void setUnitProcessed(std::function<void(char)> unitProcessed);

    private:
      MessageImpl * mImplementation;

  };
}
