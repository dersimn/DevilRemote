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

String uint64ToString(uint64_t input, uint8_t base = 10) { // https://github.com/crankyoldgit/IRremoteESP8266/blob/3584edc2bcf3c374407e7b950238242589ec9d24/src/IRutils.cpp#L49
  String result = "";
  if (base < 2) return "";
  if (base > 36) return ""; // Check we have a base that we can actually print. i.e. [0-9A-Z] == 36

  // Reserve some string space to reduce fragmentation.
  // 16 for hex
  // 20 for dez
  // 64 for bin
  result.reserve(20+2);

  do {
    char c = input % base;
    input /= base;

    if (c < 10)
      c += '0';
    else
      c += 'A' - 10;
    result = c + result;
  } while (input);
  return result;
}
