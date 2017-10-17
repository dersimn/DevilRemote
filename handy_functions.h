int limit(int input, int min, int max) {
  if ( input > max ) return max;
  if ( input < min ) return min;
  return input;
}

float rescale(float input, float input_range, float output_range) {
  return input / input_range * output_range;
}

