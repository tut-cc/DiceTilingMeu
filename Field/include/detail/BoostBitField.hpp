#pragma once

template <typename T>
const T & BoostBitField<T>::raw() const
{
  T v;
  return t;
}
