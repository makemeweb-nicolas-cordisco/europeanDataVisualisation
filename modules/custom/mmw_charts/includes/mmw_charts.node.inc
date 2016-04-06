<?php
/**
 * @file
 * MMW Charts node function file.
 *
 * @brief All functions for the node render.
 *
 * @package mmw_charts
 *
 * @author Makemeweb
 */

/**
 * Implements hook_node_view().
 *
 * Add custom field for render in template.
 */
function mmw_charts_node_view($node, $view_mode, $langcode) {
  if (!empty($node->mmw_charts_json) && !empty($node->mmw_charts_json) && !empty($node->mmw_charts_json[LANGUAGE_NONE][0]['value'])) {
    mmw_charts_action($node);
  }
}

/**
 * Load the CSS and JS for charts page.
 *
 * @param string $module_path
 *   Path of this module.
 * @param bool $commission
 *   Load js from commission or from local.
 */
function mmw_charts_load_assets($module_path, $commission) {

  drupal_add_library('system', 'ui.slider');
  // The CSS filename to load.
  $css_files = array(
    'jquery-ui-slider-pips',
    'font-awesome.min',
    'awesome-bootstrap-checkbox',
    'main',
  );
  foreach ($css_files as $css_file) {
    drupal_add_css($module_path . '/assets/css/' . $css_file . '.css');
  }
  // The FusionCharts filename to load.
  if ($commission) {
    drupal_add_js('//europa.eu/webtools/load.js', array('defer' => TRUE));
  }
  else {
    $fusioncharts_files = array(
      'fusioncharts',
      'fusioncharts.charts',
      'themes/fusioncharts.theme.fint',
    );
    foreach ($fusioncharts_files as $fusioncharts_file) {
      drupal_add_js($module_path . '/assets/fusioncharts/js/' . $fusioncharts_file . '.js');
    }
  }

  // The JS filename to load.
  $js_files = array(
    'jquery-ui-slider-pips',
    'main',
    'export',
  );
  foreach ($js_files as $js_file) {
    drupal_add_js($module_path . '/assets/js/' . $js_file . '.js');
  }
}

/**
 * Prepare the template for charts page.
 *
 * @param object $node
 *   The node object.
 *
 * @see ../templates/node--advanced-fusioncharts.tpl.php
 */
function mmw_charts_action(&$node) {
  $data_json = charts_json_to_array($node);
  if (!empty($data_json)) {
    // Initalise variable for Javascript, used in main.js and export.js.
    drupal_add_js(array('mmw_charts' => array('node_id' => $node->nid, 'url_alias' => drupal_get_path_alias('node/' . $node->nid))), 'setting');
    // Get the module path.
    $module_path = drupal_get_path('module', 'mmw_charts');
    // Load CSS and JS.
    $commission = TRUE;
    mmw_charts_load_assets($module_path, $commission);
    // Prepare the variables for inject in node object.
    $gets = filter_input_array(INPUT_GET);
    $static_data = mmw_charts_get_static_data($node->nid);
    $type = ($gets && array_key_exists("type", $gets)) ? $gets['type'] : FALSE;
    $themes = ($gets && array_key_exists("themes", $gets)) ? $gets['themes'] : FALSE;
    $menu = $data_json['sMenu'];
    $countries = $data_json['countries'];
    $year_to_display = $data_json['years'];
    $other_charts_by_years = mmw_charts_get_nodes_by_data_type($node);
    // Set custom js.
    if (!empty($type)) {
      drupal_add_js($module_path . "/assets/js/" . $static_data['graphTypes'][$type]['custom_js']);
      if ($static_data['graphTypes'][$type]['select_country']) {
        if (!$commission) {
          drupal_add_js($module_path . '/assets/js/mini-map.js');
          $node->content['miniMapJs'] = "";
        }
        else {
          $node->content['miniMapJs'] = $GLOBALS['base_url'] . '/' . $module_path . '/assets/js/mini-map.js';
        }
      }
    }
    // Set variable in content for used in template.
    $node->content['commission'] = $commission;
    $node->content['baseChartJs'] = $GLOBALS['base_url'] . '/' . $module_path . '/assets/js/baseChart.js';
    $node->content['staticData'] = $static_data;
    $node->content['type'] = $type;
    $node->content['themes'] = $themes;
    $node->content['yearToDisplay'] = $year_to_display;
    $node->content['menu'] = $menu;
    $node->content['countries'] = $countries;
    $node->content['otherChartsByYears'] = $other_charts_by_years;
  }
}

/**
 * Check if node field json is not empty and return array of json decoded.
 *
 * @param object $node
 *   The node object.
 *
 * @return array
 *   If field is not empty, return an array of json decoded
 *   also not return an empty array.
 */
function charts_json_to_array($node) {
  if (!empty($node->mmw_charts_json) &&
      !empty($node->mmw_charts_json[LANGUAGE_NONE]) &&
      !empty($node->mmw_charts_json[LANGUAGE_NONE][0]) &&
      !empty($node->mmw_charts_json[LANGUAGE_NONE][0]['value'])) {
    $json = $node->mmw_charts_json[LANGUAGE_NONE][0]['value'];
    return drupal_json_decode($json);
  }
  return array();
}

/**
 * Get charts nodes by data type.
 *
 * @param object $node
 *   The node object.
 *
 * @return array $year
 *   A array of pair url/year
 */
function mmw_charts_get_nodes_by_data_type($node) {

  $year_current_node = field_get_items('node', $node, 'mmw_charts_year');
  $year = array($year_current_node[0]['value']);

  $other_nodes = array();
  $data_type = field_get_items('node', $node, 'mmw_charts_data_type');
  if (!empty($data_type)) {
    $query = new entityfieldquery();

    $query->entityCondition('entity_type', 'node')
      ->entityCondition('bundle', 'advanced_fusioncharts')
      ->propertyCondition('status', NODE_PUBLISHED)
      ->propertyCondition('nid', $node->nid, '<>')
      ->fieldCondition('mmw_charts_data_type', 'value', $data_type[0]['value'], '=');
    $result = $query->execute();
    if (isset($result['node'])) {
      $other_node_datas = array_keys($result['node']);
      $other_nodes = entity_load('node', $other_node_datas);
    }
    foreach ($other_nodes as $other_node) {
      $year_node = field_get_items('node', $other_node, 'mmw_charts_year');
      if (!empty($year_node)) {
        $year[drupal_get_path_alias('node/' . $other_node->nid)] = $year_node[0]['value'];
      }
    }
  }
  arsort($year);
  return $year;
}
