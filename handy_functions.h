int limit(int input, int min, int max) {
  if ( input > max ) return max;
  if ( input < min ) return min;
  return input;
}

