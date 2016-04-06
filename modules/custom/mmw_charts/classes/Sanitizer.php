<?php
/**
 * @file
 * MMW Charts Class Sanitizer.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * Class for sanitize value of The Excel file and encode to UTF8 format.
 */
class Sanitizer {

  /**
   * Slugify the given string.
   *
   * @param string $string
   *   String to sanitize.
   *
   * @return string $string
   *   String sanitized.
   */
  public static function slugify($string) {

    // Remove tags.
    $string = strip_tags($string);
    // Preserve escaped octets.
    $string = preg_replace('|%([a-fA-F0-9][a-fA-F0-9])|', '---$1---', $string);
    // Remove percent signs that are not part of an octet.
    $string = str_replace('%', '', $string);
    // Restore octets.
    $string = preg_replace('|---([a-fA-F0-9][a-fA-F0-9])---|', '%$1', $string);

    if (self::seemsUtf8($string)) {
      if (function_exists('mb_strtolower')) {
        $string = mb_strtolower($string, 'UTF-8');
      }
      $string = self::utf8UriEncode($string, 200);
    }

    $string = strtolower($string);
    // Kill entities.
    $string = preg_replace('/&.+?;/', '', $string);
    $string = str_replace('.', '-', $string);
    $string = preg_replace('/[^%a-z0-9 _-]/', '', $string);
    $string = preg_replace('/\s+/', '-', $string);
    $string = preg_replace('|-+|', '-', $string);
    $string = trim($string, '-');

    return $string;
  }

  /**
   * Encode as UTF8 .
   *
   * @param string $utf8_string
   *   String to encode.
   * @param int $length
   *   Length.
   *
   * @return string $unicode
   *   String encoded.
   */
  private static function utf8UriEncode($utf8_string, $length = 0) {

    $unicode = '';
    $values = array();
    $num_octets = 1;
    $unicode_length = 0;

    $string_length = strlen($utf8_string);
    for ($i = 0; $i < $string_length; $i++) {
      $value = ord($utf8_string[$i]);

      if ($value < 128) {
        if ($length && ($unicode_length >= $length)) {
          break;
        }
        $unicode .= chr($value);
        $unicode_length++;
      }
      else {
        if (count($values) == 0) {
          $num_octets = ($value < 224) ? 2 : 3;
        }

        $values[] = $value;

        if ($length && ($unicode_length + ($num_octets * 3)) > $length) {
          break;
        }
        if (count($values) == $num_octets) {
          if ($num_octets == 3) {
            $unicode .= '%' . dechex($values[0]) . '%' . dechex($values[1]) . '%' . dechex($values[2]);
            $unicode_length += 9;
          }
          else {
            $unicode .= '%' . dechex($values[0]) . '%' . dechex($values[1]);
            $unicode_length += 6;
          }

          $values = array();
          $num_octets = 1;
        }
      }
    }

    return $unicode;
  }

  /**
   * If string seems to be utf 8 encoded .
   *
   * @param string $str
   *   String to check.
   *
   * @return bool
   *   Check result.
   */
  private static function seemsUtf8($str) {

    $length = strlen($str);
    for ($i = 0; $i < $length; $i++) {
      $c = ord($str[$i]);
      if ($c < 0x80) {
        // 0bbbbbbb.
        $n = 0;
      }
      elseif (($c & 0xE0) == 0xC0) {
        // 110bbbbb.
        $n = 1;
      }
      elseif (($c & 0xF0) == 0xE0) {
        // 1110bbbb.
        $n = 2;
      }
      elseif (($c & 0xF8) == 0xF0) {
        // 11110bbb.
        $n = 3;
      }
      elseif (($c & 0xFC) == 0xF8) {
        // 111110bb.
        $n = 4;
      }
      elseif (($c & 0xFE) == 0xFC) {
        // 1111110b.
        $n = 5;
      }
      else {
        // Does not match any model.
        return FALSE;
      }
      for ($j = 0; $j < $n; $j++) {
        // N bytes matching 10bbbbbb follow ?
        if ((++$i == $length) || ((ord($str[$i]) & 0xC0) != 0x80)) {
          return FALSE;
        }
      }
    }
    return TRUE;
  }

}
