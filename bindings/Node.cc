#include <algorithm>

#include <yoga/Yoga.h>

#include "./Node.hh"
#include "./Layout.hh"
#include "./Config.hh"

static YGSize globalMeasureFunc(YGNodeRef nodeRef, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
  Node const& node = *reinterpret_cast<Node const*>(YGNodeGetContext(nodeRef));

  return node.callMeasureFunc(width, widthMode, height, heightMode);
}

static void globalDirtiedFunc(YGNodeRef nodeRef) {
  Node const& node = *reinterpret_cast<Node const*>(YGNodeGetContext(nodeRef));

  node.callDirtiedFunc();
}

/* static */ Node* Node::fromYGNode(YGNodeRef nodeRef) {
  return reinterpret_cast<Node*>(YGNodeGetContext(nodeRef));
}

/* static */ void Node::destroy(Node* node) {
  delete node;
}

Node::Node(Config* config)
    : m_node(YGNodeNewWithConfig(config->m_config))
    , m_measureFunc(nullptr)
    , m_dirtiedFunc(nullptr) {
  YGNodeSetContext(m_node, reinterpret_cast<void*>(this));
}

Node::~Node(void) {
  YGNodeFree(m_node);
}

void Node::reset(void) {
  m_measureFunc.reset(nullptr);
  m_dirtiedFunc.reset(nullptr);

  YGNodeReset(m_node);
}

void Node::copyStyle(Node const& other) {
  YGNodeCopyStyle(m_node, other.m_node);
}

void Node::setPositionType(int positionType) {
  YGNodeStyleSetPositionType(m_node, static_cast<YGPositionType>(positionType));
}

void Node::setPosition(int edge, double position) {
  YGNodeStyleSetPosition(m_node, static_cast<YGEdge>(edge), position);
}

void Node::setPositionPercent(int edge, double position) {
  YGNodeStyleSetPositionPercent(m_node, static_cast<YGEdge>(edge), position);
}

void Node::setAlignContent(int alignContent) {
  YGNodeStyleSetAlignContent(m_node, static_cast<YGAlign>(alignContent));
}

void Node::setAlignItems(int alignItems) {
  YGNodeStyleSetAlignItems(m_node, static_cast<YGAlign>(alignItems));
}

void Node::setAlignSelf(int alignSelf) {
  YGNodeStyleSetAlignSelf(m_node, static_cast<YGAlign>(alignSelf));
}

void Node::setFlexDirection(int flexDirection) {
  YGNodeStyleSetFlexDirection(m_node, static_cast<YGFlexDirection>(flexDirection));
}

void Node::setFlexWrap(int flexWrap) {
  YGNodeStyleSetFlexWrap(m_node, static_cast<YGWrap>(flexWrap));
}

void Node::setJustifyContent(int justifyContent) {
  YGNodeStyleSetJustifyContent(m_node, static_cast<YGJustify>(justifyContent));
}

void Node::setMargin(int edge, double margin) {
  YGNodeStyleSetMargin(m_node, static_cast<YGEdge>(edge), margin);
}

void Node::setMarginPercent(int edge, double margin) {
  YGNodeStyleSetMarginPercent(m_node, static_cast<YGEdge>(edge), margin);
}

void Node::setMarginAuto(int edge) {
  YGNodeStyleSetMarginAuto(m_node, static_cast<YGEdge>(edge));
}

void Node::setOverflow(int overflow) {
  YGNodeStyleSetOverflow(m_node, static_cast<YGOverflow>(overflow));
}

void Node::setDisplay(int display) {
  YGNodeStyleSetDisplay(m_node, static_cast<YGDisplay>(display));
}

void Node::setFlex(double flex) {
  YGNodeStyleSetFlex(m_node, flex);
}

void Node::setFlexBasis(double flexBasis) {
  YGNodeStyleSetFlexBasis(m_node, flexBasis);
}

void Node::setFlexBasisPercent(double flexBasis) {
  YGNodeStyleSetFlexBasisPercent(m_node, flexBasis);
}

void Node::setFlexGrow(double flexGrow) {
  YGNodeStyleSetFlexGrow(m_node, flexGrow);
}

void Node::setFlexShrink(double flexShrink) {
  YGNodeStyleSetFlexShrink(m_node, flexShrink);
}

void Node::setWidth(double width) {
  YGNodeStyleSetWidth(m_node, width);
}

