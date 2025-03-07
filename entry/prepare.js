const CONSTANTS = exports.constants = {
  ALIGN_COUNT: 8,
  ALIGN_AUTO: 0,
  ALIGN_FLEX_START: 1,
  ALIGN_CENTER: 2,
  ALIGN_FLEX_END: 3,
  ALIGN_STRETCH: 4,
  ALIGN_BASELINE: 5,
  ALIGN_SPACE_BETWEEN: 6,
  ALIGN_SPACE_AROUND: 7,

  DIMENSION_COUNT: 2,
  DIMENSION_WIDTH: 0,
  DIMENSION_HEIGHT: 1,

  DIRECTION_COUNT: 3,
  DIRECTION_INHERIT: 0,
  DIRECTION_LTR: 1,
  DIRECTION_RTL: 2,

  DISPLAY_COUNT: 2,
  DISPLAY_FLEX: 0,
  DISPLAY_NONE: 1,

  EDGE_COUNT: 9,
  EDGE_LEFT: 0,
  EDGE_TOP: 1,
  EDGE_RIGHT: 2,
  EDGE_BOTTOM: 3,
  EDGE_START: 4,
  EDGE_END: 5,
  EDGE_HORIZONTAL: 6,
  EDGE_VERTICAL: 7,
  EDGE_ALL: 8,

  EXPERIMENTAL_FEATURE_COUNT: 1,
  EXPERIMENTAL_FEATURE_WEB_FLEX_BASIS: 0,

  FLEX_DIRECTION_COUNT: 4,
  FLEX_DIRECTION_COLUMN: 0,
  FLEX_DIRECTION_COLUMN_REVERSE: 1,
  FLEX_DIRECTION_ROW: 2,
  FLEX_DIRECTION_ROW_REVERSE: 3,

  GUTTER_COUNT: 3,
  GUTTER_COLUMN: 0,
  GUTTER_ROW: 1,
  GUTTER_ALL: 2,

  JUSTIFY_COUNT: 6,
  JUSTIFY_FLEX_START: 0,
  JUSTIFY_CENTER: 1,
  JUSTIFY_FLEX_END: 2,
  JUSTIFY_SPACE_BETWEEN: 3,
  JUSTIFY_SPACE_AROUND: 4,
  JUSTIFY_SPACE_EVENLY: 5,

  LOG_LEVEL_COUNT: 6,
  LOG_LEVEL_ERROR: 0,
  LOG_LEVEL_WARN: 1,
  LOG_LEVEL_INFO: 2,
  LOG_LEVEL_DEBUG: 3,
  LOG_LEVEL_VERBOSE: 4,
  LOG_LEVEL_FATAL: 5,

  MEASURE_MODE_COUNT: 3,
  MEASURE_MODE_UNDEFINED: 0,
  MEASURE_MODE_EXACTLY: 1,
  MEASURE_MODE_AT_MOST: 2,

  NODE_TYPE_COUNT: 2,
  NODE_TYPE_DEFAULT: 0,
  NODE_TYPE_TEXT: 1,

  OVERFLOW_COUNT: 3,
  OVERFLOW_VISIBLE: 0,
  OVERFLOW_HIDDEN: 1,
  OVERFLOW_SCROLL: 2,

  POSITION_TYPE_COUNT: 3,
  POSITION_TYPE_STATIC: 0,
  POSITION_TYPE_RELATIVE: 1,
  POSITION_TYPE_ABSOLUTE: 2,

  PRINT_OPTIONS_COUNT: 3,
  PRINT_OPTIONS_LAYOUT: 1,
  PRINT_OPTIONS_STYLE: 2,
  PRINT_OPTIONS_CHILDREN: 4,

  UNIT_COUNT: 4,
  UNIT_UNDEFINED: 0,
  UNIT_POINT: 1,
  UNIT_PERCENT: 2,
  UNIT_AUTO: 3,

  WRAP_COUNT: 3,
  WRAP_NO_WRAP: 0,
  WRAP_WRAP: 1,
  WRAP_WRAP_REVERSE: 2,
};

class Layout {
  constructor(left, right, top, bottom, width, height) {
    this.left = left;
    this.right = right;
    this.top = top;
    this.bottom = bottom;
    this.width = width;
    this.height = height;
  }

  fromJS(expose) {
    expose(
      this.left,
      this.right,
      this.top,
      this.bottom,
      this.width,
      this.height,
    )
  }

