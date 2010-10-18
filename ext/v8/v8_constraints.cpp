#include "v8.h"
#include "platform.h"
#include "v8_ref.h"
#include "v8_constraints.h"

using namespace v8;

namespace {
  VALUE ResourceConstraintsClass;

  VALUE SetConstraints(VALUE self, VALUE max_young_space_size, VALUE max_old_space_size) {
    ResourceConstraints constraints;
    constraints.set_max_young_space_size(NUM2INT(rb_to_int(max_young_space_size)));
    constraints.set_max_old_space_size(NUM2INT(rb_to_int(max_old_space_size)));
    return rr_v82rb(SetResourceConstraints(&constraints));
  }
}

void rr_init_constraints() {
  ResourceConstraintsClass = rr_define_class("ResourceConstraints");
  rr_define_singleton_method(ResourceConstraintsClass, "SetConstraints", SetConstraints, 2);
}
