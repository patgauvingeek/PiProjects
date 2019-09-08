#include "PwmElement.h"
#include "SysFsHelper.h"

#include <sstream>
#include <stdexcept>
#include <cmath>

namespace SimOn
{
  class PwmElementImpl
  {
    private:

#ifdef __arm__
      const std::string PWM_FOLDER = "/sys/class/pwm/pwmchip0";
#else
      const std::string PWM_FOLDER = "pwm/";
#endif

    public:
      PwmElementImpl(std::string const &pwm)
        : mPwm(pwm)
      {
        std::stringstream filename;
        filename << PWM_FOLDER << "export";
        SysFsHelper::writeData(filename.str(), pwm, "export PWM", pwm);
      }

      ~PwmElementImpl()
      {
        std::stringstream filename;
        filename << PWM_FOLDER << "unexport";
        SysFsHelper::writeData(filename.str(), mPwm, "unexport PWM", mPwm);
      }

      void setFrequency(double frequency)
      {
        int wPeriod = std::round(1 / frequency * 1e+9);
        setPeriod(wPeriod);
        setDutyCycle(wPeriod / 2);
      }

      void setPeriod(int period)
      {
        std::stringstream filename;
        filename << PWM_FOLDER << "pwm" << mPwm << "/period";
        std::stringstream wPeriod;
        wPeriod << period;
        SysFsHelper::writeData(filename.str(), wPeriod.str(), "write period for PWM", mPwm);
      }

      void setDutyCycle(int dutyCycle)
      {
        std::stringstream filename;
        filename << PWM_FOLDER << "pwm" << mPwm << "/duty_cycle";
        std::stringstream wDutyCycle;
        wDutyCycle << dutyCycle;
        SysFsHelper::writeData(filename.str(), wDutyCycle.str(), "write duty cycle for PWM", mPwm);
      }

      void start()
      {
        std::stringstream filename;
        filename << PWM_FOLDER << "pwm" << mPwm << "/enable";
        SysFsHelper::writeData(filename.str(), "1", "start PWM", mPwm);
      }

      void stop()
      {
        std::stringstream filename;
        filename << PWM_FOLDER << "pwm" << mPwm << "/enable";
        SysFsHelper::writeData(filename.str(), "0", "start PWM", mPwm);
      }

    private:
      std::string mPwm;

  };

  PwmElement::PwmElement(std::string const &pwm)
    : mImplementation(new PwmElementImpl(pwm))
  {
  }  

  PwmElement::~PwmElement()
  {
    delete mImplementation;
  }

  void PwmElement::setFrequency(double frequency)
  {
    mImplementation->setFrequency(frequency);
  }
  
  void PwmElement::setPeriod(int period)
  {
    mImplementation->setPeriod(period);
  }

  void PwmElement::setDutyCycle(int dutyCycle)
  {
    mImplementation->setDutyCycle(dutyCycle);
  }

  void PwmElement::start()
  {
    mImplementation->start();
  }

  void PwmElement::stop()
  {
    mImplementation->stop();
  }

}