  toString() {
    return `<Layout#${this.left}:${this.right};${this.top}:${this.bottom};${this.width}:${this.height}>`;
  }
}

class Size {
  static fromJS({ width, height }) {
    return new Size(width, height);
  }

  constructor(width, height) {
    this.width = width;
    this.height = height;
  }

  fromJS(expose) {
    expose(this.width, this.height);
  }

  toString() {
    return `<Size#${this.width}x${this.height}>`;
  }
}

class Value {
  constructor(unit, value) {
    this.unit = unit;
    this.value = value;
  }

  fromJS(expose) {
    expose(this.unit, this.value);
  }

  toString() {
    switch (this.unit) {
      case CONSTANTS.UNIT_POINT:
        return String(this.value);
      case CONSTANTS.UNIT_PERCENT:
        return `${this.value}%`;
      case CONSTANTS.UNIT_AUTO:
        return 'auto';
      default: {
        return `${this.value}?`;
      }
    }
  }

  valueOf() {
    return this.value
  }
}

function patch(prototype, name, fn) {
  let original = prototype[name];

  prototype[name] = function (...args) {
    return fn.call(this, original, ...args);
  };
}

exports.prepare = lib => {
  for (let fnName of [
    'setPosition',
    'setMargin',
    'setFlexBasis',
    'setWidth',
    'setHeight',
    'setMinWidth',
    'setMinHeight',
    'setMaxWidth',
    'setMaxHeight',
    'setPadding',
  ]) {
    const methods = {
      [CONSTANTS.UNIT_POINT]: lib.Node.prototype[fnName],
      [CONSTANTS.UNIT_PERCENT]: lib.Node.prototype[`${fnName}Percent`],
      [CONSTANTS.UNIT_AUTO]: lib.Node.prototype[`${fnName}Auto`],
    };

    patch(lib.Node.prototype, fnName, function (original, ...args) {
      // We patch all these functions to add support for the following calls:
      // .setWidth(100) / .setWidth("100%") / .setWidth(.getWidth()) / .setWidth("auto")

      let value = args.pop();
      let unit, asNumber;

      if (value === 'auto') {
        unit = CONSTANTS.UNIT_AUTO;
        asNumber = undefined;
      } else if (value instanceof Value) {
        unit = value.unit;
        asNumber = value.valueOf();
      } else {
        unit = typeof value === 'string' && value.endsWith('%')
          ? CONSTANTS.UNIT_PERCENT
          : CONSTANTS.UNIT_POINT;
        asNumber = parseFloat(value);
        if (!Number.isNaN(value) && Number.isNaN(asNumber)) {
          throw new Error(`Invalid value ${value} for ${fnName}`);
        }
      }

      if (!methods[unit])
        throw new Error(`Failed to execute "${fnName}": Unsupported unit '${value}'`);

      if (asNumber !== undefined) {
        return methods[unit].call(this, ...args, asNumber);
      } else {
        return methods[unit].call(this, ...args);
      }
    });
  }

  patch(lib.Config.prototype, 'free', () => {
    // Since we handle the memory allocation ourselves (via lib.Config.create),
    // we also need to handle the deallocation
    lib.Config.destroy(this);
  });

  patch(lib.Node.prototype, 'free', function () {
    // Since we handle the memory allocation ourselves (via lib.Node.create),
    // we also need to handle the deallocation
    lib.Node.destroy(this);
  });

  patch(lib.Node.prototype, 'freeRecursive', function () {
    for (let t = 0, T = this.getChildCount(); t < T; ++t)
      this.getChild(0).freeRecursive();

    this.free();
  });

  patch(lib.Node.prototype, 'setMeasureFunc', function (original, measureFunc) {
    // https://github.com/vincentriemer/yoga-dom/blob/b1592710c64c85f610f9d963cd54c99bbf01ae03/src/index.js
    original.call(this, lib.MeasureCallback.implement({
      measure: (width, widthMode, height, heightMode) => {
        return measureFunc(width, widthMode.value, height, heightMode.value);
      },
    }));
  });

  patch(lib.Node.prototype, 'calculateLayout', function (original, width = NaN, height = NaN, direction = CONSTANTS.DIRECTION_LTR) {
    // Just a small patch to add support for the function default parameters
    return original.call(this, width, height, direction);
  });

  return lib;
}
