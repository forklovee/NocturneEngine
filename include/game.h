#pragma once

#include <string>

namespace NocEngine {

class Game {
public:
  Game(const std::string &config);

  void Run();

private:
  void Init(const std::string &path);

private:
  // GLTFwindow
};

} // namespace NocEngine
