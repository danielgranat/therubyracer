
module V8
  class Constraints
    def self.set_constraints(max_young_space_size, max_old_space_size)
      C::ResourceConstraints::SetConstraints(max_young_space_size, max_old_space_size)
    end
  end
end

