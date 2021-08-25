#include <iostream>
#include <string>
#include <memory>
#include <array>
#include <functional>
#include <memory>
#include <tuple>
#include <charconv>
#include <cstdint>
#include <utility>
#include <cstdio>

#include <fcntl.h>

struct file_deleter {
    void operator()(std::FILE* fp)
    {
        if(fp)
          std::fclose(fp);
    }
};

using TSysFsFile = std::unique_ptr<std::FILE, file_deleter>;


constexpr inline std::string_view kExportString = "/sys/class/gpio/export";
constexpr inline std::string_view kUnexportString = "/sys/class/gpio/unexport";
constexpr inline std::string_view kConcreteGpioBegin = "/sys/class/gpio/gpio";

enum class Direction
{
  In,
  Out
};


#define LOG_DEBUG(MESSAGE) std::cout<<MESSAGE<<std::endl;
struct PortWrapper
{

  PortWrapper(int portNumber, Direction direction)
    : m_portNumber(portNumber),
      m_direction{direction},
      m_pValueFileHandle{nullptr},
      m_portStr{},
      m_portString{}
  {
    TSysFsFile pExportFile {fopen(kExportString.data(),"w")};

    LOG_DEBUG("Crated export file");
    if(!pExportFile)
      throw std::logic_error("Can't open sysfs for gpio exports");

LOG_DEBUG("convert begin");

      if(auto [ptr, ec] = std::to_chars(m_portStr.data(), m_portStr.data() + m_portStr.size(), m_portNumber); ec == std::errc())
      {
          m_portString = std::string_view(m_portStr.data(), ptr - m_portStr.data());
          fprintf(pExportFile.get(),m_portString.data() );
      }
      LOG_DEBUG(m_portString)
LOG_DEBUG("convert end");
    std::string concretePortString{kConcreteGpioBegin.data()};
    concretePortString += m_portString;

    std::string directionString{concretePortString};
    directionString+="/direction";
  std::string valueString {concretePortString};
  valueString +="/value";

LOG_DEBUG(directionString);
    TSysFsFile pDirectionFile {fopen(directionString.c_str(),"wr")};

    m_pValueFileHandle.reset(fopen(valueString.c_str(),"wr"));
LOG_DEBUG(valueString);
    if(direction == Direction::In){
      LOG_DEBUG("DIREECTION FILE WRITE BEGIN");
      LOG_DEBUG((pDirectionFile == nullptr));
      fprintf(pDirectionFile.get(), "r");
    }
    else
      fprintf(pDirectionFile.get(), "w");
LOG_DEBUG("WRITEN DIRECTION");
  }

  bool hasClick() const noexcept
  {
    if(m_direction != Direction::In)
      return false;

LOG_DEBUG("hasClick");
      constexpr std::size_t kTempBuf{10};
      std::array<char,kTempBuf> tempBufArray{};

      int len = fread(tempBufArray.data(),tempBufArray.size(),1,m_pValueFileHandle.get());
      if(len < 0)
      {
        throw std::logic_error("Can't read port state");
      }

        int result{};
 
        auto [ptr, ec] { std::from_chars(tempBufArray.data(), tempBufArray.data() + tempBufArray.size(), result) };
LOG_DEBUG(result);
      return result == 1;
  }
  ~PortWrapper()
  {
    TSysFsFile pUnexport {fopen(kUnexportString.data(),"w")};

    if(pUnexport)
    {
      fprintf(pUnexport.get(), m_portString.data() );
    }
  }

private:
  int m_portNumber;
  Direction m_direction;
  TSysFsFile m_pValueFileHandle;
  std::array<char, 10> m_portStr;
  std::string_view m_portString;
};

int main() {

  std::uint64_t kClicksCounter{};
  PortWrapper buttonPort(17, Direction::In);
  LOG_DEBUG("CONASTRUCTION READY")
  while(true)
  {
      if(buttonPort.hasClick())
      {
        system("clear");
        ++kClicksCounter;
        std::cout<<"Current clicks value:" << kClicksCounter;
      }
  }
  return 0;
}