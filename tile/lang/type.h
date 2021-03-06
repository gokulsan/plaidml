#pragma once

#include <map>
#include <set>
#include <string>

#include "tile/lang/ops.h"
#include "tile/lang/shape.h"

namespace vertexai {
namespace tile {
namespace lang {

struct Binding {
  explicit Binding(const TensorShape& _shape) : tag(TENSOR), shape(_shape) {}
  explicit Binding(int64_t _iconst) : tag(ICONST), iconst(_iconst) { shape.type = DataType::INT32; }
  // TODO(): This can't be enabled if the device doesn't support FP16
  // explicit Binding(double _fconst) : tag(FCONST), fconst(_fconst) { shape.type = DataType::FLOAT16; }
  explicit Binding(double _fconst) : tag(FCONST), fconst(_fconst) { shape.type = DataType::FLOAT32; }
  enum { TENSOR, ICONST, FCONST } tag;
  TensorShape shape;
  int64_t iconst;
  double fconst;

  bool operator==(const Binding& rhs);
  bool operator!=(const Binding& rhs);
};

inline MAKE_LOGGABLE(Binding, t, os) {
  switch (t.tag) {
    case Binding::TENSOR:
      os << "T:" << t.shape;
      break;
    case Binding::ICONST:
      os << "I:" << t.iconst;
      break;
    case Binding::FCONST:
      os << "F:" << t.fconst;
      break;
  }
  return os;
}

typedef std::map<std::string, Binding> Bindings;

// Compute output from input shapes
// TODO: T383
// ShapeMap ComputeShapes(const std::string& code, const Bindings& inputs)

// Update the initial bindings in var to contain all variables.  Also, inline constant values into program
void TypeCheck(Program* prog, Bindings* vars);

// Do the whole ball of wax
Bindings BindProgram(Program* p, const ShapeMap& inputs, const ShapeMap& outputs);

}  // namespace lang
}  // namespace tile
}  // namespace vertexai
