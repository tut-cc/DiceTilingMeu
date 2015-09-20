#pragma once

template <typename T>
class FieldEX : public Field {
public:
  virtual ~FieldEX() = default;
  virtual const T & raw() const = 0;
};
