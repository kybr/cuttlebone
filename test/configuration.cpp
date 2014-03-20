#include "Framework/Configuration.hpp"

ConfigurationData configurationData[] = {{"quux", "127.0.0.1", true, true, true},
                                         {"audio", .audio = true},
                                         {"gr02", .visual = true},
                                         {"gr03", .visual = true},
                                         {"gr04", .visual = true}, };

int main() {
  Configuration a;
  Configuration b(configurationData);
}
