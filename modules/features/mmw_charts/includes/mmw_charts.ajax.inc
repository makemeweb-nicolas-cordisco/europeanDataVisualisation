<?php

/**
 * @file
 * MMW Charts ajax function file.
 *
 * @brief All ajax callback functions and function for return ajax page.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * The magic function when return the ajax request.
 *
 * @params mixed $page_callback_result
 *   The value to render from an ajax callback.
 */
function _mmw_charts_ajax_callback($page_callback_result) {
  // Only render content.
  $content = drupal_render($page_callback_result);
  print $content;
  // Perform end-of-request tasks.
  drupal_page_footer();
}

/**
 * Generate a json array with information for fustioncharts.
 */
function _mmw_charts_get_json_action() {
  $gets = filter_input_array(INPUT_GET);
  $static_data = _mmw_charts_get_static_data($gets['nid']);

  // Chart type ( radar, msline, ...).
  $type = $gets['type'];
  // Data themes selected, to be display on the graph.
  $themes = $gets['themes'];

  // Get Json array.
  $node = node_load($gets['nid']);
  $data_json = _mmw_charts_json_to_array($node);

  // Countries available for this data file.
  $countries_array = $data_json['countries'];

  // Method defined in config $static_data, used for generate json.
  $method = $static_data['graphTypes'][$type]['process_data_function'];
  $data_processed = array();

  // Some parameters are not usefull for all graph but we keep it as parameters
  // for processing data method to avoid to complicate code.
  if ($static_data['graphTypes'][$type]['categories_multiselect']) {
    $full_data = array("categories" => array(array("category" => "")), "dataset" => array());
  }
  else {
    $full_data = array();
  }

  // Chart options added to json data to configure display
  // of FusionCharts charts.
  $chart_option = $static_data['graphTypes'][$type]['chart_options'];

  // Execption for map chart, the key used in fusionChart
  // is "map" isntead of "chart".
  if ($type == "maps/europe2") {
    $full_data['map'] = $chart_option;
  }
  else {
    $full_data['chart'] = $chart_option;
  }

  // Getting json using the chart's spefific $method function.
  $data_processed = $method($themes, $full_data, $type, $countries_array, $static_data);
  $data_to_send["data"] = $data_processed['fullData'];

  echo json_encode($data_to_send);
}

/**
 * Generate json for radar fusionchart.
 *
 * @param array $themes
 *   Data themes selected, to be display on the chart.
 * @param array $full_data
 *   Contain all data for all countries and years not
 *   only those selected by user.
 * @param array $type
 *   Chart type ( radar, msline, ...).
 * @param array $countries_array
 *   Countries available in xls data file.
 * @param array $static_data
 *   Static data defined by chart type to configure chart
 *   display and data generation.
 *
 * @return array $data
 *   Data array to be convert in json.
 */
function _mmw_charts_get_json_radar($themes, $full_data, $type, $countries_array, $static_data) {
  $categories = array();
  // Use to convert country code to code used by the client
  // (Interinstitutional style guide).
  $countries_mapping = $static_data['countryMapping'];
  $units = array();
  $title = "";

  $node = node_load($static_data['nid']);
  // Full data of the xls file sort by themes.
  $data_json = _mmw_charts_json_to_array($node);

  // For each themes selected by user.
  foreach ($themes as $theme) {
    $data = $data_json[$theme];
    $title .= $data['title'] . ', ';
    $units[$data['unit']] = $data['unit'];

    // Build dataset for dom data.
    foreach ($data['data'] as $country_key => $country_data) {
      // Dataset categories = countries.
      $categories[$countries_mapping[$country_key]] = array();
      $categories[$countries_mapping[$country_key]]['label'] = $countries_array[$country_key] . "";

      // Sort data by year to simplify data change with timeline.
      foreach ($country_data as $year_key => $val) {
        $tooltip = 'Data: ' . $data['title'] . '{br}Country: ' . $countries_array[$country_key] . '{br}Value: ' . round($val, 2) . ' (' . $data['unit'] . ')';
        $full_data['dataset'][$theme][$year_key][$countries_mapping[$country_key]]['data'][] = array('value' => $val, 'toolText' => $tooltip);
      }
      $full_data['dataset'][$theme]['seriesname'] = htmlspecialchars_decode($data['title']) . '';
    }
  }
  $data = array();
  // Categories are added to chart options (in js) after each change
  // by user (chart type, themes or countries).
  $full_data['categories'] = $categories;
  // Chart title.
  $full_data['chart']['caption'] = htmlspecialchars_decode(trim($title, ', ')) . ' (' . implode(',', $units) . ')';
  $data['fullData'] = $full_data;

  return $data;
}