void Node::setWidthPercent(double width) {
  YGNodeStyleSetWidthPercent(m_node, width);
}

void Node::setWidthAuto() {
  YGNodeStyleSetWidthAuto(m_node);
}

void Node::setHeight(double height) {
  YGNodeStyleSetHeight(m_node, height);
}

void Node::setHeightPercent(double height) {
  YGNodeStyleSetHeightPercent(m_node, height);
}

void Node::setHeightAuto() {
  YGNodeStyleSetHeightAuto(m_node);
}

void Node::setMinWidth(double minWidth) {
  YGNodeStyleSetMinWidth(m_node, minWidth);
}

void Node::setMinWidthPercent(double minWidth) {
  YGNodeStyleSetMinWidthPercent(m_node, minWidth);
}

void Node::setMinHeight(double minHeight) {
  YGNodeStyleSetMinHeight(m_node, minHeight);
}

void Node::setMinHeightPercent(double minHeight) {
  YGNodeStyleSetMinHeightPercent(m_node, minHeight);
}

void Node::setMaxWidth(double maxWidth) {
  YGNodeStyleSetMaxWidth(m_node, maxWidth);
}

void Node::setMaxWidthPercent(double maxWidth) {
  YGNodeStyleSetMaxWidthPercent(m_node, maxWidth);
}

void Node::setMaxHeight(double maxHeight) {
  YGNodeStyleSetMaxHeight(m_node, maxHeight);
}

void Node::setMaxHeightPercent(double maxHeight) {
  YGNodeStyleSetMaxHeightPercent(m_node, maxHeight);
}

void Node::setAspectRatio(double aspectRatio) {
  YGNodeStyleSetAspectRatio(m_node, aspectRatio);
}

void Node::setBorder(int edge, double border) {
  YGNodeStyleSetBorder(m_node, static_cast<YGEdge>(edge), border);
}

void Node::setPadding(int edge, double padding) {
  YGNodeStyleSetPadding(m_node, static_cast<YGEdge>(edge), padding);
}

void Node::setPaddingPercent(int edge, double padding) {
  YGNodeStyleSetPaddingPercent(m_node, static_cast<YGEdge>(edge), padding);
}

void Node::setGap(int gutter, double gapLength) {
  YGNodeStyleSetGap(m_node, static_cast<YGGutter>(gutter), gapLength);
}

int Node::getPositionType(void) const {
  return YGNodeStyleGetPositionType(m_node);
}

YGValue Node::getPosition(int edge) const {
  return YGNodeStyleGetPosition(m_node, static_cast<YGEdge>(edge));
}

int Node::getAlignContent(void) const {
  return YGNodeStyleGetAlignContent(m_node);
}

int Node::getAlignItems(void) const {
  return YGNodeStyleGetAlignItems(m_node);
}

int Node::getAlignSelf(void) const {
  return YGNodeStyleGetAlignSelf(m_node);
}

int Node::getFlexDirection(void) const {
  return YGNodeStyleGetFlexDirection(m_node);
}

int Node::getFlexWrap(void) const {
  return YGNodeStyleGetFlexWrap(m_node);
}

int Node::getJustifyContent(void) const {
  return YGNodeStyleGetJustifyContent(m_node);
}

YGValue Node::getMargin(int edge) const {
  return YGNodeStyleGetMargin(m_node, static_cast<YGEdge>(edge));
}

int Node::getOverflow(void) const {
  return YGNodeStyleGetOverflow(m_node);
}

int Node::getDisplay(void) const {
  return YGNodeStyleGetDisplay(m_node);
}

YGValue Node::getFlexBasis(void) const {
  return YGNodeStyleGetFlexBasis(m_node);
}

double Node::getFlexGrow(void) const {
  return YGNodeStyleGetFlexGrow(m_node);
}

double Node::getFlexShrink(void) const {
  return YGNodeStyleGetFlexShrink(m_node);
}

YGValue Node::getWidth(void) const {
  return YGNodeStyleGetWidth(m_node);
}

YGValue Node::getHeight(void) const {
  return YGNodeStyleGetHeight(m_node);
}

YGValue Node::getMinWidth(void) const {
  return YGNodeStyleGetMinWidth(m_node);
}

YGValue Node::getMinHeight(void) const {
  return YGNodeStyleGetMinHeight(m_node);
}

YGValue Node::getMaxWidth(void) const {
  return YGNodeStyleGetMaxWidth(m_node);
}

