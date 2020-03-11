#include "yaml-cpp-0.2.7/parser.h"
#include "yaml-cpp-0.2.7/contrib/graphbuilder.h"
#include "graphbuilderadapter.h"

namespace YAML_0_2_7
{
  void *BuildGraphOfNextDocument(Parser& parser, GraphBuilderInterface& graphBuilder)
  {
    GraphBuilderAdapter eventHandler(graphBuilder);
    if (parser.HandleNextDocument(eventHandler)) {
      return eventHandler.RootNode();
    } else {
      return NULL;
    }
  }
}