/**
 * Generate json chart where it is not possible to select more than one theme.
 *
 * @param array $themes
 *   Data themes selected, to be display on the chart.
 * @param array $full_data
 *   Contain all data for all countries and years not
 *   only those selected by user.
 * @param array $type
 *   Chart type ( radar, msline, ...).
 * @param array $countries_array
 *   Countries available in xls data file.
 * @param array $static_data
 *   Static data defined by chart type to configure chart
 *   display and data generation.
 *
 * @return array $data
 *   Data array to be convert in json.
 */
function _mmw_charts_get_json_single_select($themes, $full_data, $type, $countries_array, $static_data) {
  $node = node_load($static_data['nid']);
  // Full data of the xls file sort by themes.
  $data_json = _mmw_charts_json_to_array($node);

  // Selected theme data.
  $theme_data = $data_json[$themes[0]];
  $colors = $static_data['colors'];
  $countries_mapping = $static_data['countryMapping'];
  // Build dataset for dom data.
  foreach ($theme_data['data'] as $country_key => $country_data) {
    if ($country_key != 'eu28' && $country_key != 'eu27') {
      foreach ($country_data as $year_key => $val) {
        $tooltip = 'Country: ' . $countries_array[$country_key] . '{br}Value: ' . round($val, 2) . ' (' . $theme_data['unit'] . ')';
        $full_data['data'][$year_key][$countries_mapping[$country_key]] = array(
          'label' => $countries_array[$country_key],
          'value' => $val,
          'color' => $colors[$country_key],
          'toolText' => $tooltip,
        );
      }
    }
  }
  $data = array();
  $full_data['chart']['caption'] = htmlspecialchars_decode($theme_data['title']);
  $data['fullData'] = $full_data;
  return $data;
}

/**
 * Generate json for europe map.
 *
 * @param array $themes
 *   Data themes selected, to be display on the chart.
 * @param array $full_data
 *   Contain all data for all countries and years not
 *   only those selected by user.
 * @param array $type
 *   Chart type ( radar, msline, ...).
 * @param array $countries_array
 *   Countries available in xls data file.
 * @param array $static_data
 *   Static data defined by chart type to configure chart
 *   display and data generation.
 *
 * @return array $data
 *   Data array to be convert in json.
 */
