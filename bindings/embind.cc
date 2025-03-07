#include "./Config.hh"
#include "./Layout.hh"
#include "./Node.hh"

#include <yoga/Yoga.h>

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(YGEnums) {
  enum_<YGMeasureMode>("YGMeasureMode")
    .value("undefined", YGMeasureModeUndefined)
    .value("exactly", YGMeasureModeExactly)
    .value("atMost", YGMeasureModeAtMost)
  ;

  enum_<YGUnit>("YGUnit")
    .value("undefined", YGUnitUndefined)
    .value("point", YGUnitPoint)
    .value("percent", YGUnitPercent)
    .value("auto", YGUnitAuto)
  ;

  enum_<YGGutter>("YGGutter")
    .value("column", YGGutterColumn)
    .value("row", YGGutterRow)
    .value("all", YGGutterAll)
  ;
}

EMSCRIPTEN_BINDINGS(Config) {
  class_<Config>("Config")
    .constructor<>(&Config::create, allow_raw_pointers())
    .class_function<>("create", &Config::create, allow_raw_pointers())
    .class_function<>("destroy", &Config::destroy, allow_raw_pointers())
    .function("createNode", &Config::createNode, allow_raw_pointers())
    .function("setExperimentalFeatureEnabled", &Config::setExperimentalFeatureEnabled)
    .function("isExperimentalFeatureEnabled", &Config::isExperimentalFeatureEnabled)
    .function("setPointScaleFactor", &Config::setPointScaleFactor)
  ;
}

EMSCRIPTEN_BINDINGS(Layout) {
  value_object<Layout>("Layout")
    .field("left", &Layout::left)
    .field("right", &Layout::right)
    .field("top", &Layout::top)
    .field("bottom", &Layout::bottom)
    .field("width", &Layout::width)
    .field("height", &Layout::height)
  ;
}

