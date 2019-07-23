int limit(int input, int min, int max) {
  if ( input > max ) return max;
  if ( input < min ) return min;
  return input;
}
bool inRange(float input, float min, float max) {
  if ( input > max ) return false;
  if ( input < min ) return false;
  return true;
}

float rescale(float input, float input_range, float output_range) {
  return input / input_range * output_range;
}

String upperCaseStr(String str) {
  str.toUpperCase();
  return str;
}