function _mmw_charts_get_json_map($themes, $full_data, $type, $countries_array, $static_data) {
  $countries_mapping = $static_data['countryMapping'];
  $units = array();
  // Max and min are used to defined the chart color range.
  $max = 0;
  $min = 9999999999999;

  $node = node_load($static_data['nid']);
  $data_json = _mmw_charts_json_to_array($node);

  foreach ($themes as $theme) {
    $data = $data_json[$theme];
    $title .= $data['title'] . ', ';
    $units[$data['unit']] = $data['unit'];

    foreach ($data['data'] as $country_key => $country_data) {
      if ($country_key != "eu28" && $country_key != "eu27") {
        foreach ($country_data as $year_key => $val2) {

          $mappedCountryKey = $countries_mapping[$country_key];
          // Exception for slovenia and slovakia.
          if ($mappedCountryKey == 'SK') {
            $mappedCountryKey = "EU.SK";
          }
          if ($mappedCountryKey == 'SI') {
            $mappedCountryKey = "SK";
          }

          // Max & min are calculated between all year and themes are added up.
          if (array_key_exists($mappedCountryKey, $full_data['data'][$year_key])) {
            $val2 += $full_data['data'][$year_key][$mappedCountryKey]['value'];
          }
          if ($val2 > $max) {
            $max = $val2;
          }
          if ($val2 < $min) {
            $min = $val2;
          }
          $full_data['data'][$year_key][$country_key] = array(
            'id' => $mappedCountryKey,
            'value' => round($val2, 2),
          );
        }
      }
    }
  }

  // Delete country key in array.
  foreach ($full_data['data'] as $year_key => $year_data) {
    $full_data['data'][$year_key] = array_values($year_data);
  }
  $colorrange = $static_data['graphTypes'][$type]['colorrange'];
  $colorrange['minvalue'] = round($min, 2);
  // $colorrange['color'][0]["maxvalue"] = ($max - $min) / 2;.
  $colorrange['color'][0]["maxvalue"] = round($max, 2);
  $full_data['colorrange'] = $colorrange;

  unset($full_data['dataset']);
  unset($full_data['categories']);

  $data = array();
  $full_data['map']['caption'] = htmlspecialchars_decode(trim($title, ", ")) . " (" . implode(', ', $units) . ")";;
  $full_data['map']['yaxisname'] = implode(",", $units);
  $data['fullData'] = $full_data;

  return $data;
}

/**
 * Generate json for charts where all years are directly display.
 *
 * @param array $themes
 *   Data themes selected, to be display on the chart.
 * @param array $full_data
 *   Contain all data for all countries and years not
 *   only those selected by user.
 * @param array $type
 *   Chart type ( radar, msline, ...).
 * @param array $countries_array
 *   Countries available in xls data file.
 * @param array $static_data
 *   Static data defined by chart type to configure chart
 *   display and data generation.
 *
 * @return array $data
 *   Data array to be convert in json.
 */
function _mmw_charts_get_json_all_year($themes, $full_data, $type, $countries_array, $static_data) {
  $countries_mapping = $static_data['countryMapping'];
  $colors = $static_data['colors'];
  $countries_excluded = $static_data['graphTypes'][$type]['countries_exclude'];
  $units = array();
  $title = "";
  $theme_count = 2;

  $node = node_load($static_data['nid']);
  $data_json = _mmw_charts_json_to_array($node);

  foreach ($themes as $theme) {
    $data = $data_json[$theme];
    $title .= $data['title'] . ", ";
    $units[$data['unit']] = $data['unit'];

    if ($theme_count == 2) {
      $first_country_data = current($data['data']);
      // Build categories.
      foreach ($first_country_data as $key => $val) {
        // Label have to be string.
        $full_data["categories"][0]["category"][]['label'] = $key . "";
      }
    }

    // Build dataset for dom data.
    foreach ($data['data'] as $country_key => $country_data) {
      if (!in_array($country_key, $countries_excluded)) {
        $country_key_mapped = $countries_mapping[$country_key];
        foreach ($country_data as $year => $val) {
          $tooltip = 'Data: ' . $data['title'] . '{br}Country: ' . $countries_array[$country_key] . '{br}Year: ' . $year . '{br}Value: ' . round($val, 2) . " (" . $data['unit'] . ")";
          $full_data['dataset'][$country_key_mapped][$theme]['data'][] = array("value" => $val, "toolText" => $tooltip);
        }
        $full_data['dataset'][$country_key_mapped][$theme]['color'] = $colors[$country_key];
        $full_data['dataset'][$country_key_mapped][$theme]['seriesname'] = htmlspecialchars_decode($data['title']) . ' ' . $country_key_mapped;
        // If not >2 display default circle anchor.
        if ($theme_count > 2 && $type != 'stackedarea2d') {
          $full_data['dataset'][$country_key_mapped][$theme]['anchorSides'] = $theme_count;
          $full_data['dataset'][$country_key_mapped][$theme]['drawAnchors'] = 1;
          $full_data['dataset'][$country_key_mapped][$theme]['anchorRadius'] = 5;
        }
      }
    }
    $theme_count++;
  }

  $data = array();
  $full_data['chart']['caption'] = htmlspecialchars_decode(trim($title, ", "));
  $full_data['chart']['yaxisname'] = implode(",", $units);
  $data['fullData'] = $full_data;

  return $data;
}