YGValue Node::getMaxHeight(void) const {
  return YGNodeStyleGetMaxHeight(m_node);
}

double Node::getAspectRatio(void) const {
  return YGNodeStyleGetAspectRatio(m_node);
}

double Node::getBorder(int edge) const {
  return YGNodeStyleGetBorder(m_node, static_cast<YGEdge>(edge));
}

YGValue Node::getPadding(int edge) const {
  return YGNodeStyleGetPadding(m_node, static_cast<YGEdge>(edge));
}

double Node::getGap(int gutter) {
  return YGNodeStyleGetGap(m_node, static_cast<YGGutter>(gutter));
}

void Node::insertChild(Node* child, unsigned index) {
  YGNodeInsertChild(m_node, child->m_node, index);
}

void Node::removeChild(Node* child) {
  YGNodeRemoveChild(m_node, child->m_node);
}

unsigned Node::getChildCount(void) const {
  return YGNodeGetChildCount(m_node);
}

Node* Node::getParent(void) {
  auto nodePtr = YGNodeGetParent(m_node);

  if (nodePtr == nullptr)
    return nullptr;

  return Node::fromYGNode(nodePtr);
}

Node* Node::getChild(unsigned index) {
  auto nodePtr = YGNodeGetChild(m_node, index);

  if (nodePtr == nullptr)
    return nullptr;

  return Node::fromYGNode(nodePtr);
}

void Node::setMeasureFunc(MeasureCallback *measureFunc) {
  m_measureFunc.reset(measureFunc);

  YGNodeSetMeasureFunc(m_node, &globalMeasureFunc);
}

void Node::unsetMeasureFunc(void) {
  m_measureFunc.reset(nullptr);

  YGNodeSetMeasureFunc(m_node, nullptr);
}

YGSize Node::callMeasureFunc(double width, YGMeasureMode widthMode, double height, YGMeasureMode heightMode) const {
  return m_measureFunc->measure(width, widthMode, height, heightMode);
}

void Node::setDirtiedFunc(DirtiedCallback *dirtiedFunc) {
  m_dirtiedFunc.reset(dirtiedFunc);

  YGNodeSetDirtiedFunc(m_node, &globalDirtiedFunc);
}

void Node::unsetDirtiedFunc(void) {
  m_dirtiedFunc.reset(nullptr);

  YGNodeSetDirtiedFunc(m_node, nullptr);
}

void Node::callDirtiedFunc(void) const {
  m_dirtiedFunc->dirtied();
}

void Node::markDirty(void) {
  YGNodeMarkDirty(m_node);
}

bool Node::isDirty(void) const {
  return YGNodeIsDirty(m_node);
}

void Node::calculateLayout(double width, double height, int direction) {
  YGNodeCalculateLayout(m_node, width, height, static_cast<YGDirection>(direction));
}

double Node::getComputedLeft(void) const {
  return YGNodeLayoutGetLeft(m_node);
}

double Node::getComputedRight(void) const {
  return YGNodeLayoutGetRight(m_node);
}

double Node::getComputedTop(void) const {
  return YGNodeLayoutGetTop(m_node);
}

double Node::getComputedBottom(void) const {
  return YGNodeLayoutGetBottom(m_node);
}

double Node::getComputedWidth(void) const {
  return YGNodeLayoutGetWidth(m_node);
}

double Node::getComputedHeight(void) const {
  return YGNodeLayoutGetHeight(m_node);
}

Layout Node::getComputedLayout(void) const {
  Layout layout;

  layout.left = YGNodeLayoutGetLeft(m_node);
  layout.right = YGNodeLayoutGetRight(m_node);

  layout.top = YGNodeLayoutGetTop(m_node);
  layout.bottom = YGNodeLayoutGetBottom(m_node);

  layout.width = YGNodeLayoutGetWidth(m_node);
  layout.height = YGNodeLayoutGetHeight(m_node);

  return layout;
}

double Node::getComputedMargin(int edge) const {
  return YGNodeLayoutGetMargin(m_node, static_cast<YGEdge>(edge));
}

double Node::getComputedBorder(int edge) const {
  return YGNodeLayoutGetBorder(m_node, static_cast<YGEdge>(edge));
}

double Node::getComputedPadding(int edge) const {
  return YGNodeLayoutGetPadding(m_node, static_cast<YGEdge>(edge));
}