EMSCRIPTEN_BINDINGS(Node) {
  class_<MeasureCallback>("MeasureCallback")
    .function("measure", &MeasureCallback::measure, pure_virtual())
    .allow_subclass<MeasureCallbackWrapper>("MeasureCallbackWrapper")
  ;
  class_<DirtiedCallback>("DirtiedCallback")
    .function("dirtied", &DirtiedCallback::dirtied, pure_virtual())
    .allow_subclass<DirtiedCallbackWrapper>("DirtiedCallbackWrapper")
  ;

  value_object<YGSize>("Size")
    .field("width", &YGSize::width)
    .field("height", &YGSize::height)
    ;

  value_object<YGValue>("Value")
    .field("value", &YGValue::value)
    .field("unit", &YGValue::unit)
    ;

  class_<Node>("Node")
    .class_function<>("destroy", &Config::destroy, allow_raw_pointers())

    .function("reset", &Node::reset)

    .function("copyStyle", &Node::copyStyle)

    .function("setPositionType", &Node::setPositionType)
    .function("setPosition", &Node::setPosition)
    .function("setPositionPercent", &Node::setPositionPercent)

    .function("setAlignContent", &Node::setAlignContent)
    .function("setAlignItems", &Node::setAlignItems)
    .function("setAlignSelf", &Node::setAlignSelf)
    .function("setFlexDirection", &Node::setFlexDirection)
    .function("setFlexWrap", &Node::setFlexWrap)
    .function("setJustifyContent", &Node::setJustifyContent)

    .function("setMargin", &Node::setMargin)
    .function("setMarginPercent", &Node::setMarginPercent)
    .function("setMarginAuto", &Node::setMarginAuto)

    .function("setOverflow", &Node::setOverflow)
    .function("setDisplay", &Node::setDisplay)

    .function("setFlex", &Node::setFlex)
    .function("setFlexBasis", &Node::setFlexBasis)
    .function("setFlexBasisPercent", &Node::setFlexBasisPercent)
    .function("setFlexGrow", &Node::setFlexGrow)
    .function("setFlexShrink", &Node::setFlexShrink)

    .function("setWidth", &Node::setWidth)
    .function("setWidthPercent", &Node::setWidthPercent)
    .function("setWidthAuto", &Node::setWidthAuto)
    .function("setHeight", &Node::setHeight)
    .function("setHeightPercent", &Node::setHeightPercent)
    .function("setHeightAuto", &Node::setHeightAuto)

    .function("setMinWidth", &Node::setMinWidth)
    .function("setMinWidthPercent", &Node::setMinWidthPercent)
    .function("setMinHeight", &Node::setMinHeight)
    .function("setMinHeightPercent", &Node::setMinHeightPercent)

    .function("setMaxWidth", &Node::setMaxWidth)
    .function("setMaxWidthPercent", &Node::setMaxWidthPercent)
    .function("setMaxHeight", &Node::setMaxHeight)
    .function("setMaxHeightPercent", &Node::setMaxHeightPercent)

    .function("setAspectRatio", &Node::setAspectRatio)

    .function("setBorder", &Node::setBorder)

    .function("setPadding", &Node::setPadding)
    .function("setPaddingPercent", &Node::setPaddingPercent)
    .function("setGap", &Node::setGap)

    .function("getPositionType", &Node::getPositionType)
    .function("getPosition", &Node::getPosition)

    .function("getAlignContent", &Node::getAlignContent)
    .function("getAlignItems", &Node::getAlignItems)
    .function("getAlignSelf", &Node::getAlignSelf)
    .function("getFlexDirection", &Node::getFlexDirection)
    .function("getFlexWrap", &Node::getFlexWrap)
    .function("getJustifyContent", &Node::getJustifyContent)

    .function("getMargin", &Node::getMargin)

    .function("getFlexBasis", &Node::getFlexBasis)
    .function("getFlexGrow", &Node::getFlexGrow)
    .function("getFlexShrink", &Node::getFlexShrink)

    .function("getWidth", &Node::getWidth)
    .function("getHeight", &Node::getHeight)

    .function("getMinWidth", &Node::getMinWidth)
    .function("getMinHeight", &Node::getMinHeight)

    .function("getMaxWidth", &Node::getMaxWidth)
    .function("getMaxHeight", &Node::getMaxHeight)

    .function("getAspectRatio", &Node::getAspectRatio)

    .function("getBorder", &Node::getBorder)

    .function("getOverflow", &Node::getOverflow)
    .function("getDisplay", &Node::getDisplay)

    .function("getPadding", &Node::getPadding)
    .function("getGap", &Node::getGap)

    .function("insertChild", &Node::insertChild, allow_raw_pointers())
    .function("removeChild", &Node::removeChild, allow_raw_pointers())

    .function("getChildCount", &Node::getChildCount)

    .function("getParent", &Node::getParent, allow_raw_pointers())
    .function("getChild", &Node::getChild, allow_raw_pointers())

    .function("setMeasureFunc", &Node::setMeasureFunc, allow_raw_pointers())
    .function("unsetMeasureFunc", &Node::unsetMeasureFunc)

    .function("setDirtiedFunc", &Node::setDirtiedFunc, allow_raw_pointers())
    .function("unsetDirtiedFunc", &Node::unsetDirtiedFunc)

    .function("markDirty", &Node::markDirty)
    .function("isDirty", &Node::isDirty)

    .function("calculateLayout", &Node::calculateLayout)

    .function("getComputedLeft", &Node::getComputedLeft)
    .function("getComputedRight", &Node::getComputedRight)

    .function("getComputedTop", &Node::getComputedTop)
    .function("getComputedBottom", &Node::getComputedBottom)

    .function("getComputedWidth", &Node::getComputedWidth)
    .function("getComputedHeight", &Node::getComputedHeight)

    .function("getComputedLayout", &Node::getComputedLayout)

    .function("getComputedMargin", &Node::getComputedMargin)
    .function("getComputedBorder", &Node::getComputedBorder)
    .function("getComputedPadding", &Node::getComputedPadding)
  ;
}