/**
 * Generate json for charts using dualY chart option.
 *
 * @param array $themes
 *   Data themes selected, to be display on the chart.
 * @param array $full_data
 *   Contain all data for all countries and years not
 *   only those selected by user.
 * @param array $type
 *   Chart type ( radar, msline, ...).
 * @param array $countries_array
 *   Countries available in xls data file.
 * @param array $static_data
 *   Static data defined by chart type to configure chart
 *   display and data generation.
 *
 * @return array $data
 *   Data array to be convert in json.
 */
function _mmw_charts_get_json_dual_y($themes, $full_data, $type, $countries_array, $static_data) {
  $countries_mapping = $static_data['countryMapping'];
  $colors = $static_data['colors'];
  $units = array();
  $theme_count = 2;
  $title = "";

  $node = node_load($static_data['nid']);
  $data_json = _mmw_charts_json_to_array($node);

  foreach ($themes as $theme) {
    $data = $data_json[$theme];
    $title .= $data['title'] . ', ';
    $units[$data['unit']] = $data['unit'];

    if ($theme_count == 2) {
      $first_country_data = current($data['data']);
      // Build categories.
      foreach ($first_country_data as $key => $val) {
        // Label have to be string.
        $full_data['categories'][0]['category'][]['label'] = $key . '';
      }
    }

    // Build dataset for dom data.
    foreach ($data['data'] as $country_key => $country_data) {
      $country_key_mapped = $countries_mapping[$country_key];
      foreach ($country_data as $year => $val) {
        $tooltip = 'Data: ' . $data['title'] . '{br}Country: ' . $countries_array[$country_key] . '{br}Year: ' . $year . '{br}Value: ' . round($val, 2) . " (" . $data['unit'] . ")";
        $full_data['dataset'][$country_key_mapped][$theme]['data'][] = array("value" => $val, "toolText" => $tooltip);
      }

      $full_data['dataset'][$country_key_mapped][$theme]['color'] = $colors[$country_key];
      $full_data['dataset'][$country_key_mapped][$theme]['seriesname'] = htmlspecialchars_decode($data['title']) . ' ' . $country_key_mapped;
      if ($country_key != 'eu28' && $country_key != 'eu27') {
        $full_data['dataset'][$country_key_mapped][$theme]['renderAs'] = "line";
        $full_data['dataset'][$country_key_mapped][$theme]['parentYAxis'] = "S";
      }
      else {
        // Change alpha value to be between 20 and 100 with the same gap
        // between each theme.
        $full_data['dataset'][$country_key_mapped][$theme]['alpha'] = 100 - (80 / (count($themes) - 1)) * ($theme_count - 2);
      }
      // If not >2 display default circle anchor.
      if ($theme_count > 2) {
        $full_data['dataset'][$country_key_mapped][$theme]['anchorSides'] = $theme_count;
        $full_data['dataset'][$country_key_mapped][$theme]['drawAnchors'] = 1;
        $full_data['dataset'][$country_key_mapped][$theme]['anchorRadius'] = 5;
      }
    }
    $theme_count++;
  }
  $data = array();
  $full_data['chart']['caption'] = htmlspecialchars_decode(trim($title, ', '));
  $full_data['chart']['pYAxisName'] = 'European Union (' . implode(',', $units) . ')';
  $full_data['chart']['sYAxisName'] = 'Countries (' . implode(',', $units) . ')';
  $data['fullData'] = $full_data;

  return $data;
}
